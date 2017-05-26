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
        //intro stuff
        auto isModelMatched = true;
        auto furthestPart = weak_ptr<Part>();
        auto AtomsSize = s->atoms.size();
        auto PartsSize = s->parts.size();
        if (!AtomsSize || !PartsSize)
            isModelMatched = false;
        s->atoms[0]->asignment = sample[0];
        sample[0].lock()->matched = true;
        auto match = SearchTree{ TreeNode {s->atoms[0]} };

        //main loop
        auto i = 1u;
        while (i < AtomsSize) {
            //if root didn't work out -- move to separate function later
            if (i == 0) {
                //find next segment for root
                match[0].atom.lock()->asignment.lock()->matched = false;
                match[0].discardedAtoms.clear();
                auto nextSegment = weak_ptr<LineWrap>();
                int j = 1;
                for (auto & seg : sample) {
                    if (seg.lock() == match[0].atom.lock()->asignment.lock()) {
                        if (j < sample.size())
                            nextSegment = *(&seg + 1);
                        break;
                    }
                    j++;
                }

                if (nextSegment.expired()) {
                    //if we have tried all segments as roots then
                    // it is non match
                    isModelMatched = false;
                    i = AtomsSize;
                    break;
                }
                else { //we found next segment for tree
                    match[0].atom.lock()->asignment = nextSegment;
                    nextSegment.lock()->matched = true;
                    i++;
                }
            }

            auto nextAtom = weak_ptr<Atom>{};
            tie(nextAtom, furthestPart) = FindAtom(match);
            if (nextAtom.expired()) {
                if (i > 1) {
                    match[i - 2].discardedSegments.
                        push_back(match[i - 1].atom.lock()->asignment);
                    match[i - 1].atom.lock()->asignment.lock()->matched = false;
                    match[i - 1].atom.lock()->asignment.reset();
                    match.pop_back();
                }

                i--;
                continue;
            }

            auto nextSegment = FindSegment(sample,
                match[i - 1].discardedSegments, *nextAtom.lock());
            if (nextSegment.expired()) {
                match[i - 1].discardedAtoms.push_back(nextAtom);
                match[i - 1].discardedSegments.clear();
                continue;
            }

            nextSegment.lock().get()->matched = true;
            nextAtom.lock().get()->asignment = nextSegment;
            match.push_back(TreeNode{ nextAtom });

            i++;
        }
        if (!isModelMatched) {
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
