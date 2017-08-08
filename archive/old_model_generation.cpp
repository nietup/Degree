//some old test case functions for constraints (these were bad fo new test 
//because it assumes that lines are segmented in the "right" order
/* test case 2
     * constraints = 3: c0, c1, c2
     * atoms = 3
     * parts = 3:
     * MODEL
     * 1 ? 1
     * ? ? 1
     * 1 ? ?
     *
     * positive samples:
     * sample #0
     * pair0: line0, line1: 1 1 0
     * pair1: line1, line2: 0 0 1
     * pair2: line0, line2: 1 0 1
     *
     * sample #1
     * pair0: line0, line1: 1 1 0
     * pair1: line1, line2: 1 0 1
     * pair2: line0, line2: 1 1 1
     *
     * sample #2
     * pair0: line0, line1: 1 0 0
     * pair1: line1, line2: 0 0 1
     * pair2: line0, line2: 1 0 1
     *
     * sample #4
     * pair0: line0, line1: 1 1 1
     * pair1: line1, line2: 1 1 1
     * pair2: line0, line2: 1 1 1
     *
     * negative samples:
     */

    auto c0 = make_shared<Constraint>([](const LineWrap & a,
                                                     const LineWrap & b) {
        //sample p0
        if (0 == a.start.first && 0 == a.start.second &&
            0 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (0 == a.start.first && 1 == a.start.second &&
            0 == b.start.first && 2 == b.start.second)
            return 1.0;

        if (0 == a.start.first && 0 == a.start.second &&
            0 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample p1
        if (1 == a.start.first && 0 == a.start.second &&
            1 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (1 == a.start.first && 1 == a.start.second &&
            1 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (1 == a.start.first && 0 == a.start.second &&
            1 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample p2
        if (2 == a.start.first && 0 == a.start.second &&
            2 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (2 == a.start.first && 1 == a.start.second &&
            2 == b.start.first && 2 == b.start.second)
            return 1.0;

        if (2 == a.start.first && 0 == a.start.second &&
            2 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample p3
        if (3 == a.start.first && 0 == a.start.second &&
            3 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (3 == a.start.first && 1 == a.start.second &&
            3 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (3 == a.start.first && 0 == a.start.second &&
            3 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample n0
        if (4 == a.start.first && 0 == a.start.second &&
            4 == b.start.first && 1 == b.start.second)
            return 1.0;

        if (4 == a.start.first && 1 == a.start.second &&
            4 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (4 == a.start.first && 0 == a.start.second &&
            4 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample n1
        if (5 == a.start.first && 0 == a.start.second &&
            5 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (5 == a.start.first && 1 == a.start.second &&
            5 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (5 == a.start.first && 0 == a.start.second &&
            5 == b.start.first && 2 == b.start.second)
            return 1.0;

        //sample n2
        if (6 == a.start.first && 0 == a.start.second &&
            6 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (6 == a.start.first && 1 == a.start.second &&
            6 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (6 == a.start.first && 0 == a.start.second &&
            6 == b.start.first && 2 == b.start.second)
            return 0.0;
    });

    auto c1 = make_shared<Constraint>([](const LineWrap & a,
                                         const LineWrap & b) {
        //sample p0
        if (0 == a.start.first && 0 == a.start.second &&
            0 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (0 == a.start.first && 1 == a.start.second &&
            0 == b.start.first && 2 == b.start.second)
            return 1.0;

        if (0 == a.start.first && 0 == a.start.second &&
            0 == b.start.first && 2 == b.start.second)
            return 1.0;

        //sample p1
        if (1 == a.start.first && 0 == a.start.second &&
            1 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (1 == a.start.first && 1 == a.start.second &&
            1 == b.start.first && 2 == b.start.second)
            return 1.0;

        if (1 == a.start.first && 0 == a.start.second &&
            1 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample p2
        if (2 == a.start.first && 0 == a.start.second &&
            2 == b.start.first && 1 == b.start.second)
            return 1.0;

        if (2 == a.start.first && 1 == a.start.second &&
            2 == b.start.first && 2 == b.start.second)
            return 1.0;

        if (2 == a.start.first && 0 == a.start.second &&
            2 == b.start.first && 2 == b.start.second)
            return 1.0;

        //sample p3
        if (3 == a.start.first && 0 == a.start.second &&
            3 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (3 == a.start.first && 1 == a.start.second &&
            3 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (3 == a.start.first && 0 == a.start.second &&
            3 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample n0
        if (4 == a.start.first && 0 == a.start.second &&
            4 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (4 == a.start.first && 1 == a.start.second &&
            4 == b.start.first && 2 == b.start.second)
            return 1.0;

        if (4 == a.start.first && 0 == a.start.second &&
            4 == b.start.first && 2 == b.start.second)
            return 1.0;

        //sample n1
        if (5 == a.start.first && 0 == a.start.second &&
            5 == b.start.first && 1 == b.start.second)
            return 1.0;

        if (5 == a.start.first && 1 == a.start.second &&
            5 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (5 == a.start.first && 0 == a.start.second &&
            5 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample n2
        if (6 == a.start.first && 0 == a.start.second &&
            6 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (6 == a.start.first && 1 == a.start.second &&
            6 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (6 == a.start.first && 0 == a.start.second &&
            6 == b.start.first && 2 == b.start.second)
            return 0.0;
    });

    auto c2 = make_shared<Constraint>([](const LineWrap & a,
                                         const LineWrap & b) {
        //sample p0
        if (0 == a.start.first && 0 == a.start.second &&
            0 == b.start.first && 1 == b.start.second)
            return 1.0;

        if (0 == a.start.first && 1 == a.start.second &&
            0 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (0 == a.start.first && 0 == a.start.second &&
            0 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample p1
        if (1 == a.start.first && 0 == a.start.second &&
            1 == b.start.first && 1 == b.start.second)
            return 1.0;

        if (1 == a.start.first && 1 == a.start.second &&
            1 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (1 == a.start.first && 0 == a.start.second &&
            1 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample p2
        if (2 == a.start.first && 0 == a.start.second &&
            2 == b.start.first && 1 == b.start.second)
            return 1.0;

        if (2 == a.start.first && 1 == a.start.second &&
            2 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (2 == a.start.first && 0 == a.start.second &&
            2 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample p3
        if (3 == a.start.first && 0 == a.start.second &&
            3 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (3 == a.start.first && 1 == a.start.second &&
            3 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (3 == a.start.first && 0 == a.start.second &&
            3 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample n0
        if (4 == a.start.first && 0 == a.start.second &&
            4 == b.start.first && 1 == b.start.second)
            return 1.0;

        if (4 == a.start.first && 1 == a.start.second &&
            4 == b.start.first && 2 == b.start.second)
            return 1.0;

        if (4 == a.start.first && 0 == a.start.second &&
            4 == b.start.first && 2 == b.start.second)
            return 0.0;

        //sample n1
        if (5 == a.start.first && 0 == a.start.second &&
            5 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (5 == a.start.first && 1 == a.start.second &&
            5 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (5 == a.start.first && 0 == a.start.second &&
            5 == b.start.first && 2 == b.start.second)
            return 1.0;

        //sample n2
        if (6 == a.start.first && 0 == a.start.second &&
            6 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (6 == a.start.first && 1 == a.start.second &&
            6 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (6 == a.start.first && 0 == a.start.second &&
            6 == b.start.first && 2 == b.start.second)
            return 1.0;
    });

//
/*PSEUDOCODE FOR THE OLD LEARNING
     * Algorytm CEA

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

		/*old new samle
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
        */

/*
    for (const auto & sample : negativeSamples) {
        //we need to prepare sample in our format
        auto const extract = Extract(sample, pairCount, constraints);

        auto willBeDeleted = vector<int>();
        auto willBeAdded = vector<Hypothesis>();
        for (auto & hypothesis : g) {
            if (Consistent(hypothesis, extract)) {
                auto spetializations = Specialize(hypothesis, s, g, extract);
                willBeAdded.insert(willBeAdded.end(), spetializations.begin(),
                    spetializations.end());

                //if there are some more general hypothesis than the new one
                //they should be deleted
                for (auto i = 0; i < g.size(); i++) {
                    for (auto j = 0; j < spetializations.size(); j++) {
                        if (MoreGeneralOrEqual(g[i], spetializations[j])) {
                            if (willBeDeleted.end() ==
                                find(willBeDeleted.begin(),
                                     willBeDeleted.end(), i)) {
                                willBeDeleted.push_back(i);
                            }
                        }
                    }
                }
            }
        }


        for (auto i : willBeDeleted) {
            g.erase(find(g.begin(), g.end(), g[i]));
        }
        g.insert(g.end(), willBeAdded.begin(), willBeAdded.end());
    }
    */
