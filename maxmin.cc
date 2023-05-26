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


float RcNode::getOptRate(){

    EV << endl << "Calculating optimal rate.." << endl;

    int N = GateSize + 1;

    float e0 = egress[config.LEADER_IDX];

    map<int, int>::iterator it;
    it = gateToPeer.begin();
    int nodeIdx = it->second;
    float in_min = ingress[nodeIdx];
    float sum_egr = e0 + egress[nodeIdx];
    it++;

    while (it != gateToPeer.end()){
        nodeIdx = it->second;
        sum_egr += egress[nodeIdx];
        if (ingress[nodeIdx] < in_min) in_min = ingress[nodeIdx];
        it ++ ;
    }

//    EV << "sum_egr = " << sum_egr << endl;

    float ecrit = sum_egr / ((float) (N-1));
    float ecrit_prime = (sum_egr - e0) / ((float) (N-2));

    EV << "e0 = " << e0 << endl;
    EV << "in_min = " << in_min << endl;
    EV << "ecrit = " << ecrit << endl;
    EV << "ecrit' = " << ecrit_prime << endl;

//    float ropt;
    if (ecrit_prime <= min(in_min, e0)){
        ropt = min(in_min, ecrit);
    }
    else {
        ropt = min(in_min, e0);
    }

    return ropt;
}


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

        float ropt = getOptRate();
        EV << "ropt = " << ropt << endl;

//        minRxNode = 1;
    }
    else {
        // schedule first probe msg

//        intermSeqNum = 0;
    }

    registerSignals();

    RxRateTimer *newrxrtmsg = new RxRateTimer();
    float interval = config.RxRateSignalInterval;
    scheduleAt(simTime() + interval, newrxrtmsg);


    int numChanges = 2;

    RateChangeTimer *rctmsg = new RateChangeTimer();

//    scheduleAt(simTime() + 15, rctmsg);

//    scheduleAt(simTime() + 25, rctmsg->dup());

//    scheduleAt(simTime() + 55, rctmsg->dup());



//    for (int i = 0; i < numChanges - 1; i ++){
//        RateChangeTimer *newrctmsg = new RateChangeTimer();
//        scheduleAt(simTime() + config.RateChangeInterval, newrctmsg);
//    }

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

            vector<int> msgIds1;
            vector<int> msgIds2;
            nodeToMsgsSent[peerIndex] = msgIds1;
            nodeToMsgsAcked[peerIndex] = msgIds2;
            nodeToCurrSeqNum[peerIndex] = 0;

            nodeToNewRateFirstMsgId[peerIndex] = -1;

            nodeToAIMDStatus[peerIndex] = CAN_DECREASE; // can apply MD update to this node iff true
        }
    }
}

void RcNode::registerSignals(){


    if (getIndex() == 0) {

        roptSignal = registerSignal("roptSignal");

        intermNodeSignal = registerSignal("intermNodeSignal");

        uScoreSignal = vector<simsignal_t>(GateSize);

        map<int, int>::iterator it;
        for (it = gateToPeer.begin(); it != gateToPeer.end(); it++){
            int gateIdx = it->first;
            int nodeIdx = it->second;

            char signalName[64];

            sprintf(signalName, "utilScoreNode%d", nodeIdx);
            uScoreSignal[gateIdx] = registerSignal(signalName);
            // set up statistic recording
            char statisticName[64];
            sprintf(statisticName, "uScoreNode%d", nodeIdx);
            cProperty *statisticTemplate = getProperties()->get("statisticTemplate", "uScore");
            getEnvir()->addResultRecorders(this, uScoreSignal[gateIdx], statisticName, statisticTemplate);

        }

    }
    else {
        numRxAsIntermSignal = registerSignal("numRxAsIntermSignal");
        rxRateAsIntermSignal = registerSignal("rxRateAsIntermSignal");

        pRateSignal = vector<simsignal_t>(GateSize);

        map<int, int>::iterator it;
        for (it = gateToPeer.begin(); it != gateToPeer.end(); it++){
            int gateIdx = it->first;
            int nodeIdx = it->second;

            char signalName[64];

            sprintf(signalName, "peerRateToNode%d", nodeIdx);
            pRateSignal[gateIdx] = registerSignal(signalName);
            // set up statistic recording
            char statisticName[64];
            sprintf(statisticName, "rateToNode%d", nodeIdx);
            cProperty *statisticTemplate = getProperties()->get("statisticTemplate", "pRate");
            getEnvir()->addResultRecorders(this, pRateSignal[gateIdx], statisticName, statisticTemplate);

        }

    }

    bdInRateSignal = registerSignal("bdInRateSignal");
    bdOutRateSignal = registerSignal("bdOutRateSignal");

    emit(bdInRateSignal, bdInRate);
    emit(bdOutRateSignal, bdOutRate);

    outQSizeSignal = registerSignal("outQSizeSignal");
    inQSizeSignal = registerSignal("inQSizeSignal");

    numRxSignal = registerSignal("numRxSignal");
    rxRateSignal = registerSignal("rxRateSignal");

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


pair<int,int> RcNode::getMinRxNode(){

    int lastMsgId = getLastMsgIdToCheck();
    int firstMsgId = max(0, lastMsgId - config.TotalMsgsToCheck + 1);
//    int lastMsgId = currMsgId - config.DistFromCurrMsgId;

    EV << "firstMsgId: " << firstMsgId << ", lastMsgId: " << lastMsgId << "\n";

    if (lastMsgId < firstMsgId){
        EV << "lastMsgId < firstMsgId: will not update utility scores \n";
        return pair<int,int>(1,1);
    }

//    int lastMsgId = max(0, currMsgId - config.DistFromCurrMsgId);
    int numMsgsToCheck = lastMsgId - firstMsgId + 1;

    map<int, int> numRx;
    map<int, int>::iterator it;
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
//    map<int, int>::iterator it;
    for (it = numRx.begin(); it != numRx.end(); it++){
        int nodeIdx = it->first;
        int rxNum = it->second;
        if (rxNum < minRxNum){
            minRxNum = rxNum;
            minRxNodeIdx = nodeIdx;
        }
    }
    return pair<int,int>(minRxNodeIdx, minRxNum);
}


void RcNode::updateUScores(){

    int lastMsgId = getLastMsgIdToCheck();
    int firstMsgId = max(0, lastMsgId - config.TotalMsgsToCheck + 1);
//    int lastMsgId = currMsgId - config.DistFromCurrMsgId;

    EV << "firstMsgId: " << firstMsgId << ", lastMsgId: " << lastMsgId << "\n";

    if (lastMsgId < firstMsgId){
        EV << "lastMsgId < firstMsgId: will not update utility scores \n";
        return;
    }

//    int lastMsgId = max(0, currMsgId - config.DistFromCurrMsgId);
    int numMsgsToCheck = lastMsgId - firstMsgId + 1;

    map<int, int> numRx;
    map<int, int>::iterator it;
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
//    map<int, int>::iterator it;
    for (it = numRx.begin(); it != numRx.end(); it++){
        int nodeIdx = it->first;
        int rxNum = it->second;
        if (rxNum < minRxNum){
            minRxNum = rxNum;
            minRxNodeIdx = nodeIdx;
        }
    }

    //////////////////////////////////////////////////////////////////////////

//    std::pair<int,int> minRxPair = getMinRxNode();
//    int minRxNodeIdx = minRxPair.first;
//    int minRxNum = minRxPair.second;

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
//            EV << "receivers for msgId = " << msgId << ": {";
            for (auto dstNodeIdx : msgMap[msgId].receivers){
//                EV << dstNodeIdx << ", ";
                if (dstNodeIdx == minRxNodeIdx){
                    numSentToMinRx[intermNodeIdx] ++;
                }
//                EV << "} \n";
            }
        }
    }

    EV << "numSentToMinRx map: \n";
    printMapIntToInt(numSentToMinRx);
    EV << "numRxAsInterm map: \n";
    printMapIntToInt(numRxAsInterm);

    map<int, float> newUScores;

    for (it = peerToGate.begin(); it != peerToGate.end(); it++){
        int nodeIdx = it->first;
        if (numRxAsInterm[nodeIdx] > 0 && numMsgsToCheck > 0){
//            uScores[nodeIdx] = (float)numSentToMinRx[nodeIdx] / (float)numRxAsInterm[nodeIdx];
            newUScores[nodeIdx] = (float)numSentToMinRx[nodeIdx];
        }
//        uScores[nodeIdx] = numSentToMinRx[nodeIdx] / (numMsgsToCheck * (GateSize - 1));

        uScores[nodeIdx] = 0.2 * uScores[nodeIdx] + 0.8 * newUScores[nodeIdx];
    }
}


void RcNode::updateLeaderSchedule(){

    // assumes that utility scores are updated

    int numMsgs = config.LeaderScheduleSize;
    float prec = config.Prec;
    float epsilon = config.Epsilon;

    map<int, float> uScoresNorm;
    float uScoresSum = 0;
    for (auto const& [key, val] : uScores) uScoresSum += val;
    for (auto const& [key, val] : uScores) uScoresNorm[key] = val / uScoresSum;

    for (int i = 0; i < GateSize; i++){
        emit(uScoreSignal[i], uScoresNorm[gateToPeer[i]]);
    }

    leaderSchedule = getLeaderSchedule(numNodes, numMsgs, uScoresNorm, prec, epsilon, GateSize, gateToPeer);

    // NEW: each follower is guaranteed to serve as an intermediate for >= 1 message
    int gateIdx = 0;
    for (int msgId = numMsgs; msgId < numMsgs + GateSize; msgId++) {
        leaderSchedule[msgId] = gateToPeer[gateIdx];
        gateIdx ++ ;
    }

    numMsgs += GateSize;

    for (int msgId = 0; msgId < numMsgs; msgId++){
        if (leaderSchedule[msgId] == -1) {
            EV_WARN << "ISSUE: intermNodeIdx = -1 for msgId = " << currMsgId << "\n";
        }
    }

    EV << "Leader schedule actual size: " << leaderSchedule.size() << endl;
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
    numRxTemp ++ ;

    emit(numRxSignal, 1);

    // DONE: find connection between gate index and module index
    if (peerIdx == config.LEADER_IDX) {

        numRxAsIntermTemp ++ ;

        // Node serves as an intermediate node. Need to broadcast the message to all nodes except for the leader.
        MaxMinMsgInfo newMMMsgInfo;
        newMMMsgInfo.msgId = msg->getMsgId();
        newMMMsgInfo.numAcksExp = GateSize - 1;
        newMMMsgInfo.timeOut = false;

        msgMap[msg->getMsgId()] = newMMMsgInfo;

        EV << "Node " << getIndex() << " serving as intermediate for msgId = " << newMMMsgInfo.msgId << "\n";
        EV << "rates map: \n";
        printMapIntToFloat(rates);
        EV << "tokenBuckets map: \n";
        printMapIntToFloat(tokenBuckets);

//        vector<int> rx;
//        map<int, int>::reverse_iterator it;
//        map<int, int>::iterator it;

//        vector<int, int>::iterator it;

        vector<int> gates = vector<int>(GateSize);
        for (int i = 0; i < GateSize; i++){
            gates[i] = i;
        }
        random_shuffle ( gates.begin(), gates.end() );  // in place no extra

        EV << "random shuffled gates:" << endl;
        for (std::vector<int>::iterator it=gates.begin(); it!=gates.end(); ++it){
           EV << ' ' << *it;
        }
        EV << endl;

        for (int i = 0; i < GateSize; i++){
            int gateIdx = gates[i];
            int dstIdx = gateToPeer[gateIdx];
            if (dstIdx != config.LEADER_IDX) {
//                EV << "Node " << getIndex() << " will consider forwarding msgID " << msg->getMsgId() << " to node " << dstIdx << "\n";
                if (tokenBuckets[dstIdx] >= 1){
                    // TODO: augment newMMMsgInfo with set of expected receivers
                    EV << "Node " << getIndex() << " will forward mmmsg with msgID " << msg->getMsgId() << " to node " << dstIdx << "\n";
//                    rx.push_back(dstIdx);
                    tokenBuckets[dstIdx] -- ;
                    MaxMinMsg *mmmsgCopy = msg->dup();
                    mmmsgCopy->setDestination(dstIdx);
                    handleOutMsg(mmmsgCopy);

                    nodeToMsgsSent[dstIdx].push_back(mmmsgCopy->getMsgId());

                    pair<int,int> key = pair<int,int>(dstIdx, mmmsgCopy->getMsgId());
                    intermSeqNumMap[key] = nodeToCurrSeqNum[dstIdx];
                    nodeToCurrSeqNum[dstIdx] ++ ;

                    // first message sent after applying MD rate update
                    if (nodeToNewRateFirstMsgId[dstIdx] == -1){
                        nodeToNewRateFirstMsgId[dstIdx] = mmmsgCopy->getMsgId();
                    }

                }
                tokenBuckets[dstIdx] += rates[dstIdx]; // should always add rate to the token bucket
            }
        }

//        intermSeqNum ++ ;
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
    EV << "Node " << getIndex() << " received ACK message for msgID " << msgId << " from node " << peerIdx << "\n";

    if (getIndex() == config.LEADER_IDX){

        if (msgMap.at(msgId).timeOut == false){
            msgMap[msgId].receivers.insert(peerIdx);
        }

//        pair<int,int> minRxPair = getMinRxNode();
//        int minRxNodeIdx = minRxPair.first;
//        int minRxNum = minRxPair.second;
//        EV << "MinRxNode nodeIdx: " << minRxNodeIdx << " received " << minRxNum << " messages.\n";
//
//        if (minRxNodeIdx == peerIdx) {
//            if (msgMap.at(msgId).timeOut == false){
//                // increase utility score of this intermediate node
//                int intermNodeIdx = msgMap[msgId].intermediate;
//
//                uScores[intermNodeIdx] += config.UtilAddVal;
////                uScores[intermNodeIdx] += config.UtilAddVal / uScores[intermNodeIdx];
//
//                msgMap[msgId].receivers.insert(peerIdx);
//            }
//        }
    }
    else {
        if (!nodeToMsgsAcked[peerIdx].empty()) {
            int lastMsgIdAcked = nodeToMsgsAcked[peerIdx].back();

            pair<int,int> lastMsgKey = pair<int,int>(peerIdx, lastMsgIdAcked);
            pair<int,int> thisMsgKey = pair<int,int>(peerIdx, msgId);
            int lastIntermSeqNum = intermSeqNumMap[lastMsgKey];
            int thisIntermSeqNum = intermSeqNumMap[thisMsgKey];

            if (msgId > nodeToNewRateFirstMsgId[peerIdx]){
                EV_WARN << " Node " << getIndex() << " CAN DECREASE rate to node " << peerIdx << " since msgId > " << nodeToNewRateFirstMsgId[peerIdx] << "\n";
                nodeToAIMDStatus[peerIdx] = CAN_DECREASE;
            }

            // check if there is a gap in acknowledged messages // DONE: fix bug here
            int updateType = NO_UPDATE;
            if (thisIntermSeqNum > lastIntermSeqNum + 1){
                EV_WARN << "ACK TIMEOUT: Node " << getIndex() << " received non-consecutive ACK from node " << peerIdx << " for msgID " << msgId << "\n";
                // TODO: reduce rate to this peer (if this is not the leader?)
                if (nodeToAIMDStatus[peerIdx] == CAN_DECREASE) {
                    EV_WARN << "Mult Decrease: Node " << getIndex() << " will apply MD rate decrease to node " << peerIdx << "\n";
                    updateType = DECREASE;
                    nodeToAIMDStatus[peerIdx] = NO_DECREASE;

//                    AIMDTimer *aimdtmsg = new AIMDTimer();
//                    aimdtmsg->setPeerIdx(peerIdx);
//                    aimdtmsg->setMsgId(msgId);
//                    scheduleAt(simTime() + aimdConfig.MDInterval, aimdtmsg);
                }
            }
            else {
                // TODO: should we apply Add Increase every time we receive ACKS for consecutive msgIds?
                updateType = INCREASE;
            }
            applyAIMDUpdate(peerIdx, updateType);
        }
    }

//    // TODO: should we check for any timeout before adding (msgId, peerIdx) in our records?
//    msgMap[msgId].receivers.insert(peerIdx);

    nodeToMsgsAcked[peerIdx].push_back(msgId);
    // TODO: this is only used by follower nodes for AIMD purposed.
    // Should it happen only if timeout = false?

}


void RcNode::applyAIMDUpdate(int peerIdx, int updateType){
    // DONE: apply AIMD update
    if (updateType == NO_UPDATE) return;

    float currRate = rates[peerIdx];
    EV << "AIMDUpdate: Node " << getIndex() << " will update node " << peerIdx << "'s current rate = " << currRate << "\n";
    float newRate = currRate;

    if (updateType == DECREASE) {
        newRate = max(aimdConfig.MinVal, aimdConfig.MultFactor * currRate);
        nodeToNewRateFirstMsgId[peerIdx] = -1;
    }
    else if (updateType == INCREASE) {
        newRate = min(aimdConfig.MaxVal, currRate + aimdConfig.AddVal * (aimdConfig.MinVal / currRate));
//        newRate = min(aimdConfig.MaxVal, currRate + aimdConfig.AddVal);
    }

    EV << "AIMDUpdate: Node " << getIndex() << " updated node " << peerIdx << "'s rate. newRate = " << newRate << "\n";

    rates[peerIdx] = newRate;

    map<int, int>::iterator it;
//    float rateValues[2]; // needs to have type = float!!
//    int i = 0;
    for (it = peerToGate.begin(); it != peerToGate.end(); it++){
        int nodeIdx = it->first;
//        if (dstIdx != config.LEADER_IDX) {
//            EV << "AIMDUpdate: node " << getIndex() << " emitting signals. dstIdx = " << dstIdx << " and rates[dstIdx] = " << rates[dstIdx] << "\n";
//            rateValues[i] = rates[dstIdx];
//            EV << "AIMDUpdate: node " << getIndex() << " emitting signals. i = " << i << " and rateValues[i] =  " << rateValues[i] << "\n";
//            i++ ;
//        }
        emit(pRateSignal[peerToGate[nodeIdx]], rates[nodeIdx]);
    }

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

//    int localMsgId = currMsgId % config.LeaderScheduleSize;
    int localMsgId = currMsgId % (config.LeaderScheduleSize + GateSize);


    if (localMsgId == 0) {
        EV << "Updating utility scores \n";
        updateUScores();
        EV << "Updating leader schedule \n";
        updateLeaderSchedule();
    }

//    emit(uScore1Signal, uScores[1]);
//    emit(uScore2Signal, uScores[2]);
//    emit(uScore3Signal, uScores[3]);

    int intermNodeIdx = leaderSchedule[localMsgId];

//    int intermNodeIdx = 1; // TODO: comment out this line

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
    if (bdOutRate > 0) {
        float duration = config.MaxMinMsgSize / bdOutRate;
        newstmsg->setMsgTimer(stmsg->getMsgTimer() + duration);
        scheduleAt(newstmsg->getMsgTimer(), newstmsg);
    }
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

void RcNode::handleRxRateTimer(RxRateTimer *rxrtmsg){

    emit(bdInRateSignal, bdInRate);
    emit(bdOutRateSignal, bdOutRate);

    float interval = config.RxRateSignalInterval;

    float rateTemp = (float) numRxTemp / interval ;
    float rateAsIntermTemp = (float) numRxAsIntermTemp / interval ;

//    EV << "rateTemp = " << rateTemp ;

    emit(rxRateSignal, rateTemp);
    emit(rxRateAsIntermSignal, numRxAsIntermTemp);

    emit(roptSignal, ropt); // TODO: ropt does not change when bdInRate or bdOutRate change. Fix it.

    RxRateTimer *newrxrtmsg = new RxRateTimer();
    scheduleAt(simTime() + interval, newrxrtmsg);

    numRxTemp = 0;
    numRxAsIntermTemp = 0;


}

void RcNode::handleRateChangeTimer(RateChangeTimer *rctmsg){

    emit(bdInRateSignal, bdInRate);
    emit(bdOutRateSignal, bdOutRate);

    if (getIndex() == 1){
        bdOutRate *= 0;
    }

    emit(bdInRateSignal, bdInRate);
    emit(bdOutRateSignal, bdOutRate);
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
        else if(instanceof<RateChangeTimer>(msg)){
            RateChangeTimer *rctmsg = check_and_cast<RateChangeTimer *>(msg);
            handleRateChangeTimer(rctmsg);
        }
        else if(instanceof<RxRateTimer>(msg)){
            RxRateTimer *rxrtmsg = check_and_cast<RxRateTimer *>(msg);
            handleRxRateTimer(rxrtmsg);
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
//    pair<int,int> pair = pair<int,int>(mmmsg->getMsgId(), mmmsg->getDestination());

    if (outQueue.size() < config.OutQueueMaxSize){
        outMsgMap[msgId] = msg; // todo: should I duplicate the message here?
        outQueue.push(msgId);
    }
    else {
        EV_WARN << "Node " << getIndex() << ": OUT Queue is full, dropped message " << msg << "to dstIdx " << msg->getDestination() << "\n";
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
//    pair<int,int> pair = outQueue.front();
    int outMsgId = outQueue.front();
//    int outMsgDstIdx = pair.second;
    OutMsgTimer *outtmsg = new OutMsgTimer();
    outtmsg->setMsgId(outMsgId);
//    outtmsg->setDestination(outMsgDstIdx);

    if (bdOutRate > 0) {
        float duration = config.MaxMinMsgSize / bdOutRate;
        scheduleAt(simTime() + duration, outtmsg);
    }
    // if bdInRate = 0, pop and drop the message
    outQueue.pop();
}

// assumes that inQueue is not empty
void RcNode::processNextInMsg(){
//    pair<int,int> pair = inQueue.front();
//    int inMsgType = pair.first;
    int inMsgId = inQueue.front();
    cMessage *msg = inMsgMap[inMsgId];
    InMsgTimer *intmsg = new InMsgTimer();
//    intmsg->setMsgType(inMsgType);
    intmsg->setMsgId(inMsgId);
    float duration;

    if (bdInRate > 0) {
        if (instanceof<MaxMinMsg>(msg)) {
    //        EV << "Node " << getIndex() << " processing maxmin message " << msg << "\n";
            duration = config.MaxMinMsgSize / bdInRate;
        }
        scheduleAt(simTime() + duration, intmsg);
    }
    // if bdInRate = 0, pop and drop the message
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


