#ifndef __UTILS_H
#define __UTILS_H

#include <omnetpp.h>
#include <vector>
#include <map>
#include <algorithm> // for sort function
#include "maxmin_m.h"

using namespace omnetpp;


const int numNodes = 4;
// the first node (index 0) is the leader
const int ingress[numNodes] = {100, 1000, 1000, 1000};
const int egress[numNodes] =  {1000, 1000, 1000, 1000};

//const int ackTimeOuts[numNodes] = {0.2, 0.2, 0.2, 0.2};

// For reference: propagation delay is 10ms = 0.01s
struct Config {
    const int LEADER_IDX = 0;
//    const int MAXMIN_MSG_TYPE = 0;
//    const int PROBE_MSG_TYPE = 1;

    const int MaxMinMsgSize = 1;
    const int TotalMsgsToCheck = 2000; // set to 400 --> history of 4s
    const float InitUtilScore = 1;
//    const float ackTimeOutFactor = 5.0; // set to constant: 400ms
    const float ACKTimeoutDuration = 0.5;
    const int InQueueMaxSize = 600; // order of 10, not 100, order of bandwidth delay product (bdp)
    const int OutQueueMaxSize = 1000; // order of 10, not 100 --> maybe 20,30
};

struct AIMDConfig {
    const float ProbeMsgSize = 0.01;
    const float ProbeMsgTimeOut = 0.5;
    const float ProbeMsgFreq = 0.02; // change to interval, make it 0.001

    const float AddVal = 0.05;
    const float MultFactor = 0.99;
    const float InitVal = 1;
    const float MinVal = 0.5;
    const float MaxVal = 1;
};

enum QueueType{
    IN = 0,
    OUT = 1
};

struct MaxMinMsgInfo {
    int msgId;
    int intermediate;
    std::set<int> receivers;
    int numAcksExp;
    bool timeOut;
};

struct ProbeMsgInfo {
    long msgId;
//    int dstIdx;
    bool timeOut;
};

// for dynamic type checking
template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
   return dynamic_cast<const Base*>(ptr) != nullptr;
}

// utility comparator function to pass to the sort() module
bool sortByVal(const std::pair<int, int> &a,
               const std::pair<int, int> &b);

std::vector<std::pair<int,float>> sortMapByValue(std::map<int,float> M);

void printMapIntToInt(std::map<int, int> myMap);
void printMapIntToFloat(std::map<int, float> myMap);
//void printMapIntToIntArray(std::map<int, int[]> myMap);
void printMapPairIntIntToMaxMinMsg(std::map<std::pair<int,int>, MaxMinMsg*> myMap);


#endif
