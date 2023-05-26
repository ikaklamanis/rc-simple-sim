#ifndef __UTILS_H
#define __UTILS_H

#include <omnetpp.h>
#include <vector>
#include <map>
#include <algorithm> // for sort function
#include <numeric>
#include "maxmin_m.h"

using namespace omnetpp;
using namespace std;


const int numNodes = 4; // int numNodes = access array size
// the first node (index 0) is the leader
//const float ingress[numNodes] = {100, 1000, 1000, 1000};
//const float egress[numNodes] =  {1000, 670, 670, 670};
//
//const float ingress[numNodes] = {100, 1600, 1000, 400};
//const float egress[numNodes] =  {1600, 800, 400, 400};

const float ingress[numNodes] = {1000, 1000, 1000, 1000};
const float egress[numNodes] =  {1000, 1200, 600, 200};

// For reference: propagation delay is 10ms = 0.01s
struct Config {
    const int LEADER_IDX = 0;

    const int MaxMinMsgSize = 1;
    const int TotalMsgsToCheck = 1000;
    const float InitUtilScore = 1;

    const float UtilAddVal = 1; // TODO: make it inverse-proportional to current utility value
    const int LeaderScheduleSize = 100;
    const float Prec = 3;
    const float Epsilon = 0.02;

//    const float ackTimeOutFactor = 5.0; // set to constant: 400ms
    const float ACKTimeoutDuration = 0.2;
    const int InQueueMaxSize = 200; // order of 10, not 100, order of bandwidth delay product (bdp)
    const int OutQueueMaxSize = 200; // order of 10, not 100 --> maybe 20,30

    const float RxRateSignalInterval = 0.5; // emit signal every 0.5 seconds

    const float RateChangeInterval = 20;
};

struct AIMDConfig {
    const float AddVal = 0.01;
    const float MultFactor = 0.5;
    const float InitVal = 0.5;
    const float MinVal = 0.1;
    const float MaxVal = 1;

    const float MDInterval = 0.5;
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
    set<int> receivers;
    int numAcksExp;
    bool timeOut;
//    map<int, int> intermSeqNumMap;
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
bool sortByVal(const pair<int, int> &a,
               const pair<int, int> &b);

vector<pair<int,float>> sortMapByValue(map<int,float> M);

void printMapIntToInt(map<int, int> myMap);
void printMapIntToFloat(map<int, float> myMap);
//void printMapIntToIntArray(map<int, int[]> myMap);
void printMapPairIntIntToMaxMinMsg(map<pair<int,int>, MaxMinMsg*> myMap);

void printSchedule(map<int, int> myMap);


#endif
