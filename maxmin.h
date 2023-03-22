#ifndef __MAXMIN_H
#define __MAXMIN_H

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

#include <vector>
#include <map>
#include <algorithm> // for sort function
#include <queue>

using namespace omnetpp;

// Include a generated file: the header file created from maxmin.msg.
// It contains the definition of the MaxMinMgs class, derived from
// cMessage.
#include "maxmin_m.h"
#include "utils.h"


class RcNode : public cSimpleModule {
    private:
        float bdInRate = 10;
        float bdOutRate = 10;
        int GateSize;
        std::map<int, MsgInfo> msgMap;
        std::map<int, int> peerToGate, gateToPeer;
        Config config;
        // only for leader
        std::map<int, float> uScores;
        int leastRxNode;
        int currMsgId = 0;

        std::queue<std::pair<int,int>> outQueue;
        bool outQueueBusy = false;

        std::queue<int> inQueue;
        bool inQueueBusy = false;

        std::map<int, MaxMinMsg*> inMsgMap;
        std::map<std::pair<int,int>, MaxMinMsg*> outMsgMap;

    protected:
        virtual MaxMinMsg* generateMessage(int dest);
        virtual void forwardMessage(cMessage *msg, int dstIdx);
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;

        virtual void fillBookkeepingInfo();
        virtual void handleSelfTimerMessage(SelfTimer *tmsg);
        virtual void handleACKTimeOutMessage(AckTimeOut *atmsg);
        virtual void handleInMessageTimer(InMsgTimer *intmsg);
        virtual void handleOutMessageTimer(OutMsgTimer *outtmsg);


        virtual void handleMaxMinMessage(MaxMinMsg *mmmsg);
        virtual void handleACKMessage(ACKMsg *amsg);
        virtual void broadcastMessage(MaxMinMsg *msg, std::vector<int> rx);
        virtual void sendACK(MaxMinMsg *msg, int dstIdx);

        virtual void updateUScores();
        virtual std::pair<int,int> getMinRxNode(std::map<int, int> nodeToNumRx, int minRxNum);

        virtual void handleOutMsg(MaxMinMsg *msg);
        virtual void processNextOutMsg();
        virtual void processNextInMsg();

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



#endif
