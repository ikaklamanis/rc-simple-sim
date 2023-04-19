#include <schedule.h>


using namespace omnetpp;

float roundUp(float n, int decimals){
    int multiplier = pow(10, decimals);
    return (float) ceil(n * multiplier) / (float) multiplier;
}

float roundDown(float n, int decimals){
    int multiplier = pow(10, decimals);
    return (float) ceil(n * multiplier) / (float) multiplier;
}

std::map<int, float> getRoundedScores(std::map<int, float> scores, int prec){
    float credit = 0;
    std::map<int, float> scoresRounded;
    for (auto& score : scores) {
        score.second = roundUp(score.second, prec + 2);
    }
    auto cmp = [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
        return a.second > b.second;
    };
    std::vector<std::pair<int, float>> sorted_scores(scores.begin(), scores.end());
    sort(sorted_scores.begin(), sorted_scores.end(), cmp);
//    double sum_scores = 0;
//    for (const auto& score : sorted_scores) {
//        sum_scores += score.second;
//    }
//    EV << "sum sorted_scores = " << sum_scores << endl;
    for (const auto& score : sorted_scores) {
        int nodeIdx = score.first;
        float s = score.second;
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



int findClosestEmptySlot(std::map<int, int> leaderSchedule, int idx) {
    if (leaderSchedule[idx] == -1) {
        return idx;
    }
    int min_idx = leaderSchedule.begin()->first;
    int max_idx = leaderSchedule.rbegin()->first;
    for (int i = 0; i < max_idx + 1; i++) {
        std::vector<int> indices{idx + i, idx - i};
        for (int next_idx : indices) {
            if (leaderSchedule.find(next_idx) != leaderSchedule.end() && leaderSchedule[next_idx] == -1) {
                return next_idx;
            }
        }
    }
    return -1;
}


// TODO: currently assuming that follower nodes are nodes 1,2,3,... Make more modular

std::map<int, int> getLeaderSchedule(int numNodes, int numMsgs, std::map<int, float> uScoresNorm, int prec, float epsilon) {
    std::map<int, float> uScoresRounded;
    std::map<int, int> nodeToNumMsgs, leaderSchedule;
    std::map<int, std::vector<float>> nodeToLocs;

    uScoresRounded = getRoundedScores(uScoresNorm, prec);
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

    std::vector<std::pair<int, float>> sorted_uScoresNorm(uScoresRounded.begin(), uScoresRounded.end());
    std::sort(sorted_uScoresNorm.begin(), sorted_uScoresNorm.end(), [](auto const& lhs, auto const& rhs) {
        return lhs.second < rhs.second;
    });

    for (int i = 0; i < sorted_uScoresNorm.size() - 1; i++) {
        int nodeIdx = sorted_uScoresNorm[i].first;
        std::vector<float> locs = nodeToLocs[nodeIdx];
        for (int j = 0; j < locs.size(); j++) {
            int loc = findClosestEmptySlot(leaderSchedule, round(locs[j]));
            if (loc != -1) {
                leaderSchedule[loc] = nodeIdx;
            }
        }
    }

    int lastNodeIdx = sorted_uScoresNorm.back().first;
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

    std::map<int, float> uScoresCalc;
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


//int main() {
//    int numNodes = 4;
//    int numMsgs = 100;
//
//    int prec = 3;
//    float epsilon = 0.02;
//
//    for (int i = 1; i < 20; i++) {
//        for (int j = 1; j < 50; j++) {
//            for (int k = 1; k < 50; k++) {
//                std::map<int, float> uScores = {{1, i}, {2, j}, {3, k}};
//                    EV << "uScores" << endl;
//                    for(int i=1; i<numNodes; i++) {
//                        EV << i << " : " << uScores[i] << endl;
//                    }
//                    EV << endl;
//                std::map<int, int> leaderSchedule = getLeaderSchedule(numNodes, numMsgs, uScores, prec, epsilon);
//                // do something with leaderSchedule
//            }
//        }
//    }
//
//    return 0;
//}


