#ifndef __UTILS_H
#define __UTILS_H

#include <omnetpp.h>
#include <vector>
#include <map>
#include <algorithm> // for sort function
#include "maxmin_m.h"

using namespace omnetpp;


struct Config {
    const int LEADER_IDX = 0;
    const int TotalMsgsToCheck = 20;
    const int IntermMsgsToCheck = 5;
    const int MsgSize = 1;
    const int LeaderMsgTimeout = 2;
    const float initUtilScore = 1;

};

struct MsgInfo {
    int msgId;
    int intermediate;
    std::set<int> receivers;
    int numAcksExp;
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
void printMapPairIntIntToMaxMinMsg(std::map<std::pair<int,int>, MaxMinMsg*> myMap);


#endif
