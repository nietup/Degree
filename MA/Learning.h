//
// Created by nietup on 22.05.17.
//

#ifndef DEGREE_LEARNING_H
#define DEGREE_LEARNING_H

#include <iostream>
#include "MatchingSystem.h"

using namespace std;

//needs to be remade
using Hypothesis = vector<vector<BoolPlus>>;

//will be totally replaced with matching algoritm
bool Consistent(const Hypothesis & a, const Hypothesis & b) {
    if (!a.size())
        return true;

    auto pairCount = a.size();
    auto constraintCount = a[0].size();

    for(auto i = 0; i < pairCount; i++) {
        for (auto j = 0; j < constraintCount; j++) {
            if (DNC != a[i][j] && DNC != b[i][j] && a[i][j] != b[i][j]) {
                return false;
            }
        }
    }

    return true;
}

//I don't even now how to tackle negative samples that way
bool MoreGeneralOrEqual(const Hypothesis & a, const Hypothesis & b) {
    auto pairCount = a.size();
    auto constraintCount = a[0].size();
    auto changes = 0;

    for(auto i = 0; i < pairCount; i++) {
        for (auto j = 0; j < constraintCount; j++) {
            if (a[i][j] != b[i][j]) {
                changes++;
                if (1 < changes || DNC != a[i][j])
                    return false;
            }
        }
    }

    return true;
}

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

/*void PrintHypothesis(Hypothesis h) {
    cout << "\nHypothesis\n";
    for (auto & row : h) {
        for (auto & field : row) {
            cout << field << " ";
        }
        cout << endl;
    }
}*/

//this function does change furthest part in the model so that it is consistent
//with the most similar part from sample
void Generalize(Part * furthestPart,
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


//I don't even now how to tackle negative samples that way
//returns false if cannot be further specialized
vector<Hypothesis> Specialize(const Hypothesis & h, const Hypothesis & s,
    const vector<Hypothesis> & g, const Hypothesis & counterexample) {

    //chose right DNC
    auto pairCount = h.size();
    auto constraintCount = h[0].size();
    auto fieldsOfSpecialization = vector<pair<int, int>>{};

    for(auto i = 0; i < pairCount; i++) {
        for (auto j = 0; j < constraintCount; j++) {
            if (DNC == h[i][j]) {
                //if counterexample is consistent with s on this field then skip
                if (s[i][j] == counterexample[i][j] || DNC == s[i][j]) {
                    continue;
                }
                fieldsOfSpecialization.push_back({i, j});
            }
        }
    }

    auto hIndex = 0;
    for (auto & a : g)
        if (h == a)
            break;
        else
            hIndex++;
    auto retVals = vector<Hypothesis>{};
    for (auto & field : fieldsOfSpecialization) {
        auto newSpecialized = Hypothesis(pairCount,
                                         vector<BoolPlus>(constraintCount));

        //copy(h.begin(), h.end(), back_inserter(newSpecialized));
        newSpecialized = g[hIndex];
        newSpecialized[field.first][field.second] =
            (YES == counterexample[field.first][field.second]) ? NO : YES;

        retVals.push_back(newSpecialized);
    }

    if (!fieldsOfSpecialization.size())
        return vector<Hypothesis>{};
    
    return retVals;
}

uint GetPairsCount(uint pairsCount) {
    for (auto i = 1u; i < pairsCount; i++) {
        if (0.5*i*(i-1) == pairsCount) {
            return i;
        }
    }
    return 0u;
}

shared_ptr<SModel> GenerateModel(
    const vector<vector<weak_ptr<LineWrap>>> & positiveSamples,
    const vector<vector<weak_ptr<LineWrap>>> & negativeSamples,
    const vector<shared_ptr<Constraint>> & constraints) {

    const auto atomCount = positiveSamples[0].size();
    const auto pairCount = (uint)(0.5*atomCount*(atomCount-1));
    const auto constraintCount = constraints.size();

    //creation of first hypothesis
    //S must be initialized by first positive sample
    auto sExtract = Extract(positiveSamples[0], pairCount, constraints);

    //make vector of atoms
    auto atoms = vector<shared_ptr<Atom>>();
    for (auto i = 0; i < atomCount; i++) {
        atoms.push_back(make_shared<Atom>(Atom{}));
    }

    //make vector of parts
    auto parts = vector<shared_ptr<Part>>();

    //connects parts with atoms
    for (auto i = 0; i < pairCount; i++) {
        parts.push_back(make_shared<Part>(Part{}));
        parts[i]->constraints = sExtract[i];
        auto atomsI = unpair(i);
        parts[i]->atoms = {weak_ptr<Atom>{atoms[atomsI.first]},
                           weak_ptr<Atom>{atoms[atomsI.second]}};
        atoms[atomsI.first]->involved.push_back(weak_ptr<Part>{parts[i]});
        atoms[atomsI.second]->involved.push_back(weak_ptr<Part>{parts[i]});
    }

    auto s = make_shared<SModel>(SModel{parts, atoms, constraints});


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
        auto furthestPart = weak_ptr<Part>{};
        tie(isMatched, furthestPart) = Match(*s, sample);
        //reset assignments in atoms
        for (auto & a : s.get()->atoms) {
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
        for (auto & row : s->parts) {
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
                if (DNC == s->parts[i]->constraints[j]
                    || extract[i][j] == s->parts[i]->constraints[j]) {
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
    for (auto & pair : s->parts) {
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
