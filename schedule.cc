#include <schedule.h>


//using namespace omnetpp;


float roundUp(float n, int decimals){
    int multiplier = pow(10, decimals);
    return (float) ceil(n * multiplier) / (float) multiplier;
}

float roundDown(float n, int decimals){
    int multiplier = pow(10, decimals);
    return (float) ceil(n * multiplier) / (float) multiplier;
}

map<int, float> getRoundedScores(map<int, float> scores, int prec, int gateSize, map<int, int> gateToPeer){
    float credit = 0;
    map<int, float> scoresRounded;

    for (auto& score : scores) {
        score.second = roundUp(score.second, prec + 2);
    }
    auto cmp = [](const pair<int, float>& a, const pair<int, float>& b) {
        return a.second > b.second;
    };

    vector<int> gates = vector<int>(gateSize);
    for (int i = 0; i < gateSize; i++){
        gates[i] = i;
    }
    random_shuffle ( gates.begin(), gates.end() );  // in place no extra

    EV << "random shuffled gates and scores:" << endl;
    for (std::vector<int>::iterator it=gates.begin(); it!=gates.end(); ++it){
       EV << ' ' << *it << " : " << scores[gateToPeer[*it]] << endl;
    }
    EV << endl;

//    vector<pair<int, float>> sorted_scores(scores.begin(), scores.end());
//    sort(sorted_scores.begin(), sorted_scores.end(), cmp);

    for (int i = 0; i < gateSize; i++){
//    for (const auto& score : sorted_scores) {
//        int nodeIdx = score.first;
//        float s = score.second;
        int nodeIdx = gateToPeer[gates[i]];
        float s = scores[nodeIdx];
        float diff_ceil = roundUp(s, prec) - s;
        float diff_floor = s - roundDown(s, prec);
        if (diff_ceil == 0) {
            scoresRounded[nodeIdx] = s;
        }
        else if (credit >= diff_ceil) {
            credit -= diff_ceil;
            scoresRounded[nodeIdx] = roundUp(s, prec);
        }
        else {
            credit += diff_floor;
            scoresRounded[nodeIdx] = roundDown(s, prec);
        }
    }
    return scoresRounded;
}



int findClosestEmptySlot(map<int, int> leaderSchedule, int idx) {
    if (leaderSchedule[idx] == -1) {
        return idx;
    }
    int min_idx = leaderSchedule.begin()->first;
    int max_idx = leaderSchedule.rbegin()->first;
    for (int i = 0; i < max_idx + 1; i++) {
        vector<int> indices{idx + i, idx - i};
        for (int next_idx : indices) {
            if (leaderSchedule.find(next_idx) != leaderSchedule.end() && leaderSchedule[next_idx] == -1) {
                return next_idx;
            }
        }
    }
    return -1;
}


// TODO: currently assuming that follower nodes are nodes 1,2,3,... Make more modular

map<int, int> getLeaderSchedule(int numNodes, int numMsgs, map<int, float> uScoresNorm, int prec, float epsilon,
        int gateSize, map<int, int> gateToPeer) {

    map<int, float> uScoresRounded;
    map<int, int> nodeToNumMsgs, leaderSchedule;
    map<int, vector<float>> nodeToLocs;

    uScoresRounded = getRoundedScores(uScoresNorm, prec, gateSize, gateToPeer);
    EV << "uScoresRounded" << endl;
    for(int i=1; i<numNodes; i++) {
        EV << i << " : " << uScoresRounded[i] << endl;
    }

    for (int nodeIdx = 1; nodeIdx < numNodes; nodeIdx++) {
        float freq = 1.0 / uScoresRounded[nodeIdx];
        nodeToLocs[nodeIdx] = {};
        int i = 0;
        while (i * freq <= numMsgs - 1) {
            nodeToLocs[nodeIdx].push_back(i * freq);
            i++;
        }
        // nodeTokens[nodeIdx] = 1;
    }

    for (int msgId = 0; msgId < numMsgs; msgId++) {
        leaderSchedule[msgId] = -1;
    }

    vector<int> gates = vector<int>(gateSize);
    for (int i = 0; i < gateSize; i++){
        gates[i] = i;
    }
    random_shuffle ( gates.begin(), gates.end() );  // in place no extra

//    vector<pair<int, float>> sorted_uScoresNorm(uScoresRounded.begin(), uScoresRounded.end());
//    sort(sorted_uScoresNorm.begin(), sorted_uScoresNorm.end(), [](auto const& lhs, auto const& rhs) {
//        return lhs.second < rhs.second;
//    });

    EV << "random shuffled uScoresNorm:" << endl;
    for (int i = 0; i < gateSize; i++){
       EV << "" << gateToPeer[gates[i]] << " : " << uScoresRounded[gateToPeer[gates[i]]] << endl;
    }
    EV << endl;

    for (int i = 0; i < uScoresRounded.size() - 1; i++) {
        int nodeIdx = gateToPeer[gates[i]];
//        int nodeIdx = sorted_uScoresNorm[i].first;
        vector<float> locs = nodeToLocs[nodeIdx];
        for (int j = 0; j < locs.size(); j++) {
            int loc = findClosestEmptySlot(leaderSchedule, round(locs[j]));
            if (loc != -1) {
                leaderSchedule[loc] = nodeIdx;
            }
        }
    }

//    int lastNodeIdx = sorted_uScoresNorm.back().first;
    int lastNodeIdx = gateToPeer[gates.back()];
    for (int msgId = 0; msgId < numMsgs; msgId++) {
        if (leaderSchedule[msgId] == -1) {
            leaderSchedule[msgId] = lastNodeIdx;
        }
    }

    // Print leader schedule
//    EV << "Leader schedule:" << endl;
//    for(int i=0; i<numMsgs; i++) {
//        EV << leaderSchedule[i] << " ";
//    }
//    EV << endl;

    map<int, float> uScoresCalc;
    for (int nodeIdx = 1; nodeIdx < numNodes; nodeIdx++) {
        uScoresCalc[nodeIdx] = 0;
    }

    for (int msgId = 0; msgId < numMsgs; msgId++) {
        int nodeIdx = leaderSchedule[msgId];
        assert(nodeIdx != -1);
        if (nodeIdx != -1) {
            uScoresCalc[nodeIdx] += 1;
        }
    }

    for (int nodeIdx = 1; nodeIdx < numNodes; nodeIdx++) {
        uScoresCalc[nodeIdx] = uScoresCalc[nodeIdx] / numMsgs;
        EV << "uScoreCalc of node " << nodeIdx << " is " << uScoresCalc[nodeIdx] << endl;
        // EV << "epsilon: " << epsilon << endl;
        if(abs(uScoresCalc[nodeIdx] - uScoresNorm[nodeIdx]) > epsilon) {
            EV_WARN << "Node " << nodeIdx << " uScoresNorm = " << uScoresNorm[nodeIdx] << " and uScoresCalc = " << uScoresCalc[nodeIdx] << endl;
        }
//        assert(abs(uScoresCalc[nodeIdx] - uScoresNorm[nodeIdx]) < epsilon);
    }

    return leaderSchedule;

}


