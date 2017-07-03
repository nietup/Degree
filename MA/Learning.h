//
// Created by nietup on 22.05.17.
//

#ifndef DEGREE_LEARNING_H
#define DEGREE_LEARNING_H

#include <iostream>
#include "MatchingSystem.h"

using namespace std;

using Hypothesis = vector<vector<BoolPlus>>;

pair<uint, uint> unpair(uint i) {
    int y = (int) floor(0.5 * (sqrt(8.0 * i + 1.0) + 1.0));
    int x = (int) (0.5 * (y * y + y - 2 * i) - 1);
    return pair<uint, uint>{x, y};
};

Hypothesis Extract(const vector<weak_ptr<LineWrap>> & sample, uint pairCount,
                   const vector<shared_ptr<Constraint>> & constraints) {

    auto constraintCount = constraints.size();
    auto hypothesis = Hypothesis(pairCount, vector<BoolPlus>(constraintCount,
                                                             NO));

    for(auto i = 0; i < pairCount; i++) {
        auto segsPair = unpair(i);
        for (auto j = 0; j < constraintCount; j++) {
            if (threshold > constraints[j]->operator()(
                *sample[segsPair.first].lock(),
                *sample[segsPair.second].lock())) {
                hypothesis[i][j] = YES;
            }
        }
    }

    return hypothesis;
}

//this function does change furthest part in the model so that it is consistent
//with the most similar part from sample
void Generalize(Edge * furthestPart,
    const vector<weak_ptr<LineWrap>> & sample,
    const vector<shared_ptr<Constraint>> & constraints) {

    auto pairCount = 0.5*sample.size()*(sample.size()-1);
    auto extract = Extract(sample, pairCount, constraints);

    struct BestMatch {
        uint index;
        vector<int> diffs;
    };

    auto cSize = constraints.size();
    auto bestMatch = BestMatch{0, vector<int>(cSize+1)};

    auto partRow = furthestPart->constraints;

    for (auto i = 0; i < pairCount; i++) {
        auto thisRowScore = BestMatch{i, {}};
        for (auto j = 0; j < cSize; j++) {
            if (DNC != partRow[j] && extract[i][j] != partRow[j]) {
                thisRowScore.diffs.push_back(j);
            }
        }
        if (bestMatch.diffs.size() > thisRowScore.diffs.size()) {
            bestMatch = thisRowScore;
        }
    }

    for (auto i : bestMatch.diffs) {
        furthestPart->constraints[i] = DNC;
    }
}

shared_ptr<Model> GenerateModel(
    const vector<vector<weak_ptr<LineWrap>>> & positiveSamples,
    const vector<vector<weak_ptr<LineWrap>>> & negativeSamples,
    const vector<shared_ptr<Constraint>> & constraints) {

    const auto atomCount = positiveSamples[0].size();
    const auto pairCount = (uint)(0.5*atomCount*(atomCount-1));
    const auto constraintCount = constraints.size();

    //creation of first hypothesis
    //S must be initialized by first positive sample
    auto sExtract = Extract(positiveSamples[0], pairCount, constraints);

    //make vector of vertices
    auto atoms = vector<shared_ptr<Vertex>>();
    for (auto i = 0; i < atomCount; i++) {
        atoms.push_back(make_shared<Vertex>(Vertex{}));
    }

    //make vector of edges
    auto parts = vector<shared_ptr<Edge>>();

    //connects edges with vertices
    for (auto i = 0; i < pairCount; i++) {
        parts.push_back(make_shared<Edge>(Edge{}));
        parts[i]->constraints = sExtract[i];
        auto atomsI = unpair(i);
        parts[i]->vertices = {weak_ptr<Vertex>{atoms[atomsI.first]},
                           weak_ptr<Vertex>{atoms[atomsI.second]}};
        atoms[atomsI.first]->involved.push_back(weak_ptr<Edge>{parts[i]});
        atoms[atomsI.second]->involved.push_back(weak_ptr<Edge>{parts[i]});
    }

    auto s = make_shared<Model>(Model{parts, atoms, constraints});


    auto g = Hypothesis{};
    for (auto i = 0; i < pairCount; i++) {
        g.push_back(vector<BoolPlus>{});
        for (auto j = 0; j < constraintCount; j++) {
            g[i].push_back(DNC);
        }
    }

    for (const auto & sample : positiveSamples) {
        //New matching done here
        auto isMatched = bool{};
        auto furthestPart = weak_ptr<Edge>{};
        tie(isMatched, furthestPart) = Match(*s, sample);
        //reset assignments in vertices
        for (auto & a : s.get()->vertices) {
            a.get()->asignment.reset();
        }
        while (!isMatched) {
            //generalize and match again
            Generalize(furthestPart.lock().get(), sample, constraints);
            tie(isMatched, furthestPart) = Match(*s, sample);
        }
    }

    for (const auto & sample : negativeSamples) {
        auto extract = Extract(sample, pairCount, constraints);

        auto sortedExtract = Hypothesis{};

        //change oder to maximally match s
        for (auto & row : s->edges) {
            auto bestScore = 0;
            auto bestIndex = -1;
            //for every line in extract
                //count score / update bests
            //best push bak sorted, pop extract
            for (auto i = 0; i < extract.size(); i++) {
                auto pairScore = 0;
                for (auto j = 0; j < constraintCount; j++) {
                    if (extract[i][j] == row->constraints[j]) {
                        pairScore++;
                    }
                }
                if (pairScore > bestScore) {
                    bestIndex = i;
                    bestScore = pairScore;
                }
            }
            sortedExtract.push_back(extract[bestIndex]);
            extract.erase(extract.begin()+ bestIndex);
        }
        extract = sortedExtract;

        auto eSize = extract.size();
        for (auto i = 0; i < eSize; i++) {
            for (auto j = 0; j < constraintCount; j++) {
                if (DNC == s->edges[i]->constraints[j]
                    || extract[i][j] == s->edges[i]->constraints[j]) {
                    extract[i][j] = DNC;
                } else {
                    if (YES == extract[i][j]) {
                        extract[i][j] = NO;
                    } else if (NO == extract[i][j]) {
                        extract[i][j] = YES;
                    }
                }
            }
        }


        //sum with G
        for (auto i = 0; i < eSize; i++) {
            for (auto j = 0; j < constraintCount; j++) {
                if (DNC != g[i][j]) {
                    extract[i][j] = g[i][j];
                }
            }
        }
        g = extract;
    }

    cout << "S: \n";
    for (auto & pair : s->edges) {
        for (auto & field : pair->constraints) {
            cout << field << " ";
        }
        cout << endl;
    }

    cout << "\nG: \n";
        for (auto & pair : g) {
            for (auto &field : pair) {
                cout << field << " ";
            }
            cout << endl;
        }
        cout << endl;


    return s;
}

#endif //DEGREE_LEARNING_H
