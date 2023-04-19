//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "maxmin.h"


Define_Module(RcNode);


void RcNode::initialize(){

    // TODO: discard map entries after a specified timeout, to avoid memory overflow

    fillBookkeepingInfo();

    bdInRate = ingress[getIndex()];
    bdOutRate = egress[getIndex()];
    EV << "Node " << getIndex() << " has in = " << bdInRate << " and e = " << bdOutRate << "\n";
    EV << "GateSize = " << GateSize << "\n";

    WATCH(numReceived);
    WATCH(numReceivedAsInterm);

    // Leader (module 0) sends the first message
    if (getIndex() == 0) {
        SelfTimer *stmsg = new SelfTimer();
        float duration = config.MaxMinMsgSize / bdOutRate;
//        stmsg->setMsgTimer(stmsg->getMsgTimer() + duration);
        stmsg->setMsgTimer(duration);
        // Todo: first message doesn't need to go through both pipes (self msgtimer and out msgtimer)
        scheduleAt(stmsg->getMsgTimer(), stmsg);
        WATCH(uScores[1]);
        WATCH(uScores[2]);
        WATCH(uScores[3]);

//        minRxNode = 1;
    }
    else {
        // schedule first probe msg

        intermSeqNum = 0;
    }

    registerSignals();

}


void RcNode::fillBookkeepingInfo(){
    GateSize = gateSize("gate");
    for (cModule::GateIterator i(this); !i.end(); i++) {
        cGate *gate = *i;
        int gateIndex = gate->getIndex();
        int peerIndex = gate->getPathEndGate()->getOwnerModule()->getIndex();
        if (peerIndex != getIndex()){
            peerToGate[peerIndex] = gateIndex;
            gateToPeer[gateIndex] = peerIndex;
//            EV << "node " << getIndex() << " - peer: " << peerIndex << " - gate: " << gateIndex << "\n";
            uScores[peerIndex] = config.InitUtilScore;
            rates[peerIndex] = aimdConfig.InitVal;
            tokenBuckets[peerIndex] = aimdConfig.InitVal;

            std::vector<int> msgIds;
            nodeToMsgsAcked[peerIndex] = msgIds;
            nodeToAIMDStatus[peerIndex] = CAN_DECREASE; // can apply MD update to this node iff true
        }
    }
}

void RcNode::registerSignals(){

    if (getIndex() == 0) {
        uScore1Signal = registerSignal("uScore1Signal");
        uScore2Signal = registerSignal("uScore2Signal");
        uScore3Signal = registerSignal("uScore3Signal");

        intermNodeSignal = registerSignal("intermNodeSignal");
    }
    else {
        numRxAsIntermSignal = registerSignal("numRxAsIntermSignal");
        rate1Signal = registerSignal("rate1Signal");
        rate2Signal = registerSignal("rate2Signal");
    }

    outQSizeSignal = registerSignal("outQSizeSignal");
    inQSizeSignal = registerSignal("inQSizeSignal");

    numRxSignal = registerSignal("numRxSignal");

    numDrOutQSignal = registerSignal("numDrOutQSignal");
    numDrInQSignal = registerSignal("numDrInQSignal");

}

// When updating the utility score of each peer, only consider messages for which the ACK timeout has elapsed.
int RcNode::getLastMsgIdToCheck(){
    for (int msgId = currMsgId; msgId >= 0; msgId--) {
        if (msgMap[msgId].timeOut){
            return msgId;
        }
    }
    return -1;
}

//std::map<int, int> numRx, int minRxNumInit
std::pair<int,int> RcNode::getMinRxNode(){

    int lastMsgId = getLastMsgIdToCheck();
    int firstMsgId = std::max(0, lastMsgId - config.TotalMsgsToCheck + 1);
//    int lastMsgId = currMsgId - config.DistFromCurrMsgId;

    EV << "firstMsgId: " << firstMsgId << ", lastMsgId: " << lastMsgId << "\n";

    if (lastMsgId < firstMsgId){
        EV << "lastMsgId < firstMsgId: will not update utility scores \n";
        return std::pair<int,int>(1,1);
    }

//    int lastMsgId = std::max(0, currMsgId - config.DistFromCurrMsgId);
    int numMsgsToCheck = lastMsgId - firstMsgId + 1;

    std::map<int, int> numRx;
    std::map<int, int>::iterator it;
    for (it = peerToGate.begin(); it != peerToGate.end(); it++){
        int nodeIdx = it->first;
        if (nodeIdx != config.LEADER_IDX){
            numRx[nodeIdx] = 0;
        }
    }
    for (int msgId = firstMsgId; msgId <= lastMsgId; msgId++){
        for (auto nodeIdx : msgMap[msgId].receivers){
            numRx[nodeIdx] ++ ;
        }
    }
    EV << "numRx map: \n";
    printMapIntToInt(numRx);


    int minRxNum = numMsgsToCheck;
    int minRxNodeIdx = 1;
//    std::map<int, int>::iterator it;
    for (it = numRx.begin(); it != numRx.end(); it++){
        int nodeIdx = it->first;
        int rxNum = it->second;
        if (rxNum < minRxNum){
            minRxNum = rxNum;
            minRxNodeIdx = nodeIdx;
        }
    }
    return std::pair<int,int>(minRxNodeIdx, minRxNum);
}


void RcNode::updateLeaderSchedule(){

    int numMsgs = config.LeaderScheduleSize;
    float prec = config.Prec;
    float epsilon = config.Epsilon;

    std::map<int, float> uScoresNorm;
    float uScoresSum = 0;
    for (auto const& [key, val] : uScores) uScoresSum += val;
    for (auto const& [key, val] : uScores) uScoresNorm[key] = val / uScoresSum;

    emit(uScore1Signal, uScoresNorm[1]);
    emit(uScore2Signal, uScoresNorm[2]);
    emit(uScore3Signal, uScoresNorm[3]);

    leaderSchedule = getLeaderSchedule(numNodes, numMsgs, uScoresNorm, prec, epsilon);

    for (int msgId = 0; msgId < config.LeaderScheduleSize; msgId++){
        if (leaderSchedule[msgId] == -1) {
            EV_WARN << "ISSUE: intermNodeIdx = -1 for msgId = " << currMsgId << "\n";
        }
    }

    EV << "Leader schedule:" << endl;
    printSchedule(leaderSchedule);

}


void RcNode::handleMaxMinMessage(MaxMinMsg *msg){

    int peerGate = msg->getArrivalGate()->getIndex();
    int peerIdx = gateToPeer[peerGate];
    // Message arrived.
//    EV << "Node " << getIndex() << " received message " << msg << "\n";

    // send ACK to sender
    sendACK(msg, peerIdx);
    numReceived ++ ;

    emit(numRxSignal, 1);

    // DONE: find connection between gate index and module index
    if (peerIdx == config.LEADER_IDX) {
        // Node serves as an intermediate node. Need to broadcast the message to all nodes except for the leader.
        MaxMinMsgInfo newMMMsgInfo;
        newMMMsgInfo.msgId = msg->getMsgId();
        newMMMsgInfo.numAcksExp = GateSize - 1;
        newMMMsgInfo.timeOut = false;
        newMMMsgInfo.intermSeqNum = intermSeqNum;

        msgMap[msg->getMsgId()] = newMMMsgInfo;

        EV << "Node " << getIndex() << " serving as intermediate for msgId = " << newMMMsgInfo.msgId << "\n";
        EV << "rates map: \n";
        printMapIntToFloat(rates);
        EV << "tokenBuckets map: \n";
        printMapIntToFloat(tokenBuckets);

//        std::vector<int> rx;
//        std::map<int, int>::reverse_iterator it;
        std::map<int, int>::iterator it;
        for (it = peerToGate.begin(); it != peerToGate.end(); it++){
            int dstIdx = it->first;
            if (dstIdx != config.LEADER_IDX) {
//                EV << "Node " << getIndex() << " will forward msgID " << msg->getMsgId() << " to node " << dstIdx << "\n";
                if (tokenBuckets[dstIdx] >= 1){
                    // TODO: augment newMMMsgInfo with set of expected receivers
//                    EV << "Node " << getIndex() << " will forward mmmsg with msgID " << msg->getMsgId() << " to node " << dstIdx << "\n";
//                    rx.push_back(dstIdx);
                    tokenBuckets[dstIdx] -- ;
                    MaxMinMsg *mmmsgCopy = msg->dup();
                    mmmsgCopy->setDestination(dstIdx);
                    handleOutMsg(mmmsgCopy);
                }
                tokenBuckets[dstIdx] += rates[dstIdx]; // should always add rate to the token bucket
            }
        }

        intermSeqNum ++ ;
//        sendMessage(msg, rx);
        numReceivedAsInterm ++ ;
        emit(numRxAsIntermSignal, 1);
    }
    else {
        bubble("RECEIVED mmmsg as dst node!");
        // DONE: send ACK to leader
        sendACK(msg, config.LEADER_IDX);
        delete msg;
    }
}

void RcNode::handleMaxMinACK(MaxMinACK *ammmsg){
    int peerGate = ammmsg->getArrivalGate()->getIndex();
    int peerIdx = gateToPeer[peerGate];
    int msgId = ammmsg->getMsgId();
    // Message arrived.
//    EV << "Node " << getIndex() << " received ACK message for msgID " << msgId << " from node " << peerIdx << "\n";

    if (getIndex() == config.LEADER_IDX){
        std::pair<int,int> minRxPair = getMinRxNode();
        int minRxNodeIdx = minRxPair.first;
        int minRxNum = minRxPair.second;
        EV << "MinRxNode nodeIdx: " << minRxNodeIdx << " received " << minRxNum << " messages.\n";

        if (minRxNodeIdx == peerIdx) {
            if (msgMap.at(msgId).timeOut == false){
                // increase utility score of this intermediate node
                int intermNodeIdx = msgMap[msgId].intermediate;
                uScores[intermNodeIdx] += config.UtilAddVal;
            }
        }
    }
    else {
        if (!nodeToMsgsAcked[peerIdx].empty()) {
            int lastMsgIdAcked = nodeToMsgsAcked[peerIdx].back();
            int thisIntermSeqNum = msgMap[msgId].intermSeqNum;
            int lastIntermSeqNum = msgMap[lastMsgIdAcked].intermSeqNum;
            // check if there is a gap in acknowledged messages
            int updateType = NO_UPDATE;
            if (thisIntermSeqNum > lastIntermSeqNum + 1){
                EV_WARN << "ACK TIMEOUT: Node " << getIndex() << " received non-consecutive ACK from node " << peerIdx << " for msgID " << msgId << "\n";
                // TODO: reduce rate to this peer (if this is not the leader?)
                if (nodeToAIMDStatus[peerIdx] == CAN_DECREASE) {
                    EV_WARN << "Mult Decrease: Node " << getIndex() << " will apply MD rate decrease to node " << peerIdx << "\n";
                    updateType = DECREASE;
                    nodeToAIMDStatus[peerIdx] = NO_DECREASE;

                    AIMDTimer *aimdtmsg = new AIMDTimer();
                    aimdtmsg->setPeerIdx(peerIdx);
                    aimdtmsg->setMsgId(msgId);
                    scheduleAt(simTime() + aimdConfig.MDInterval, aimdtmsg);
                }
            }
            else {
                // TODO: should we apply Add Increase every time we receive ACKS for consecutive msgIds?
                updateType = INCREASE;
            }
            applyAIMDUpdate(peerIdx, updateType);
        }
    }
    // TODO: should we check for any timeout before adding (msgId, peerIdx) in our records?
    msgMap[msgId].receivers.insert(peerIdx);
    nodeToMsgsAcked[peerIdx].push_back(msgId);

}


void RcNode::applyAIMDUpdate(int peerIdx, int updateType){
    // DONE: apply AIMD update
    if (updateType == NO_UPDATE) return;

    float currRate = rates[peerIdx];
    float newRate = currRate;
    if (updateType == DECREASE) newRate = std::max(aimdConfig.MinVal, aimdConfig.MultFactor * currRate);
//    if (probeMsgMap[msgId].timeOut) newRate = currRate;
    else if (updateType == INCREASE) newRate = std::min(aimdConfig.MaxVal, currRate + aimdConfig.AddVal);
    rates[peerIdx] = newRate;

    std::map<int, int>::iterator it;
    int rateValues[2];
    int i = 0;
    for (it = peerToGate.begin(); it != peerToGate.end(); it++){
        int dstIdx = it->first;
        if (dstIdx != config.LEADER_IDX) {
            rateValues[i] = rates[dstIdx];
            i++ ;
        }
    }
    emit(rate1Signal, rateValues[0]);
    emit(rate2Signal, rateValues[1]);
}

void RcNode::handleAIMDTimer(AIMDTimer *aimdtmsg){
    int peerIdx = aimdtmsg->getPeerIdx();
    int msgId = aimdtmsg->getMsgId();

    nodeToAIMDStatus[peerIdx] = CAN_DECREASE;
}


void RcNode::handleACKTimeOutMessage(AckTimeOut *atmsg){
    int msgId = atmsg->getMsgId();
    EV << "Node " << getIndex() << " received a self ACK Timeout for msgID " << msgId << "\n";
    msgMap.at(msgId).timeOut = true;
}

void RcNode::handleSelfTimerMessage(SelfTimer *stmsg){
    EV << "Current msgId = " << currMsgId << "\n";
    MaxMinMsg *mmmsg = generateMessage(currMsgId);

    MaxMinMsgInfo newMMMsgInfo;
    newMMMsgInfo.msgId = currMsgId;
    newMMMsgInfo.numAcksExp = GateSize;
    newMMMsgInfo.timeOut = false;

    EV << "Utility scores: \n";
    printMapIntToFloat(uScores);

    int localMsgId = currMsgId % config.LeaderScheduleSize;

    if (localMsgId == 0) {
        EV << "Updating leader schedule \n";
        updateLeaderSchedule();
    }

//    emit(uScore1Signal, uScores[1]);
//    emit(uScore2Signal, uScores[2]);
//    emit(uScore3Signal, uScores[3]);

    int intermNodeIdx = leaderSchedule[localMsgId];

//    if (intermNodeIdx == -1) {
//        EV_WARN << "ISSUE: intermNodeIdx = -1 for msgId = " << currMsgId << "\n";
//    }

    emit(intermNodeSignal, intermNodeIdx);

    mmmsg->setDestination(intermNodeIdx);
    newMMMsgInfo.intermediate = intermNodeIdx;
    EV << "Next intermediate node: " << mmmsg->getDestination() << "\n";

    msgMap[currMsgId] = newMMMsgInfo;

    handleOutMsg(mmmsg);

    AckTimeOut *ackTimeOut = new AckTimeOut();
    ackTimeOut->setMsgId(currMsgId);
    scheduleAt(simTime() + config.ACKTimeoutDuration, ackTimeOut);

    currMsgId++;

    SelfTimer *newstmsg = new SelfTimer();
    float duration = config.MaxMinMsgSize / bdOutRate;
    newstmsg->setMsgTimer(stmsg->getMsgTimer() + duration);
    scheduleAt(newstmsg->getMsgTimer(), newstmsg);
}


void RcNode::handleInMessageTimer(InMsgTimer *intmsg){
    int inMsgId = intmsg->getMsgId();
//    int inMsgType = intmsg->getMsgType();
    // msgType: MaxMinMsg = 0, ProbeMsg = 1
    cMessage *msg = inMsgMap.at(inMsgId);

    if(instanceof<MaxMinMsg>(msg)) {
        MaxMinMsg *mmmsg = check_and_cast<MaxMinMsg *>(msg);
        handleMaxMinMessage(mmmsg);
    }

    inQueueBusy = false;
    if (!inQueueBusy){
        if (!inQueue.empty()){
            inQueueBusy = true;
            processNextInMsg();
        }
    }
}

void RcNode::handleOutMessageTimer(OutMsgTimer *outtmsg){
    int outMsgId = outtmsg->getMsgId();
//    int outMsgDstIdx = outtmsg->getDestination();
    RCMessage *msg = outMsgMap.at(outMsgId);

    forwardMessage(msg, msg->getDestination());

    outQueueBusy = false;
    if (!outQueueBusy){
        if (!outQueue.empty()){
            outQueueBusy = true;
            processNextOutMsg();
        }
    }
}

void RcNode::handleMessage(cMessage *msg){

    if (msg->isSelfMessage()) {
        if(instanceof<SelfTimer>(msg)){
            SelfTimer *stmsg = check_and_cast<SelfTimer *>(msg);
            handleSelfTimerMessage(stmsg);
        }
        else if(instanceof<InMsgTimer>(msg)){
            InMsgTimer *intmsg = check_and_cast<InMsgTimer *>(msg);
            handleInMessageTimer(intmsg);
        }
        else if(instanceof<OutMsgTimer>(msg)){
            OutMsgTimer *outtmsg = check_and_cast<OutMsgTimer *>(msg);
            handleOutMessageTimer(outtmsg);
        }
        else if(instanceof<AckTimeOut>(msg)){
            AckTimeOut *atmsg = check_and_cast<AckTimeOut *>(msg);
            handleACKTimeOutMessage(atmsg);
        }
        else if(instanceof<AIMDTimer>(msg)){
            AIMDTimer *aimdtmsg = check_and_cast<AIMDTimer *>(msg);
            handleAIMDTimer(aimdtmsg);
        }

    }

    // msg is not a self message
    if(instanceof<MaxMinMsg>(msg)) {
//        EV << "Node " << getIndex() << " received msg " << msg << " of type MaxMinMsg or ProbeMsg \n";
        int msgId = msg->getId();

        if (inQueue.size() < config.InQueueMaxSize){
            inMsgMap[msgId] = msg; // todo: should I duplicate the message here?
            inQueue.push(msgId);
        }
        else {
            EV_WARN << "Node " << getIndex() << ": IN Queue is full, dropped message " << msg << "\n";

            emit(numDrInQSignal, 1);
        }

        emit(inQSizeSignal, inQueue.size());

        if (!inQueueBusy){
            if (!inQueue.empty()){
                inQueueBusy = true;
                processNextInMsg();
            }
        }
    }
    else if(instanceof<MaxMinACK>(msg)) {
//        EV << "msg " << msg << " is of type ACKMsg \n";
        MaxMinACK *ammmsg = check_and_cast<MaxMinACK *>(msg);
        handleMaxMinACK(ammmsg);
    }

}


void RcNode::handleOutMsg(RCMessage *msg){
//    MaxMinMsg *mmmsg = check_and_cast<MaxMinMsg *>(msg);

    int msgId = msg->getId();
//    std::pair<int,int> pair = std::pair<int,int>(mmmsg->getMsgId(), mmmsg->getDestination());

    if (outQueue.size() < config.OutQueueMaxSize){
        outMsgMap[msgId] = msg; // todo: should I duplicate the message here?
        outQueue.push(msgId);
    }
    else {
        EV_WARN << "Node " << getIndex() << ": OUT Queue is full, dropped message " << msg << "\n";
        emit(numDrOutQSignal, 1);
    }

    emit(outQSizeSignal, outQueue.size());

    if (!outQueueBusy){
        if (!outQueue.empty()){
            outQueueBusy = true;
            processNextOutMsg();
        }
    }
}

// assumes that outQueue is not empty
void RcNode::processNextOutMsg(){
//    std::pair<int,int> pair = outQueue.front();
    int outMsgId = outQueue.front();
//    int outMsgDstIdx = pair.second;
    OutMsgTimer *outtmsg = new OutMsgTimer();
    outtmsg->setMsgId(outMsgId);
//    outtmsg->setDestination(outMsgDstIdx);
    float duration = config.MaxMinMsgSize / bdOutRate;
    scheduleAt(simTime() + duration, outtmsg);
    outQueue.pop();
}

// assumes that inQueue is not empty
void RcNode::processNextInMsg(){
//    std::pair<int,int> pair = inQueue.front();
//    int inMsgType = pair.first;
    int inMsgId = inQueue.front();
    cMessage *msg = inMsgMap[inMsgId];
    InMsgTimer *intmsg = new InMsgTimer();
//    intmsg->setMsgType(inMsgType);
    intmsg->setMsgId(inMsgId);
    float duration;
    if (instanceof<MaxMinMsg>(msg)) {
//        EV << "Node " << getIndex() << " processing maxmin message " << msg << "\n";
        duration = config.MaxMinMsgSize / bdInRate;
    }
    scheduleAt(simTime() + duration, intmsg);
    inQueue.pop();

}

void RcNode::sendACK(MaxMinMsg *msg, int dstIdx){
    int msgId = msg->getMsgId();
    char msgname[20];
    sprintf(msgname, " ACK-max-min-msg-%d", msgId);

    MaxMinACK *ack = new MaxMinACK(msgname);
    int src = getIndex();  // our module index
    ack->setSource(src);
    ack->setMsgId(msgId);
    ack->setDestination(dstIdx);
//    EV << "Node " << getIndex() << " sending ACK for msgID " << msgId << " on gate[" << dstIdx << "]\n";
    forwardMessage(ack, dstIdx);
}


void RcNode::forwardMessage(cMessage *msg, int dstIdx){
//    EV << "Node " << getIndex() << " sending message " << msg << " on gate[" << gateIdx << "]\n";
    send(msg, "gate$o", peerToGate[dstIdx]);
}

MaxMinMsg *RcNode::generateMessage(int msgId){
    // Produce source and destination addresses.
    int src = getIndex();  // our module index
    char msgname[20];
    sprintf(msgname, " max-min-msg-%d", msgId);
    // Create message object and set source and destination field.
    MaxMinMsg *msg = new MaxMinMsg(msgname);
    msg->setSource(src);
    msg->setMsgId(msgId);
    return msg;
}


