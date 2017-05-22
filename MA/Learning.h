//
// Created by nietup on 22.05.17.
//

#ifndef DEGREE_LEARNING_H
#define DEGREE_LEARNING_H

enum BoolPlus {YES, NO, DNC};

using Hypothesis = vector<vector<BoolPlus>>;

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
            if (threshold < constraints[j]->operator()(
                *sample[segsPair.first].lock(),
                *sample[segsPair.second].lock())) {
                hypothesis[i][j] = YES;
            }
        }
    }

    return hypothesis;
}

//returns false if cannot be further specialized
bool Specialize(Hypothesis & h, const Hypothesis & s, vector<Hypothesis> & g,
                const Hypothesis & counterexample) {

    //chose right DNC
    auto pairCount = h.size();
    auto constraintCount = h[0].size();
    auto DNCi = -1;
    auto DNCj = -1;
    auto shouldBreak = false;
    for(auto i = 0; i < pairCount; i++) {
        for (auto j = 0; j < constraintCount; j++) {
            if (DNC == h[i][j]) {
                if (s[i][j] == counterexample[i][j] || DNC == s[i][j]) {
                    shouldBreak = true;
                }
                else {
                    for (auto &otherHypothesis : g) {
                        if (DNC != otherHypothesis[i][j]) {
                            shouldBreak = true;
                            break;
                        }
                    }
                }
                if (shouldBreak) {
                    shouldBreak = false;
                }
                else {
                    DNCi = i;
                    DNCj = j;
                    i = pairCount;
                    j = constraintCount;
                }
            }
        }
    }

    if (-1 == DNCi)
        return false;

    h[DNCi][DNCj] = (YES == counterexample[DNCi][DNCj]) ? NO : YES;
    //we must add multiple specializations!

    return true;
}

unique_ptr<SModel> GenerateModel(
    const vector<vector<weak_ptr<LineWrap>>> & positiveSamples,
    const vector<vector<weak_ptr<LineWrap>>> & negativeSamples,
    const vector<shared_ptr<Constraint>> & constraints) {
    /*Algorytm CEA

    input:
        samples = vector<vector<lineSegment>>
        n = liczba atomów w modelu (samples[0].size) dla uproszczenia na razie
        cosntraints = vector<Constraint>
        s = matrix<bool+don't care>[constraints.size()][0.5*n*(n-1)]
            {first positive sample}
        g = vector<matrix<bool+don't care>[constraints.size()][0.5*n*(n-1)]>

    algorithm:
        foreach sample in samples do
            if sample is positive then
                if !consistent with s on s[x][y] then
                    s[x][y] = don't care
                end
                if !consistent with s on g[i] then
                    g.del(i)
                end
            else
                foreach g[i] that is consistent with sample do
                    change one don't care in g to sth that would reject sample
                    this field can't already be used in another hipothesis in g
                    //if several spetializations possible do several???
                    //if no spetialization possible g.del(i)???
                end
            end
        end
    */
    const auto atomCount = 4;//samples[0].size();
    const auto pairCount = (uint)(0.5*atomCount*(atomCount-1));
    const auto constraintCount = 3;//constraints.size();

    //first - pair id, second - constraint in pair
    auto s = Hypothesis(
        pairCount,
        vector<BoolPlus>(constraintCount, NO)
    );

    auto g = vector<Hypothesis>{
        Hypothesis(
            pairCount,
            vector<BoolPlus>(constraintCount, DNC)
        )
    };

    for (const auto & sample : positiveSamples) {
        //we need to prepare sample in our format
        auto const extract = Extract(sample, pairCount, constraints);

        for (auto i = 0; i < pairCount; i++) {
            for (auto j = 0; j < constraintCount; j++) {
                if (extract[i][j] != s[i][j]) {
                    s[i][j] = DNC;
                }
            }
        }

        for (auto & hypothesis : g) {
            if (!Consistent(hypothesis, extract)) {
                g.erase(find(g.begin(), g.end(), hypothesis));
            }
        }
    }

    for (const auto & sample : negativeSamples) {
        //we need to prepare sample in our format
        auto const extract = Extract(sample, pairCount, constraints);

        for (auto & hypothesis : g) {
            if (Consistent(hypothesis, extract)) {
                if(!Specialize(hypothesis, s, g, extract)) {
                    g.erase(find(g.begin(), g.end(), hypothesis));
                }
            }
        }
    }

    auto model = unique_ptr<SModel>(new SModel);
    return model;
}

#endif //DEGREE_LEARNING_H
