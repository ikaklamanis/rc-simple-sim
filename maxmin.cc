//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

#include <vector>
#include <map>

using namespace omnetpp;

// Include a generated file: the header file created from tictoc13.msg.
// It contains the definition of the TictocMsg10 class, derived from
// cMessage.
#include "maxmin_m.h"


// for dynamic type checking
template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
   return dynamic_cast<const Base*>(ptr) != nullptr;
}

struct Config {
    const int LEADER_IDX = 0;
};

struct MsgInfo {
    int msgId;
    int intermediate;
    std::set<int> receivers;
    int numAcksExp;
};



class RcNode : public cSimpleModule {
    private:
        float bdInRate, bdOutRate;
        int GateSize;
        std::map<int, MsgInfo> msgMap;
        std::map<int, int> peerToGate, gateToPeer;
        Config config;

    protected:
        virtual MaxMinMsg *generateMessage(int dest);
        virtual void forwardMessage(cMessage *msg, int dstIdx);
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;

        virtual void fillPeerGateInfo();
        virtual void handleMaxMinMessage(MaxMinMsg *msg);
        virtual void handleACKMessage(ACKMsg *msg);
        virtual void broadcastMessage(MaxMinMsg *msg, std::vector<int> rx);
        virtual void sendACK(MaxMinMsg *msg, int dstIdx);

    public:
        RcNode() {}
        RcNode(float inRate, float outRate){
            bdInRate = inRate;
            bdOutRate = outRate;
        }

        float getBdInRate() {return bdInRate;}
        void setBdInRate(float newBdInRate) {bdInRate = newBdInRate;}
        float getBdOutRate() {return bdOutRate;}
        void setBdOutRate(float newBdOutRate) {bdOutRate = newBdOutRate;}
};


Define_Module(RcNode);


void RcNode::initialize(){

    fillPeerGateInfo();
    GateSize = gateSize("gate");

    // Module 0 sends the first message
    if (getIndex() == 0) {

        int numMsgs = 10;

        for (int msgId = 0; msgId < numMsgs; msgId++) {
            MaxMinMsg *msg = generateMessage(msgId);

            MsgInfo newMsgInfo;
            newMsgInfo.msgId = msgId;
            newMsgInfo.numAcksExp = GateSize;
            msgMap.insert(std::pair<int, MsgInfo>(msgId, newMsgInfo));

            scheduleAt(msgId, msg);
        }
    }
}

void RcNode::fillPeerGateInfo(){
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

void RcNode::handleMessage(cMessage *msg){

    if (msg->isSelfMessage()) {
        MaxMinMsg *mmmsg = check_and_cast<MaxMinMsg *>(msg);
//        MaxMinMsg *copy = mmmsg->dup();
//        forwardMessage(copy, 1);
        std::vector<int> rx;

        // choose intermediate nodes in a round-robin fashion
        // TODO: choose intermediate nodes based on utility score ranking
        int intermNodeIdx = gateToPeer[mmmsg->getMsgId() % GateSize];
        rx.push_back(intermNodeIdx);
        msgMap.at(mmmsg->getMsgId()).intermediate = intermNodeIdx;

        broadcastMessage(mmmsg, rx);
        return;
    }

    if(instanceof<MaxMinMsg>(msg)) {
//        EV << "msg " << msg << " is of type MaxMinMsg \n";
        MaxMinMsg *mmmsg = check_and_cast<MaxMinMsg *>(msg);
        handleMaxMinMessage(mmmsg);
    }
    else if(instanceof<ACKMsg>(msg)) {
//        EV << "msg " << msg << " is of type ACKMsg \n";
        ACKMsg *ackmsg = check_and_cast<ACKMsg *>(msg);
        handleACKMessage(ackmsg);
    }

}

// broadcast message to all receivers in rx array. If rx is empty, broadcast to all peers
void RcNode::broadcastMessage(MaxMinMsg *msg, std::vector<int> rx){

    if (rx.size() == 0) {
        std::map<int, int>::iterator it;
        for (it = peerToGate.begin(); it != peerToGate.end(); it++){
            MaxMinMsg *copy = msg->dup();
            int dstIdx = it->first;
//            EV << "Node " << getIndex() << " forwarding message " << msg << " on gate[" << peerToGate[it->first] << "]\n";
            forwardMessage(copy, dstIdx);
        }

    } else {
        for (int dstIdx : rx){
            MaxMinMsg *copy = msg->dup();
            forwardMessage(copy, dstIdx);
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


