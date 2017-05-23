auto testConstraint = make_shared<Constraint>([](const LineWrap & a,
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

        //sample n0
        if (2 == a.start.first && 0 == a.start.second &&
            2 == b.start.first && 1 == b.start.second)
            return 1.0;

        if (2 == a.start.first && 1 == a.start.second &&
            2 == b.start.first && 2 == b.start.second)
            return 0.0;

        if (2 == a.start.first && 0 == a.start.second &&
            2 == b.start.first && 2 == b.start.second)
            return 1.0;

        //sample n1
        if (3 == a.start.first && 0 == a.start.second &&
            3 == b.start.first && 1 == b.start.second)
            return 0.0;

        if (3 == a.start.first && 1 == a.start.second &&
            3 == b.start.first && 2 == b.start.second)
            return 1.0;

        if (3 == a.start.first && 0 == a.start.second &&
            3 == b.start.first && 2 == b.start.second)
            return 1.0;
    });

    auto constraints = vector<shared_ptr<Constraint>>{testConstraint};

    //positive sample 1
    auto l00 = make_shared<LineWrap>(LineWrap{{0,0},{0,0}});
    auto l01 = make_shared<LineWrap>(LineWrap{{0,1},{0,0}});
    auto l02 = make_shared<LineWrap>(LineWrap{{0,2},{0,0}});
    auto p0 = vector<weak_ptr<LineWrap>>{l00, l01, l02};

    //positive sample 2
    auto l10 = make_shared<LineWrap>(LineWrap{{1,0},{0,0}});
    auto l11 = make_shared<LineWrap>(LineWrap{{1,1},{0,0}});
    auto l12 = make_shared<LineWrap>(LineWrap{{1,2},{0,0}});
    auto p1 = vector<weak_ptr<LineWrap>>{l10, l11, l12};

    //negative sample 1
    auto l20 = make_shared<LineWrap>(LineWrap{{2,0},{0,0}});
    auto l21 = make_shared<LineWrap>(LineWrap{{2,1},{0,0}});
    auto l22 = make_shared<LineWrap>(LineWrap{{2,2},{0,0}});
    auto n0 = vector<weak_ptr<LineWrap>>{l20, l21, l22};

    //negative sample 2
    auto l30 = make_shared<LineWrap>(LineWrap{{3,0},{0,0}});
    auto l31 = make_shared<LineWrap>(LineWrap{{3,1},{0,0}});
    auto l32 = make_shared<LineWrap>(LineWrap{{3,2},{0,0}});
    auto n1 = vector<weak_ptr<LineWrap>>{l30, l31, l32};

    auto posSamples = vector<vector<weak_ptr<LineWrap>>>{p0, p1};
    auto negSamples = vector<vector<weak_ptr<LineWrap>>>{n0, n1};

/* test case 1
     * constraints = 1: A
     * atoms = 3
     * parts = 1: atom0, atom2, A
     *
     * positive samples:
     * sample #0
     * pair0: line0, line1: A
     * pair1: line1, line2: ~A
     * pair2: line0, line2: A
     *
     * sample #1
     * pair0: line0, line1: A
     * pair1: line1, line2: A
     * pair2: line0, line2: A
     *
     * negative samples:
     * sample #0
     * pair0: line0, line1: ~A
     * pair1: line1, line2: A
     * pair2: line0, line2: ~A
     *
     * sample #1
     * pair0: line0, line1: A
     * pair1: line1, line2: ~A
     * pair2: line0, line2: ~A
     *
     * expected output
     * S: <<YES>, <DNC>, <YES>>
     * G: <<<DNC>, <DNC>, <DNC>>>
     */
