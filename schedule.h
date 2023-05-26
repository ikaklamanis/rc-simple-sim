#ifndef __SCHEDULE_H
#define __SCHEDULE_H

#include <utils.h>

#include <cassert>


using namespace omnetpp;
using namespace std;


float roundUp(float n, int decimals);
float roundDown(float n, int decimals);

map<int, float> getRoundedScores(map<int, float> scores, int prec, int gateSize, map<int, int> gateToPeer);
int findClosestEmptySlot(map<int, int> leaderSchedule, int idx);

map<int, int> getLeaderSchedule(int numNodes, int numMsgs, map<int, float> uScoresNorm, int prec, float epsilon,
        int gateSize, map<int, int> gateToPeer);




#endif



