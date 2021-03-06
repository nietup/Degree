//
// Created by nietup on 22.05.17.
//

#ifndef DEGREE_LEARNING_H
#define DEGREE_LEARNING_H

#include <iostream>
#include "MatchingSystem.h"

using namespace std;

using Hypothesis = vector<vector<BoolPlus>>;
class LearningSystem {
public:
    LearningSystem() {}

Hypothesis Extract(const vector<weak_ptr<LineWrap>> & sample, uint pairCount,
                   const vector<shared_ptr<Constraint>> & constraints) {

    auto constraintCount = constraints.size();
    auto hypothesis = Hypothesis(pairCount, vector<BoolPlus>(constraintCount,
                                                             NO));
    auto util = Utilities();

    for(auto i = 0; i < pairCount; i++) {
        auto segsPair = util.unpair(i);
        for (auto j = 0; j < constraintCount; j++) {
            //make sure that the sample is not to small
            if (segsPair.first < sample.size() &&
                segsPair.second < sample.size()) {

                //if constraint is fulfilled, then fill YES
                if (threshold > constraints[j]->operator()(
                    *sample[segsPair.first].lock(),
                    *sample[segsPair.second].lock())) {

                    hypothesis[i][j] = YES;
                }
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
        if (bestMatch.diffs.size() > thisRowScore.diffs.size() &&
            //pick line that is the most similar but is not consistent
            thisRowScore.diffs.size() > 0) {
            bestMatch = thisRowScore;
        }
    }

    for (auto i : bestMatch.diffs) {
        furthestPart->constraints[i] = DNC;
    }

}

pair<shared_ptr<Model>, shared_ptr<Hypothesis>> GenerateModel(
    const vector<vector<weak_ptr<LineWrap>>> & positiveSamples,
    const vector<vector<weak_ptr<LineWrap>>> & negativeSamples,
    const vector<shared_ptr<Constraint>> & constraints,
    Model model, Hypothesis gbound) {

    const auto atomCount = positiveSamples[0].size();
    const auto pairCount = (uint) (0.5 * atomCount * (atomCount - 1));
    const auto constraintCount = constraints.size();
    auto util = Utilities();

    auto s = shared_ptr<Model>{};
    auto gret = shared_ptr<Hypothesis>{};

    if (model.constraints.size()) {
        s = make_shared<Model>(model);
    } else {
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
            auto atomsI = util.unpair(i);
            parts[i]->vertices = {weak_ptr<Vertex>{atoms[atomsI.first]},
                                  weak_ptr<Vertex>{atoms[atomsI.second]}};
            atoms[atomsI.first]->involved.push_back(weak_ptr<Edge>{parts[i]});
            atoms[atomsI.second]->involved.push_back(weak_ptr<Edge>{parts[i]});
        }

        s = make_shared<Model>(Model{parts, atoms, constraints});
    }

    auto matcher = Matcher();

    auto g = Hypothesis{};
    if (0 < gbound.size()) {
        g = gbound;
    }
    else {
        for (auto i = 0; i < pairCount; i++) {
            g.push_back(vector<BoolPlus>{});
            for (auto j = 0; j < constraintCount; j++) {
                g[i].push_back(DNC);
            }
        }
    }

    for (const auto & sample : positiveSamples) {
        //New matching done here
        auto isMatched = bool{};
        auto furthestPart = weak_ptr<Edge>{};
        tie(isMatched, furthestPart) = matcher.Match(*s, sample);
        //reset assignments in vertices
        for (auto & a : s.get()->vertices) {
            a.get()->asignment.reset();
        }
        while (!isMatched) {
            //generalize and match again
            Generalize(furthestPart.lock().get(), sample, constraints);
            tie(isMatched, furthestPart) = matcher.Match(*s, sample);
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
            //if best index is -1 then no line in extract is matching with s
            //so just take the first line
            if (-1 == bestIndex) {
                bestIndex = 0;
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

    /*cout << "S: \n";
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
    */

    gret = make_shared<Hypothesis>(g);

    return {s, gret};
}
};
#endif //DEGREE_LEARNING_H
