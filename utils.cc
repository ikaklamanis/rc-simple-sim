#include "utils.h"

// utility comparator function to pass to the sort() module
bool sortByVal(const pair<int, int> &a,
               const pair<int, int> &b)
{
    return (a.second < b.second);
}


vector<pair<int,float>> sortMapByValue(map<int,float> M)
{
    // Declare vector of pairs
    vector<pair<int,float>> A;
    // Copy key-value pair from Map
    // to vector of pairs
    for (auto& it : M) {
        A.push_back(it);
    }
    // Sort using comparator function
    sort(A.begin(), A.end(), sortByVal);
    return A;
}


void printMapIntToInt(map<int, int> myMap){
    for(const auto& elem : myMap)
    {
       EV << elem.first << ": " << elem.second << "\n";
    }
    EV << "\n";
}

void printMapIntToFloat(map<int, float> myMap){
    for(const auto& elem : myMap)
    {
       EV << elem.first << ": " << elem.second << "\n";
    }
    EV << "\n";
}

//void printMapIntToIntArray(map<int, int[]> myMap){
//    for(const auto& elem : myMap)
//    {
//       EV << elem.first << ": {";
//       for (int id : elem.second){
//           EV << id << ", ";
//       }
//       EV << "} \n";
//    }
//    EV << "\n";
//}

void printMapPairIntIntToMaxMinMsg(map<pair<int,int>, MaxMinMsg*> myMap){
    for(auto& elem : myMap)
    {
       EV << "msgID: " << elem.first.first << ", dstIdx: " << elem.first.second << "\n";
    }
    EV << "\n";
}

void printSchedule(map<int, int> myMap){
    for(const auto& elem : myMap)
    {
        EV << elem.second << ",";
    }
    EV << "\n";
}



