#include "utils.h"

// utility comparator function to pass to the sort() module
bool sortByVal(const std::pair<int, int> &a,
               const std::pair<int, int> &b)
{
    return (a.second < b.second);
}


std::vector<std::pair<int,float>> sortMapByValue(std::map<int,float> M)
{
    // Declare vector of pairs
    std::vector<std::pair<int,float>> A;
    // Copy key-value pair from Map
    // to vector of pairs
    for (auto& it : M) {
        A.push_back(it);
    }
    // Sort using comparator function
    sort(A.begin(), A.end(), sortByVal);
    return A;
}


void printMapIntToInt(std::map<int, int> myMap){
    for(const auto& elem : myMap)
    {
       EV << elem.first << " " << elem.second << "\n";
    }
}

void printMapIntToFloat(std::map<int, float> myMap){
    for(const auto& elem : myMap)
    {
       EV << elem.first << " " << elem.second << "\n";
    }
}

void printMapPairIntIntToMaxMinMsg(std::map<std::pair<int,int>, MaxMinMsg*> myMap){
    for(auto& elem : myMap)
    {
       EV << "msgID: " << elem.first.first << ", dstIdx: " << elem.first.second << "\n";
    }
}



