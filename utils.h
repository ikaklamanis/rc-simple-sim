#ifndef __UTILS_H
#define __UTILS_H

#include <omnetpp.h>
#include <vector>
#include <map>
#include <algorithm> // for sort function
#include <numeric>
#include "maxmin_m.h"

using namespace omnetpp;


const int numNodes = 4;
// the first node (index 0) is the leader
const int ingress[numNodes] = {100, 3000, 3000, 3000};
const int egress[numNodes] =  {1000, 1000, 400, 100};

//const int ackTimeOuts[numNodes] = {0.2, 0.2, 0.2, 0.2};

// For reference: propagation delay is 10ms = 0.01s
struct Config {
    const int LEADER_IDX = 0;

    const int MaxMinMsgSize = 1;
    const int TotalMsgsToCheck = 4000; // set to 400 --> history of 4s
    const float InitUtilScore = 1;

    const float UtilAddVal = 1; // TODO: make it inverse-proportional to current utility value
    const int LeaderScheduleSize = 100;
    const float Prec = 3;
    const float Epsilon = 0.02;

//    const float ackTimeOutFactor = 5.0; // set to constant: 400ms
    const float ACKTimeoutDuration = 0.2;
    const int InQueueMaxSize = 400; // order of 10, not 100, order of bandwidth delay product (bdp)
    const int OutQueueMaxSize = 400; // order of 10, not 100 --> maybe 20,30
};

struct AIMDConfig {
    const float AddVal = 0.05;
    const float MultFactor = 0.9;
    const float InitVal = 1;
    const float MinVal = 0.1;
    const float MaxVal = 1;

    const float MDInterval = 0.04;
};

enum AIMDUpdateType{
    NO_UPDATE = 0,
    INCREASE = 1,
    DECREASE = 2,

    CAN_DECREASE = 3,
    NO_DECREASE = 4
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
    int intermSeqNum;
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

void printSchedule(std::map<int, int> myMap);


#endif
