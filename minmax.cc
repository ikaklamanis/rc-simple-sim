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
#include "minmax_m.h"


// for dynamic type checking
template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
   return dynamic_cast<const Base*>(ptr) != nullptr;
}



class RcNode : public cSimpleModule {
    private:
        float bdInRate, bdOutRate;

    protected:
        virtual MinMaxMsg *generateMessage(int dest);
        virtual void forwardMessage(MinMaxMsg *msg, int dst);
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void handleMinMaxMessage(MinMaxMsg *msg);
        virtual void handleACKMessage(ACKMsg *msg);
        virtual void broadcastMessage(MinMaxMsg *msg, std::vector<int> rx);
        virtual void sendACK(MinMaxMsg *msg, int dst);

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

    // Module 0 sends the first message
    if (getIndex() == 0) {

//        bool sendMsgs = true;
        int numMsgs = 10;

        for (int idx=0; idx<numMsgs; idx++) {
            MinMaxMsg *msg = generateMessage(idx);
            scheduleAt(idx, msg);
        }

    }
}

void RcNode::handleMinMaxMessage(MinMaxMsg *msg){

    int sender = msg->getSenderModule()->getIndex();
    // Message arrived.
    EV << "Node " << getIndex() << " received message " << msg << " after " << msg->getHopCount() << " hops.\n";

    // send ACK to sender
    int dst = msg->getArrivalGate()->getIndex();
    sendACK(msg, dst);

    // TODO: find connection between gate index and module index
    if (sender == 0) {
        // We need to broadcast the message to all nodes except for the leader.
        int n = gateSize("gate");
        std::vector<int> rx;
        for (int i=1; i<n; i++) {
            rx.push_back(i);
        }
        broadcastMessage(msg, rx);
    }
    else {
        bubble("RECEIVED mmmsg as dst node!");
        // TODO: send ACK to leader
        sendACK(msg, 0);
        delete msg;
    }
}

void RcNode::handleACKMessage(ACKMsg *msg){

    int sender = msg->getSenderModule()->getIndex();
    int msgId = msg->getMsgId();
    // Message arrived.
    EV << "Node " << getIndex() << " received ACK message for msgID " << msgId << " from node " << sender << "\n";
    // TODO: do stuff with ACK

}

void RcNode::handleMessage(cMessage *msg){

    if (msg->isSelfMessage()) {
        MinMaxMsg *mmmsg = check_and_cast<MinMaxMsg *>(msg);
//        MinMaxMsg *copy = mmmsg->dup();
//        forwardMessage(copy, 1);
        std::vector<int> rx;
        broadcastMessage(mmmsg, rx);
        return;
    }

    if(instanceof<MinMaxMsg>(msg)) {
//        EV << "msg " << msg << " is of type MinMaxMsg \n";
        MinMaxMsg *mmmsg = check_and_cast<MinMaxMsg *>(msg);
        handleMinMaxMessage(mmmsg);
    }
    else if(instanceof<ACKMsg>(msg)) {
//        EV << "msg " << msg << " is of type ACKMsg \n";
        ACKMsg *ackmsg = check_and_cast<ACKMsg *>(msg);
        handleACKMessage(ackmsg);
    }

}

MinMaxMsg *RcNode::generateMessage(int msgId){
    // Produce source and destination addresses.
    int src = getIndex();  // our module index

    char msgname[20];
    sprintf(msgname, " max-min-msg-%d", msgId);

    // Create message object and set source and destination field.
    MinMaxMsg *msg = new MinMaxMsg(msgname);
    msg->setSource(src);
//    msg->setDestination(dest);
    return msg;
}

void RcNode::forwardMessage(MinMaxMsg *msg, int dst){

    // Increment hop count.
    msg->setHopCount(msg->getHopCount()+1);

    // Same routing as before: random gate.
    int n = gateSize("gate");
//    int k = intuniform(0, n-1);
    int k = dst;

    EV << "Node " << getIndex() << " sending message " << msg << " on gate[" << k << "]\n";
    send(msg, "gate$o", k);
}

void RcNode::broadcastMessage(MinMaxMsg *msg, std::vector<int> rx){

    if (rx.size() == 0) {
        int n = gateSize("gate");
        for (int k=0; k<n; k++){
            MinMaxMsg *copy = msg->dup();
            forwardMessage(copy, k);
        }
    } else {
        for (int gateid : rx){
            MinMaxMsg *copy = msg->dup();
            forwardMessage(copy, gateid);
        }
    }

}

void RcNode::sendACK(MinMaxMsg *msg, int dst){
    int msgId = msg->getMsgId();
    char msgname[20];
    sprintf(msgname, " ACK-max-min-msg-%d", msgId);

    ACKMsg *ack = new ACKMsg(msgname);
    int src = getIndex();  // our module index
    ack->setSource(src);
    ack->setMsgId(msgId);
    ack->setDestination(dst);

    EV << "Node " << getIndex() << " sending ACK for msgID " << msgId << " on gate[" << dst << "]\n";
    send(ack, "gate$o", dst);
}


