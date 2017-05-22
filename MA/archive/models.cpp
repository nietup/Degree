

    auto model = SModel{};

    /*
     * foremny Triangle model
     */

    /*auto a1 = make_shared<Atom>(Atom{"1"});
    auto a2 = make_shared<Atom>(Atom{"2"});
    auto a3 = make_shared<Atom>(Atom{"3"});

    auto v = vector<weak_ptr<Constraint>>{
        weak_ptr<Constraint>(angle60),
        weak_ptr<Constraint>(adjacent),
        weak_ptr<Constraint>(sizeMatch)
    };

    auto p1 = make_shared<Part>(Part{{a1, a2}, v});
    auto p2 = make_shared<Part>(Part{{a1, a3}, v});
    auto p3 = make_shared<Part>(Part{{a2, a3}, v});

    a1.get()->involved.push_back(weak_ptr<Part>(p1));
    a1.get()->involved.push_back(weak_ptr<Part>(p2));
    a2.get()->involved.push_back(weak_ptr<Part>(p1));
    a2.get()->involved.push_back(weak_ptr<Part>(p3));
    a3.get()->involved.push_back(weak_ptr<Part>(p2));
    a3.get()->involved.push_back(weak_ptr<Part>(p3));

    model.atoms.push_back(a1);
    model.atoms.push_back(a2);
    model.atoms.push_back(a3);

    model.parts.push_back(p1);
    model.parts.push_back(p2);
    model.parts.push_back(p3);*/

    /*
     * general Triangle model
     */

    auto a1 = make_shared<Atom>(Atom{"1"});
    auto a2 = make_shared<Atom>(Atom{"2"});
    auto a3 = make_shared<Atom>(Atom{"3"});

    auto v = vector<weak_ptr<Constraint>>{
        weak_ptr<Constraint>(adjacent),
        weak_ptr<Constraint>(notParallel)
    };

    auto vs = vector<weak_ptr<Constraint>>{
        weak_ptr<Constraint>(adjacentStart),
        weak_ptr<Constraint>(notParallel)
    };

    auto ve = vector<weak_ptr<Constraint>>{
        weak_ptr<Constraint>(adjacentEnd),
        weak_ptr<Constraint>(notParallel)
    };

    auto p1 = make_shared<Part>(Part{{a1, a2}, vs});
    auto p2 = make_shared<Part>(Part{{a1, a3}, ve});
    auto p3 = make_shared<Part>(Part{{a2, a3}, v});

    a1.get()->involved.push_back(weak_ptr<Part>(p1));
    a1.get()->involved.push_back(weak_ptr<Part>(p2));
    a2.get()->involved.push_back(weak_ptr<Part>(p1));
    a2.get()->involved.push_back(weak_ptr<Part>(p3));
    a3.get()->involved.push_back(weak_ptr<Part>(p2));
    a3.get()->involved.push_back(weak_ptr<Part>(p3));

    model.atoms.push_back(a1);
    model.atoms.push_back(a2);
    model.atoms.push_back(a3);

    model.parts.push_back(p1);
    model.parts.push_back(p2);
    model.parts.push_back(p3);

    /*
     * square model
     */

    /*auto a1 = make_shared<Atom>(Atom{"1"});
    auto a2 = make_shared<Atom>(Atom{"2"});
    auto a3 = make_shared<Atom>(Atom{"3"});
    auto a4 = make_shared<Atom>(Atom{"4"});

    auto closeEdges = vector<weak_ptr<Constraint>>{
        weak_ptr<Constraint>(adjacent),
        weak_ptr<Constraint>(sizeMatch),
        weak_ptr<Constraint>(perpendicular)
    };

    auto farEdges = vector<weak_ptr<Constraint>>{
        weak_ptr<Constraint>(parallel),
        weak_ptr<Constraint>(sizeMatch),
        weak_ptr<Constraint>(far)
    };

    auto p1 = make_shared<Part>(Part{{a1, a2}, closeEdges});
    auto p2 = make_shared<Part>(Part{{a1, a3}, farEdges});
    auto p3 = make_shared<Part>(Part{{a1, a4}, closeEdges});
    auto p4 = make_shared<Part>(Part{{a2, a3}, closeEdges});
    auto p5 = make_shared<Part>(Part{{a2, a4}, farEdges});
    auto p6 = make_shared<Part>(Part{{a3, a4}, closeEdges});

    a1.get()->involved.push_back(weak_ptr<Part>(p1));
    a1.get()->involved.push_back(weak_ptr<Part>(p2));
    a1.get()->involved.push_back(weak_ptr<Part>(p3));
    a2.get()->involved.push_back(weak_ptr<Part>(p1));
    a2.get()->involved.push_back(weak_ptr<Part>(p4));
    a2.get()->involved.push_back(weak_ptr<Part>(p5));
    a3.get()->involved.push_back(weak_ptr<Part>(p2));
    a3.get()->involved.push_back(weak_ptr<Part>(p4));
    a3.get()->involved.push_back(weak_ptr<Part>(p6));
    a4.get()->involved.push_back(weak_ptr<Part>(p3));
    a4.get()->involved.push_back(weak_ptr<Part>(p5));
    a4.get()->involved.push_back(weak_ptr<Part>(p6));

    model.atoms.push_back(a1);
    model.atoms.push_back(a2);
    model.atoms.push_back(a3);
    model.atoms.push_back(a4);

    model.parts.push_back(p1);
    model.parts.push_back(p2);
    model.parts.push_back(p3);
    model.parts.push_back(p4);
    model.parts.push_back(p5);
    model.parts.push_back(p6);*/

if (Match(model, segments))
        for (auto &a : model.atoms) {
            cout << a->name << " <-> ("
                 << a.get()->asignment.lock().get()->start.first << ", "
                 << a.get()->asignment.lock().get()->start.second << ") ("
                 << a.get()->asignment.lock().get()->end.first << ", "
                 << a.get()->asignment.lock().get()->end.second << ")"
                 << endl;

            auto detection = vector<LineSegment>();
            for (auto &a : model.atoms)
                detection.push_back(a->asignment.lock()->GetLineSegment());

            string detectionFile = inFile + ".detection.svg";
            SvgWriterInterface::Ptr svg2(new ElsdSvgWriter);
            svg2->setImageSize(image->xsize(), image->ysize());
            svg2->addLineSegments(detection.begin(), detection.end());
            ofs.open(detectionFile);
            ofs << *svg2 << "</svg>";
            ofs.close();
        }
    else {
        cout << "Non match" << endl;
    }
