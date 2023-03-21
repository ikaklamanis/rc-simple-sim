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

    fillBookkeepingInfo();
    GateSize = gateSize("gate");

    // Module 0 sends the first message
    if (getIndex() == 0) {

        SelfTimer *stmsg = new SelfTimer();

        float duration = config.MsgSize / bdInRate;
        stmsg->setMsgTimer(stmsg->getMsgTimer() + duration);

        // Todo: first message doesn't need to go through both pipes (self msgtimer and out msgtimer)
        scheduleAt(stmsg->getMsgTimer(), stmsg);

    }
}

void RcNode::fillBookkeepingInfo(){
    for (cModule::GateIterator i(this); !i.end(); i++) {
        cGate *gate = *i;
        int gateIndex = gate->getIndex();
        int peerIndex = gate->getPathEndGate()->getOwnerModule()->getIndex();
        if (peerIndex != getIndex()){
            peerToGate[peerIndex] = gateIndex;
            gateToPeer[gateIndex] = peerIndex;
//            EV << "node " << getIndex() << " - peer: " << peerIndex << " - gate: " << gateIndex << "\n";
        }
    }

}

std::pair<int,int> RcNode::getMinRxNode(std::map<int, int> nodeToNumRx, int minRxNum){
    return std::pair<int,int>(1,1);
}

void RcNode::updateUScores(){

}



void RcNode::handleMaxMinMessage(MaxMinMsg *msg){

    int peerGate = msg->getArrivalGate()->getIndex();
    int peerIdx = gateToPeer[peerGate];
    // Message arrived.
//    EV << "Node " << getIndex() << " received message " << msg << "\n";

    // send ACK to sender
    sendACK(msg, peerIdx);

    // DONE: find connection between gate index and module index
    if (peerIdx == config.LEADER_IDX) {
        // Node serves as an intermediate node. Need to broadcast the message to all nodes except for the leader.
        MsgInfo newMsgInfo;
        newMsgInfo.msgId = msg->getMsgId();
        newMsgInfo.numAcksExp = GateSize - 1;
        msgMap.insert(std::pair<int, MsgInfo>(msg->getMsgId(), newMsgInfo));

        std::vector<int> rx;
        std::map<int, int>::iterator it;
        for (it = peerToGate.begin(); it != peerToGate.end(); it++){
            int dstIdx = it->first;
            if (dstIdx != config.LEADER_IDX) {
//                EV << "Node " << getIndex() << " will forward msgID " << msg->getMsgId() << " to node " << dstIdx << "\n";
                rx.push_back(dstIdx);
            }
        }
        broadcastMessage(msg, rx);
    }
    else {
        bubble("RECEIVED mmmsg as dst node!");
        // TODO: send ACK to leader
        sendACK(msg, config.LEADER_IDX);
        delete msg;
    }
}

void RcNode::handleACKMessage(ACKMsg *msg){

    int peerGate = msg->getArrivalGate()->getIndex();
    int peerIdx = gateToPeer[peerGate];
    int msgId = msg->getMsgId();
    // Message arrived.
//    EV << "Node " << getIndex() << " received ACK message for msgID " << msgId << " from node " << peerIdx << "\n";

    // TODO: do stuff with ACK

    // if msgId exists as key in msgMap
    if (msgMap.count(msg->getMsgId())){
        msgMap[msg->getMsgId()].receivers.insert(peerIdx);
        // check if have received all ACKS
        if (msgMap[msg->getMsgId()].receivers.size() == msgMap[msg->getMsgId()].numAcksExp){
            EV << "Node " << getIndex() << " received all ACKS for msgID " << msgId << "\n";
        }
    }

}

void RcNode::handleSelfTimerMessage(SelfTimer *stmsg){
    MaxMinMsg *mmmsg = generateMessage(currMsgId);

    MsgInfo newMsgInfo;
    newMsgInfo.msgId = currMsgId;
    newMsgInfo.numAcksExp = GateSize;
    msgMap.insert(std::pair<int, MsgInfo>(currMsgId, newMsgInfo));

//    std::pair<int,int> minRx = getMinRxNode();
//    int minRxNodeIdx = minRx.first;
//    int minRxNum = minRx.second;

//    updateUScores();
    int intermediateNodeIdx = 1;

    currMsgId++;

    std::vector<int> rx;
    rx.push_back(intermediateNodeIdx);
    broadcastMessage(mmmsg, rx);

    SelfTimer *newstmsg = new SelfTimer();

    float duration = config.MsgSize / bdInRate;
    newstmsg->setMsgTimer(stmsg->getMsgTimer() + duration);

    scheduleAt(newstmsg->getMsgTimer(), newstmsg);

}

void RcNode::handleMessage(cMessage *msg){

    if (msg->isSelfMessage()) {
        if(instanceof<SelfTimer>(msg)){
            SelfTimer *stmsg = check_and_cast<SelfTimer *>(msg);
            handleSelfTimerMessage(stmsg);
        }
        else if(instanceof<InMsgTimer>(msg)){
            InMsgTimer *intmsg = check_and_cast<InMsgTimer *>(msg);
            int inMsgId = intmsg->getMsgId();
            MaxMinMsg *inmsg = inMsgMap.at(inMsgId);

            handleMaxMinMessage(inmsg);

            inQueueBusy = false;
            if (!inQueueBusy){
                if (!inQueue.empty()){
                    inQueueBusy = true;
                    processNextInMsg();
                }
            }

        }
        else if(instanceof<OutMsgTimer>(msg)){
            OutMsgTimer *outtmsg = check_and_cast<OutMsgTimer *>(msg);
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
    }

    if(instanceof<MaxMinMsg>(msg)) {
//        EV << "msg " << msg << " is of type MaxMinMsg \n";
        MaxMinMsg *mmmsg = check_and_cast<MaxMinMsg *>(msg);

        inMsgMap[mmmsg->getMsgId()] = mmmsg->dup(); // todo: should I duplicate the message here?
        inQueue.push(mmmsg->getMsgId());

        if (!inQueueBusy){
            if (!inQueue.empty()){
                inQueueBusy = true;
                processNextInMsg();
            }
        }

    }
    else if(instanceof<ACKMsg>(msg)) {
//        EV << "msg " << msg << " is of type ACKMsg \n";
        ACKMsg *ackmsg = check_and_cast<ACKMsg *>(msg);
        handleACKMessage(ackmsg);
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
    int inMsgId = inQueue.front();
    InMsgTimer *intmsg = new InMsgTimer();
    intmsg->setMsgId(inMsgId);
    float duration = config.MsgSize / bdInRate;
    scheduleAt(simTime() + duration, intmsg);
    inQueue.pop();

}

// broadcast message to all receivers in rx array. If rx is empty, broadcast to all peers
void RcNode::broadcastMessage(MaxMinMsg *msg, std::vector<int> rx){

    if (rx.size() == 0) {
        std::map<int, int>::iterator it;
        for (it = peerToGate.begin(); it != peerToGate.end(); it++){
            MaxMinMsg *copy = msg->dup();
            int dstIdx = it->first;
//            EV << "Node " << getIndex() << " forwarding message " << msg << " on gate[" << peerToGate[it->first] << "]\n";
            copy->setDestination(dstIdx);
            handleOutMsg(copy);
        }

    } else {
        for (int dstIdx : rx){
            MaxMinMsg *copy = msg->dup();
            copy->setDestination(dstIdx);
            handleOutMsg(copy);
        }
    }

}

void RcNode::sendACK(MaxMinMsg *msg, int dstIdx){
    int msgId = msg->getMsgId();
    char msgname[20];
    sprintf(msgname, " ACK-max-min-msg-%d", msgId);

    ACKMsg *ack = new ACKMsg(msgname);
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


