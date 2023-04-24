#ifndef __MAXMIN_H
#define __MAXMIN_H

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

#include <vector>
#include <map>
#include <algorithm> // for sort function
#include <queue>
#include <deque>
#include <cmath>
#include <cstdlib>
#include <numeric>

using namespace omnetpp;
using namespace std;

// Include a generated file: the header file created from maxmin.msg.
// It contains the definition of the MaxMinMgs class, derived from
// cMessage.
#include "maxmin_m.h"
#include "utils.h"
#include "schedule.h"


class RcNode : public cSimpleModule {
    private:
        simsignal_t bdInRateSignal;
        simsignal_t bdOutRateSignal;
        simsignal_t numRxSignal;
        simsignal_t rxRateSignal;
        simsignal_t numRxAsIntermSignal;
        simsignal_t rxRateAsIntermSignal;
        simsignal_t intermNodeSignal;
        simsignal_t numDrOutQSignal;
        simsignal_t numDrInQSignal;
        simsignal_t outQSizeSignal;
        simsignal_t inQSizeSignal;

        vector<simsignal_t> uScoreSignal;
        vector<simsignal_t> pRateSignal;

        long numReceived = 0;
        long numReceivedAsInterm = 0;

        long numRxTemp = 0;
        long numRxAsIntermTemp = 0;

        float bdInRate;
        float bdOutRate;

        int GateSize;
//        int numNodes;

        map<int, MaxMinMsgInfo> msgMap;
        map<int, int> peerToGate, gateToPeer;
        Config config;
        AIMDConfig aimdConfig;

        map<int, vector<int>> nodeToMsgsSent;
        // key = (peerIdx, msgId), value = seqNum
        map<int, int> nodeToCurrSeqNum;
        map<pair<int,int>, int> intermSeqNumMap;
        map<int,int> nodeToNewRateFirstMsgId;

        map<int, vector<int>> nodeToMsgsAcked;
        map<int, int> nodeToAIMDStatus;

        queue<int> outQueue;
        bool outQueueBusy = false;

        queue<int> inQueue;
        bool inQueueBusy = false;

        // msgType: MaxMinMsg = 0, ProbeMsg = 1

        map<int, cMessage*> inMsgMap;
        map<int, RCMessage*> outMsgMap;

        // only for leader?
        map<int, float> uScores;
//        int minRxNode;
        int currMsgId = 0;
        map<int,int> leaderSchedule; // TODO: define length

        // only for followers?
        map<int,float> rates;
        map<int,float> tokenBuckets;

//        int intermSeqNum;


    protected:
        virtual MaxMinMsg* generateMessage(int dest);
        virtual void forwardMessage(cMessage *msg, int dstIdx);
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;

        virtual void fillBookkeepingInfo();
        virtual void registerSignals();

        virtual void handleSelfTimerMessage(SelfTimer *tmsg);
        virtual void handleACKTimeOutMessage(AckTimeOut *atmsg);
        virtual void handleInMessageTimer(InMsgTimer *intmsg);
        virtual void handleOutMessageTimer(OutMsgTimer *outtmsg);
        virtual void handleRxRateTimer(RxRateTimer *rxrtmsg);
        virtual void handleRateChangeTimer(RateChangeTimer *rctmsg);

        virtual void handleMaxMinMessage(MaxMinMsg *mmmsg);
        virtual void handleMaxMinACK(MaxMinACK *ammmsg);
//        virtual void sendMessage(cMessage *msg, vector<int> rx);
        virtual void sendACK(MaxMinMsg *msg, int dstIdx);

        virtual void handleOutMsg(RCMessage *msg);
        virtual void processNextOutMsg();
        virtual void processNextInMsg();

        // only for leader?
        virtual int getLastMsgIdToCheck();
        virtual pair<int,int> getMinRxNode();
        virtual void updateLeaderSchedule();

        // only for followers?
        virtual void handleAIMDTimer(AIMDTimer *aimdtmsg);

        virtual void applyAIMDUpdate(int peerIdx, int updateType);



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
