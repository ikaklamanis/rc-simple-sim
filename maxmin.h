#ifndef __MAXMIN_H
#define __MAXMIN_H

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

#include <vector>
#include <map>
#include <algorithm> // for sort function
#include <queue>
#include <cmath>
#include <cstdlib>

using namespace omnetpp;

// Include a generated file: the header file created from maxmin.msg.
// It contains the definition of the MaxMinMgs class, derived from
// cMessage.
#include "maxmin_m.h"
#include "utils.h"


class RcNode : public cSimpleModule {
    private:
        long numReceived = 0;
        long numRxAsInterm = 0;

        float bdInRate;
        float bdOutRate;

        int GateSize;
        std::map<int, MsgInfo> msgMap;
        std::map<int, int> peerToGate, gateToPeer;
        Config config;
        AIMDConfig aimdConfig;

        std::queue<std::pair<int,int>> outQueue;
        bool outQueueBusy = false;

        std::queue<std::pair<int,int>> inQueue;
        bool inQueueBusy = false;

        // pair: (msgType, msgId)
        // msgType: MaxMinMsg = 0, ProbeMsg = 1
        std::map<int, MaxMinMsg*> inMaxMinMsgMap;
        std::map<int, ProbeMsg*> inProbeMsgMap;
        // pair: (msgId, dstIdx)
        std::map<std::pair<int,int>, MaxMinMsg*> outMsgMap;

        // only for leader?
        std::map<int, float> uScores;
        int leastRxNode;
        int currMsgId = 0;

        // only for followers?
        std::map<int,float> dstNodesRates;
        std::map<int,int> distFromNextMsg;
        std::map<int,ProbeMsgInfo> probeMsgMap;


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
        virtual void handleMaxMinACK(MaxMinACK *ammmsg);
        virtual void sendMessage(MaxMinMsg *msg, std::vector<int> rx);
        virtual void sendACK(MaxMinMsg *msg, int dstIdx);

        virtual void handleOutMsg(MaxMinMsg *msg);
        virtual void processNextOutMsg();
        virtual void processNextInMsg();

        // only for leader?
        virtual void updateUScores();
        virtual int getLastMsgIdToCheck();
        virtual std::pair<int,int> getMinRxNode(std::map<int, int> nodeToNumRx, int minRxNum);

        // only for followers?
        virtual void handleProbeMsg(ProbeMsg *pmsg);
        virtual void handleProbeMsgACK(ProbeACK *apmsg);
        virtual void handleProbeSelfTimer(ProbeSelfTimer *pstmsg);
        virtual void handleProbeAckTimeOut(ProbeAckTimeOut *aptmsg);



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
