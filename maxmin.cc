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

    // Providing a seed value
    srand(getIndex());

    // Get a random number
    int random1 = rand();
    int random2 = rand();

    // Print the random number
    EV << "Random number 1: " << random1 << endl;
    EV << "Random number 2: " << random2 << endl;

    // todo: discard map entries after a specified timeout, to avoid memory overflow

    fillBookkeepingInfo();

    bdInRate = ingress[getIndex()];
    bdOutRate = egress[getIndex()];
    EV << "Node " << getIndex() << " has in = " << bdInRate << " and e = " << bdOutRate << "\n";
    EV << "GateSize = " << GateSize << "\n";
//    msgTimeOut = 2;

    WATCH(numReceived);
    WATCH(numRxAsInterm);

    // Leader (module 0) sends the first message
    if (getIndex() == 0) {
        SelfTimer *stmsg = new SelfTimer();
        float duration = config.MsgSize / bdOutRate;
//        stmsg->setMsgTimer(stmsg->getMsgTimer() + duration);
        stmsg->setMsgTimer(duration);
        // Todo: first message doesn't need to go through both pipes (self msgtimer and out msgtimer)
        scheduleAt(stmsg->getMsgTimer(), stmsg);
    }

    // schedule first probe msg
    ProbeSelfTimer *pstmsg = new ProbeSelfTimer();
    float pmDuration = aimdConfig.ProbeMsgFreq;
    pstmsg->setMsgTimer(pmDuration);
    pstmsg->setMsgId(getIndex());
    scheduleAt(pstmsg->getMsgTimer(), pstmsg);

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
            dstNodesRates[peerIndex] = aimdConfig.InitVal;
            distFromNextMsg[peerIndex] = 1;
        }
    }

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

std::pair<int,int> RcNode::getMinRxNode(std::map<int, int> numRx, int minRxNumInit){
    int minRxNum = minRxNumInit;
    int minRxNodeIdx = 1;
    std::map<int, int>::iterator it;
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

void RcNode::updateUScores(){
    int lastMsgId = getLastMsgIdToCheck();
    int firstMsgId = std::max(0, lastMsgId - config.TotalMsgsToCheck + 1);
//    int lastMsgId = currMsgId - config.DistFromCurrMsgId;

    EV << "firstMsgId: " << firstMsgId << ", lastMsgId: " << lastMsgId << "\n";

    if (lastMsgId < firstMsgId){
        EV << "lastMsgId < firstMsgId: will not update utility scores \n";
        return;
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

    std::pair<int,int> minRxPair = getMinRxNode(numRx, numMsgsToCheck);
    int minRxNodeIdx = minRxPair.first;
    int minRxNum = minRxPair.second;

    EV << "MinRxNode nodeIdx: " << minRxNodeIdx << " received " << minRxNum << " messages.\n";

    std::map<int, int> numSentToMinRx;
    std::map<int, int> numRxAsInterm;
//    std::map<int, int>::iterator it;
    for (int msgId = firstMsgId; msgId <= lastMsgId; msgId++){
        int intermNodeIdx = msgMap[msgId].intermediate;
        numRxAsInterm[intermNodeIdx] ++;

        if (intermNodeIdx == minRxNodeIdx){
            numSentToMinRx[intermNodeIdx] ++;
        }
        else{
            for (auto dstNodeIdx : msgMap[msgId].receivers){
                if (dstNodeIdx == minRxNodeIdx){
                    numSentToMinRx[intermNodeIdx] ++;
                }
            }
        }
    }

    EV << "numSentToMinRx map: \n";
    printMapIntToInt(numSentToMinRx);


    for (it = peerToGate.begin(); it != peerToGate.end(); it++){
        int nodeIdx = it->first;
        if (numRxAsInterm[nodeIdx] > 0 && numMsgsToCheck > 0){
            uScores[nodeIdx] = (float)numSentToMinRx[nodeIdx] / (float)numRxAsInterm[nodeIdx];
        }
//        uScores[nodeIdx] = numSentToMinRx[nodeIdx] / (numMsgsToCheck * (GateSize - 1));
    }
}


void RcNode::handleMaxMinMessage(MaxMinMsg *msg){

    int peerGate = msg->getArrivalGate()->getIndex();
    int peerIdx = gateToPeer[peerGate];
    // Message arrived.
//    EV << "Node " << getIndex() << " received message " << msg << "\n";

    // send ACK to sender
    sendACK(msg, peerIdx);
    numReceived ++ ;

    // DONE: find connection between gate index and module index
    if (peerIdx == config.LEADER_IDX) {
        // Node serves as an intermediate node. Need to broadcast the message to all nodes except for the leader.
        MsgInfo newMsgInfo;
        newMsgInfo.msgId = msg->getMsgId();
        newMsgInfo.numAcksExp = GateSize - 1;
        newMsgInfo.timeOut = false;
//        msgMap.insert(std::pair<int, MsgInfo>(msg->getMsgId(), newMsgInfo));
        msgMap[msg->getMsgId()] = newMsgInfo;

        EV << "Node " << getIndex() << " serving as intermediate for msgId = " << newMsgInfo.msgId << "\n";
        EV << "dstNodesRates map: \n";
        printMapIntToFloat(dstNodesRates);
        EV << "distFromNextMsg map: \n";
        printMapIntToInt(distFromNextMsg);

        std::vector<int> rx;
        std::map<int, int>::iterator it;
        for (it = peerToGate.begin(); it != peerToGate.end(); it++){
            int dstIdx = it->first;
            if (dstIdx != config.LEADER_IDX) {
//                EV << "Node " << getIndex() << " will forward msgID " << msg->getMsgId() << " to node " << dstIdx << "\n";
                if (distFromNextMsg[dstIdx] == 1){
                    EV << "Node " << getIndex() << " will forward msgID " << msg->getMsgId() << " to node " << dstIdx << "\n";
                    rx.push_back(dstIdx);
                    distFromNextMsg[dstIdx] = ceil(1.0 / dstNodesRates[dstIdx]);
                }
                else {
                    distFromNextMsg[dstIdx] -- ;
                }

            }
        }
        sendMessage(msg, rx);
        numRxAsInterm ++ ;
    }
    else {
        bubble("RECEIVED mmmsg as dst node!");
        // TODO: send ACK to leader
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

    // TODO: do stuff with ACK

    // if msgId exists as key in msgMap
    if (msgMap.count(msgId)){
        if (msgMap[msgId].timeOut){
            EV_WARN << "ACK TIMEOUT: Node " << getIndex() << " received ACK after timeout for msgID " << msgId << "\n";
        }
        else{
            msgMap[msgId].receivers.insert(peerIdx);
            // check if have received all ACKS
//            EV << "Node " << getIndex() << " msgMap[msgId].receivers.size() = " << msgMap[msgId].receivers.size() << "\n";
//            EV << "Node " << getIndex() << " numAcksExp = " << msgMap[msgId].numAcksExp << "\n";
            if (msgMap[msgId].receivers.size() == msgMap[msgId].numAcksExp){
                EV << "Node " << getIndex() << " received all ACKS for msgID " << msgId << "\n";
            }
        }
    }
}

////////////////////////////// Probing Mechanism //////////////////////////////////////////////////////////

void RcNode::handleProbeMsg(ProbeMsg *pmsg){
    int peerGate = pmsg->getArrivalGate()->getIndex();
    int peerIdx = gateToPeer[peerGate];
//    EV << "Node " << getIndex() << " received probe msg from node with peerIdx = " << peerIdx << ", msg source = " << pmsg->getSource() << ", and msgId = " << pmsg->getMsgId() << "\n";
    ProbeACK *probeACK = new ProbeACK();
    probeACK->setMsgId(pmsg->getMsgId());
    forwardMessage(probeACK, peerIdx);
}

// main AIMD functionality is supported by this method
void RcNode::handleProbeMsgACK(ProbeACK *apmsg){
    int peerGate = apmsg->getArrivalGate()->getIndex();
    int peerIdx = gateToPeer[peerGate];
    long msgId = apmsg->getMsgId();
//    EV << "Node " << getIndex() << " received a Probe ACK from node " << peerIdx << "\n";

    // todo: apply AIMD update

    float currRate = dstNodesRates[peerIdx];
    float newRate;
    if (probeMsgMap[msgId].timeOut) newRate = std::max(aimdConfig.MinVal, aimdConfig.MultFactor * currRate);
    else newRate = std::min(aimdConfig.MaxVal, currRate + aimdConfig.AddVal);
    dstNodesRates[peerIdx] = newRate;
//    distFromNextMsg[peerIdx] = ceil(1 / newRate);
}

void RcNode::handleProbeSelfTimer(ProbeSelfTimer *pstmsg){
    long currProbeMsgId = pstmsg->getMsgId();
    ProbeMsg *pmsg = new ProbeMsg();
    pmsg->setMsgId(currProbeMsgId);
    pmsg->setSource(getIndex());

    ProbeMsgInfo newProbeMsgInfo;
    newProbeMsgInfo.msgId = currProbeMsgId;
    newProbeMsgInfo.timeOut = false;

    std::map<int, int>::iterator it;
    for (it = peerToGate.begin(); it != peerToGate.end(); it++){
        int dstIdx = it->first;
        ProbeMsg *pmsgCopy = pmsg->dup();
        pmsgCopy->setDestination(dstIdx);
        // send probe message directly to peer. No need to place probe message in outQueue ?
        forwardMessage(pmsgCopy, dstIdx);
    }
    probeMsgMap[currProbeMsgId] = newProbeMsgInfo;

    // todo: emit probe msg ack timeout which applies for ALL dst indices
    ProbeAckTimeOut *probeAckTimeOut = new ProbeAckTimeOut();
    probeAckTimeOut->setMsgId(currProbeMsgId);
    scheduleAt(simTime() + aimdConfig.ProbeMsgTimeOut, probeAckTimeOut);

    // emit new self timer for the next probe message
    ProbeSelfTimer *newpstmsg = new ProbeSelfTimer();
    float pmDuration = aimdConfig.ProbeMsgFreq;
    newpstmsg->setMsgTimer(pstmsg->getMsgTimer() + pmDuration);
    newpstmsg->setMsgId(pstmsg->getMsgId() + (GateSize + 1));
    scheduleAt(newpstmsg->getMsgTimer(), newpstmsg);
}

void RcNode::handleProbeAckTimeOut(ProbeAckTimeOut *aptmsg){
    int msgId = aptmsg->getMsgId();
//    EV << "Node " << getIndex() << " received a self Probe ACK Timeout \n";
    probeMsgMap.at(msgId).timeOut = true;
}

////////////////////////////// End of Probing Mechanism //////////////////////////////////////////////////////////


void RcNode::handleACKTimeOutMessage(AckTimeOut *atmsg){
    int msgId = atmsg->getMsgId();
    EV << "Node " << getIndex() << " received a self ACK Timeout for msgID " << msgId << "\n";
    msgMap.at(msgId).timeOut = true;
}

void RcNode::handleSelfTimerMessage(SelfTimer *stmsg){
    EV << "Current msgId = " << currMsgId << "\n";
    MaxMinMsg *mmmsg = generateMessage(currMsgId);

    MsgInfo newMsgInfo;
    newMsgInfo.msgId = currMsgId;
    newMsgInfo.numAcksExp = GateSize;
    newMsgInfo.timeOut = false;

    EV << "Updating utility scores \n";
    updateUScores();
    EV << "Utility scores: \n";
    printMapIntToFloat(uScores);
    std::vector<std::pair<int,float>> nodesRanked = sortMapByValue(uScores);
//    int intermNodeIdx = nodesRanked.back().first;
    int intermNodeIdx = 1;
    EV << "Next intermediate node: " << intermNodeIdx << "\n";

    newMsgInfo.intermediate = intermNodeIdx;
    msgMap[currMsgId] = newMsgInfo;
//    msgMap.insert(std::pair<int, MsgInfo>(currMsgId, newMsgInfo)); // this map is for internal bookkeeping (check if all acks received)
//    EV << "msgMap at msgId: " << currMsgId << " has numAcksExp = " << msgMap[currMsgId].numAcksExp << "\n";

    std::vector<int> rx;
    rx.push_back(intermNodeIdx);
    sendMessage(mmmsg, rx);

    AckTimeOut *ackTimeOut = new AckTimeOut();
    ackTimeOut->setMsgId(currMsgId);
    scheduleAt(simTime() + config.ACKTimeoutDuration, ackTimeOut);

    currMsgId++;

    SelfTimer *newstmsg = new SelfTimer();
    float duration = config.MsgSize / bdOutRate;
    newstmsg->setMsgTimer(stmsg->getMsgTimer() + duration);
    scheduleAt(newstmsg->getMsgTimer(), newstmsg);
}

void RcNode::handleInMessageTimer(InMsgTimer *intmsg){
    int inMsgId = intmsg->getMsgId();
    int inMsgType = intmsg->getMsgType();
    // msgType: MaxMinMsg = 0, ProbeMsg = 1

    if (inMsgType == config.MAXMIN_MSG_TYPE) {
        MaxMinMsg *mmmsg = inMaxMinMsgMap[inMsgId];
        handleMaxMinMessage(mmmsg);
    }
    else if (inMsgType == config.PROBE_MSG_TYPE) {
        ProbeMsg *pmsg = inProbeMsgMap[inMsgId];
        int peerIdx = gateToPeer[pmsg->getArrivalGate()->getIndex()];
//        EV << "Node " << getIndex() << " received probe msg from node with peerIdx " << peerIdx << " and msg source " << pmsg->getSource() <<  "\n";
        handleProbeMsg(pmsg);
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
    int outMsgDstIdx = outtmsg->getDestination();
    MaxMinMsg *mmmsg = outMsgMap.at(std::pair<int,int>(outMsgId, outMsgDstIdx));

    forwardMessage(mmmsg, outMsgDstIdx);

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
        else if(instanceof<ProbeSelfTimer>(msg)){
            ProbeSelfTimer *pstmsg = check_and_cast<ProbeSelfTimer *>(msg);
            handleProbeSelfTimer(pstmsg);
        }
        else if(instanceof<ProbeAckTimeOut>(msg)){
            ProbeAckTimeOut *aptmsg = check_and_cast<ProbeAckTimeOut *>(msg);
            handleProbeAckTimeOut(aptmsg);
        }

    }

    // msg is not a self message
    if(instanceof<MaxMinMsg>(msg) || instanceof<ProbeMsg>(msg)) {
//        EV << "msg " << msg << " is of type MaxMinMsg or ProbeMsg \n";
        int msgId;
        int msgType;
        if(instanceof<MaxMinMsg>(msg)) {
            msgType = config.MAXMIN_MSG_TYPE;
            MaxMinMsg *mmmsg = check_and_cast<MaxMinMsg*>(msg);
            msgId = mmmsg->getMsgId();
            inMaxMinMsgMap[msgId] = mmmsg->dup();
        }
        else if(instanceof<ProbeMsg>(msg)) {
            msgType = config.PROBE_MSG_TYPE;
            ProbeMsg *pmsg = check_and_cast<ProbeMsg*>(msg);
            int peerIdx = gateToPeer[pmsg->getArrivalGate()->getIndex()];
//            EV << "Node " << getIndex() << " received probe msg from node with peerIdx " << peerIdx << " and msg source " << pmsg->getSource() <<  "\n";
            msgId = pmsg->getMsgId();
            inProbeMsgMap[msgId] = pmsg->dup();
        }

        std::pair<int,int> pair = std::pair<int,int>(msgType,msgId);
//        inMsgMap[pair] = msg->dup(); // todo: should I duplicate the message here?
        inQueue.push(pair);

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
    else if(instanceof<ProbeACK>(msg)) {
//        EV << "msg " << msg << " is of type ACKMsg \n";
        ProbeACK *apmsg = check_and_cast<ProbeACK *>(msg);
        handleProbeMsgACK(apmsg);
    }

}


void RcNode::handleOutMsg(MaxMinMsg *msg){
    MaxMinMsg *mmmsg = check_and_cast<MaxMinMsg *>(msg);

    std::pair<int,int> pair = std::pair<int,int>(mmmsg->getMsgId(), mmmsg->getDestination());
    outMsgMap[pair] = mmmsg->dup(); // todo: should I duplicate the message here?
    outQueue.push(pair);

    if (!outQueueBusy){
        if (!outQueue.empty()){
            outQueueBusy = true;
            processNextOutMsg();
        }
    }
}


// assumes that outQueue is not empty
void RcNode::processNextOutMsg(){
    std::pair<int,int> pair = outQueue.front();
    int outMsgId = pair.first;
    int outMsgDstIdx = pair.second;
    OutMsgTimer *outtmsg = new OutMsgTimer();
    outtmsg->setMsgId(outMsgId);
    outtmsg->setDestination(outMsgDstIdx);
    float duration = config.MsgSize / bdOutRate;
    scheduleAt(simTime() + duration, outtmsg);
    outQueue.pop();
}

// assumes that inQueue is not empty
void RcNode::processNextInMsg(){
    std::pair<int,int> pair = inQueue.front();
    int inMsgType = pair.first;
    int inMsgId = pair.second;
    InMsgTimer *intmsg = new InMsgTimer();
    intmsg->setMsgType(inMsgType);
    intmsg->setMsgId(inMsgId);
    float duration;
    if (inMsgType == config.MAXMIN_MSG_TYPE) duration = config.MsgSize / bdInRate;
    else if (inMsgType == config.PROBE_MSG_TYPE) duration = aimdConfig.ProbeMsgSize / bdInRate;
    scheduleAt(simTime() + duration, intmsg);
    inQueue.pop();

}

// broadcast message to all receivers in rx array. If rx is empty, broadcast to all peers
void RcNode::sendMessage(MaxMinMsg *msg, std::vector<int> rx){
    for (int dstIdx : rx){
        MaxMinMsg *copy = msg->dup();
        copy->setDestination(dstIdx);
        handleOutMsg(copy);
    }

//    if (rx.size() == 0) {
//        std::map<int, int>::iterator it;
//        for (it = peerToGate.begin(); it != peerToGate.end(); it++){
//            MaxMinMsg *copy = msg->dup();
//            int dstIdx = it->first;
////            EV << "Node " << getIndex() << " forwarding message " << msg << " on gate[" << peerToGate[it->first] << "]\n";
//            copy->setDestination(dstIdx);
//            handleOutMsg(copy);
//        }
//
//    } else {
//        for (int dstIdx : rx){
//            MaxMinMsg *copy = msg->dup();
//            copy->setDestination(dstIdx);
//            handleOutMsg(copy);
//        }
//    }

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


