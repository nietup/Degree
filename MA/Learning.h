//
// Created by nietup on 22.05.17.
//

#ifndef DEGREE_LEARNING_H
#define DEGREE_LEARNING_H

enum BoolPlus {NO, YES, DNC};

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

unique_ptr<SModel> GenerateModel(
    const vector<vector<weak_ptr<LineWrap>>> & positiveSamples,
    const vector<vector<weak_ptr<LineWrap>>> & negativeSamples,
    const vector<shared_ptr<Constraint>> & constraints) {
    const auto atomCount = positiveSamples[0].size();
    const auto pairCount = (uint)(0.5*atomCount*(atomCount-1));
    const auto constraintCount = constraints.size();

    //first - pair id, second - constraint in pair
    //S must be initialized by first positive sample
    //auto s = Extract(positiveSamples[0], pairCount, constraints);
    auto s = unique_ptr<SModel>(new SModel);

    auto g = vector<Hypothesis>{
        Hypothesis(
            pairCount,
            vector<BoolPlus>(constraintCount, DNC)
        )
    };

    for (const auto & sample : positiveSamples) {
        //New matching done here
        auto isMatched = bool{};
        auto furthestPart = weak_ptr<Part>{};
        tie(isMatched, furthestPart) = Match(*s, sample);
        if (!isMatched) {
            //generalize and match again
        }
    }

    for (const auto & sample : negativeSamples) {
        //we leave neg samples for now
    }

    /*cout << "S: \n";
    for (auto & pair : s) {
        for (auto & field : pair) {
            cout << field << " ";
        }
        cout << endl;
    }

    cout << "\nG: \n";
    for (auto & h : g){
        for (auto & pair : h) {
            for (auto &field : pair) {
                cout << field << " ";
            }
            cout << endl;
        }
        cout << endl;
    }*/

    return s;
}

#endif //DEGREE_LEARNING_H
