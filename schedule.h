#ifndef __SCHEDULE_H
#define __SCHEDULE_H

#include <utils.h>

#include <cassert>


using namespace omnetpp;


float roundUp(float n, int decimals);
float roundDown(float n, int decimals);

std::map<int, float> getRoundedScores(std::map<int, float> scores, int prec);
int findClosestEmptySlot(std::map<int, int> leaderSchedule, int idx);

std::map<int, int> getLeaderSchedule(int numNodes, int numMsgs, std::map<int, float> uScores, int prec, float epsilon);




#endif



