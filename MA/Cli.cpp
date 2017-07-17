//
// Created by nietup on 13.07.17.
//

#include "Cli.h"

//-----------------------------------------------------------------------------
void TestMatching() {
    auto sizeMatch = [](const LineWrap &a,
                        const LineWrap &b) {
        double lenA = a.Length(),
            lenB = b.Length();
        return lenA < lenB ? 1 - (lenA / lenB) : 1 - (lenB / lenA);
    };

    auto angle60 = [](const LineWrap &a,
                      const LineWrap &b) {
        return 2 * abs(0.5 - abs(a.GetCos(b)));
    };

    auto adjacent = [](const LineWrap &a,
                       const LineWrap &b) {
        double d = a.Distance(b);
        return d / (d + 300);
    };

    auto model = Model{};

    auto a1 = make_shared<Vertex>(Vertex{});
    auto a2 = make_shared<Vertex>(Vertex{});
    auto a3 = make_shared<Vertex>(Vertex{});

    model.constraints = vector<shared_ptr<Constraint>>(3);
    model.constraints[0] = make_shared<Constraint>(angle60);
    model.constraints[1] = make_shared<Constraint>(adjacent);
    model.constraints[2] = make_shared<Constraint>(sizeMatch);

    auto p1 = make_shared<Edge>(Edge{{a1, a2}, {YES, YES, YES}});
    auto p2 = make_shared<Edge>(Edge{{a1, a3}, {YES, YES, YES}});
    auto p3 = make_shared<Edge>(Edge{{a2, a3}, {YES, YES, YES}});

    a1.get()->involved.push_back(weak_ptr<Edge>(p1));
    a1.get()->involved.push_back(weak_ptr<Edge>(p2));
    a2.get()->involved.push_back(weak_ptr<Edge>(p1));
    a2.get()->involved.push_back(weak_ptr<Edge>(p3));
    a3.get()->involved.push_back(weak_ptr<Edge>(p2));
    a3.get()->involved.push_back(weak_ptr<Edge>(p3));

    model.vertices.push_back(a1);
    model.vertices.push_back(a2);
    model.vertices.push_back(a3);

    model.edges.push_back(p1);
    model.edges.push_back(p2);
    model.edges.push_back(p3);

    auto inFile = string{"a6.PGM"};
    inFile = "./" + inFile;
    ImageInterface::Ptr image(new ElsdPgmFileReader(inFile));
    ShapesDetectorInterface::Ptr detector(new ElsDetector);
    detector->run(image);

    string outFile = inFile + ".svg";
    SvgWriterInterface::Ptr svg(new ElsdSvgWriter);
    svg->setImageSize(image->xsize(), image->ysize());
    const vector<LineSegment> &lines = detector->getLineSegments();
    svg->addLineSegments(lines.begin(), lines.end());
    ofstream ofs(outFile, ofstream::out);
    ofs << *svg
        << "</svg>";
    ofs.close();

    //Create vector of wrapped segments
    auto segsShared = vector<shared_ptr<LineWrap>>();
    for (const auto &line : lines)
        segsShared.push_back(make_shared<LineWrap>(line));

    auto segments = vector<weak_ptr<LineWrap>>();
    for (const auto &line : segsShared)
        segments.push_back(weak_ptr<LineWrap>(line));

    auto matcher = Matcher();

    if (matcher.Match(model, segments).first)
        for (auto &a : model.vertices) {
            cout << " <-> ("
                 << a.get()->asignment.lock().get()->start.first << ", "
                 << a.get()->asignment.lock().get()->start.second << ") ("
                 << a.get()->asignment.lock().get()->end.first << ", "
                 << a.get()->asignment.lock().get()->end.second << ")"
                 << endl;

            auto detection = vector<LineSegment>();
            for (auto &a : model.vertices)
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
}

//-----------------------------------------------------------------------------
Cli::Cli() {
    //sample model
    auto sizeMatch = [](const LineWrap &a,
                        const LineWrap &b) {
        double lenA = a.Length();
        double lenB = b.Length();
        cout << (lenA < lenB ? 1 - (lenA / lenB) : 1 - (lenB / lenA)) << "\n";
        return lenA < lenB ? 1 - (lenA / lenB) : 1 - (lenB / lenA);
    };

    auto angle60 = [](const LineWrap &a,
                      const LineWrap &b) {
        return 2 * abs(0.5 - abs(a.GetCos(b)));
    };

    auto adjacent = [](const LineWrap &a,
                       const LineWrap &b) {
        double d = a.Distance(b);
        return d / (d + 300);
    };

    //auto model = Model{};

    auto a1 = make_shared<Vertex>(Vertex{});
    auto a2 = make_shared<Vertex>(Vertex{});
    auto a3 = make_shared<Vertex>(Vertex{});

    model.constraints = vector<shared_ptr<Constraint>>();
    model.constraints.push_back(make_shared<Constraint>(angle60));
    model.constraints.push_back(make_shared<Constraint>(adjacent));
    model.constraints.push_back(make_shared<Constraint>(sizeMatch));

    auto p1 = make_shared<Edge>(Edge{{a1, a2}, {YES, YES, YES}});
    auto p2 = make_shared<Edge>(Edge{{a1, a3}, {YES, YES, YES}});
    auto p3 = make_shared<Edge>(Edge{{a2, a3}, {YES, YES, YES}});

    a1.get()->involved.push_back(weak_ptr<Edge>(p1));
    a1.get()->involved.push_back(weak_ptr<Edge>(p2));
    a2.get()->involved.push_back(weak_ptr<Edge>(p1));
    a2.get()->involved.push_back(weak_ptr<Edge>(p3));
    a3.get()->involved.push_back(weak_ptr<Edge>(p2));
    a3.get()->involved.push_back(weak_ptr<Edge>(p3));

    model.vertices.push_back(a1);
    model.vertices.push_back(a2);
    model.vertices.push_back(a3);

    model.edges.push_back(p1);
    model.edges.push_back(p2);
    model.edges.push_back(p3);
}

//-----------------------------------------------------------------------------
void Cli::Run() {
    SelectMode();
    SelectModel();
    ShowModel();

    if (mode == TEST) {
        SelectTestingSamples();
        Test();
    }
    else if (mode == LEARN) {
        SelectLearningSamples();
        Learn();
        SaveModel();
    }
}

//-----------------------------------------------------------------------------
void Cli::SelectMode() {
    cout << "\nProsze wybrac tryb dzialania programu:" << endl
         << "(1) Testowanie modelu" << endl
         << "(2) Trenowanie modelu" << endl;

    int response = -1;

    while (1 != response and 2 != response) {
        cin >> response;

        if (1 == response) {
            mode = TEST;
        }
        if (2 == response) {
            mode = LEARN;
        }
    }
}

//-----------------------------------------------------------------------------
void Cli::SelectModel() {
    if (mode == LEARN)
        cout << "\nProsze wprowadzic sciezke do modelu lub wpisac 'nowy':\n";
    else
        cout << "\nProsze wprowadzic sciezke do modelu:\n";

    string response;
    cin >> response;

    if (response == "nowy") {
        CreateModel();
    }
    else {
        pathToModel = response;
        LoadModel();
    }
}

//-----------------------------------------------------------------------------
void Cli::ShowModel() {
    cout << "\nModel:\n";
    for (auto & edge : model.edges) {
        //get indices of vertices
        auto index = 0;
        auto verticesNo = model.vertices.size();
        for (; index < verticesNo; index++) {
            if (model.vertices[index] == edge->vertices.first.lock()) {
                break;
            }
        }

        cout << index << ",";

        index = 0;
        for (; index < verticesNo; index++) {
            if (model.vertices[index] == edge->vertices.second.lock()) {
                break;
            }
        }

        cout << index << ";";

        for (auto & cons : edge->constraints) {
            cout << cons << ",";
        }
        cout << "\n";
    }
}

//-----------------------------------------------------------------------------
void Cli::SelectLearningSamples() {
    cout << "\nProsze wprowadzic sciezke folderu z pozytywnymi przykladami uczacymi:\n";
    string response;
    cin >> response;
    pathToPos = response;

    cout << "\nProsze wprowadzic sciezke folderu z negatywnymi przykladami uczacymi:\n";
    cin >> response;
    pathToNeg = response;
    //TODO wczytac probki do wektora
}

//-----------------------------------------------------------------------------
void Cli::SelectTestingSamples() {
    cout << "\nProsze wprowadzic sciezke przykladu testowego:\n";
    string response;
    cin >> response;
    pathToTest = response;

    /*//create vector of files
    auto files = vector<string>();
    GetFilesInDirectory(files, pathToTest);*/

    //for (auto inFile : files) {
        //cout << "\nPlik " << inFile;
        ImageInterface::Ptr image(new ElsdPgmFileReader(pathToTest));
        ShapesDetectorInterface::Ptr detector(new ElsDetector());
        detector->run(image);

        const vector<LineSegment> &lines = detector->getLineSegments();
        string outFile = pathToTest + ".svg";
        SvgWriterInterface::Ptr svg(new ElsdSvgWriter);
        svg->setImageSize(image->xsize(), image->ysize());
        svg->addLineSegments(lines.begin(), lines.end());
        ofstream ofs(outFile, ofstream::out);
        ofs << *svg
            << "</svg>";
        ofs.close();

        //Create vector of wrapped segments
        testingSamples.push_back(vector<shared_ptr<LineWrap>>());
        for (const auto &line : lines)
            testingSamples[0].push_back(make_shared<LineWrap>(line));
    //}
}

//-----------------------------------------------------------------------------
void Cli::SaveModel() {
    string outFile = pathToModel;
    ofstream ofs(outFile, ofstream::out);

    for (auto & edge : model.edges) {
        //get indices of vertices
        auto index = 0;
        auto verticesNo = model.vertices.size();
        for (; index < verticesNo; index++) {
            if (model.vertices[index] == edge->vertices.first.lock()) {
                break;
            }
        }

        ofs << index << ",";

        index = 0;
        for (; index < verticesNo; index++) {
            if (model.vertices[index] == edge->vertices.second.lock()) {
                break;
            }
        }

        ofs << index << ";";

        for (auto & cons : edge->constraints) {
            ofs << cons << ",";
        }
        ofs << "\n";
    }

    ofs.close();

    cout << "\nModel zapisano do: " << pathToModel;
}

//-----------------------------------------------------------------------------
void Cli::Test() {
    auto segments = vector<weak_ptr<LineWrap>>();
    for (const auto &line : testingSamples[0])
        segments.push_back(weak_ptr<LineWrap>(line));

    auto matcher = Matcher();

    if (matcher.Match(model, segments).first) {
        for (auto &a : model.vertices) {
            cout << " <-> ("
                 << a.get()->asignment.lock().get()->start.first << ", "
                 << a.get()->asignment.lock().get()->start.second << ") ("
                 << a.get()->asignment.lock().get()->end.first << ", "
                 << a.get()->asignment.lock().get()->end.second << ")"
                 << endl;

            /*auto detection = vector<LineSegment>();
            for (auto &a : model.vertices)
                detection.push_back(a->asignment.lock()->GetLineSegment());

            string detectionFile = pathToTest + ".detection.svg";
            SvgWriterInterface::Ptr svg2(new ElsdSvgWriter);
            svg2->setImageSize(image->xsize(), image->ysize());
            svg2->addLineSegments(detection.begin(), detection.end());
            ofs.open(detectionFile);
            ofs << *svg2 << "</svg>";
            ofs.close();*/
        }
    }
    else {
        cout << "\nNon match" << endl;
    }
}

//-----------------------------------------------------------------------------
void Cli::Learn() {
    //function scoring size match
    //* returns double in range [0, 1], where 0 is the best match
    auto sizeMatch = make_shared<Constraint>([](const LineWrap &a,
                                                const LineWrap &b) {
        double lenA = a.Length(),
            lenB = b.Length();
        /*cout << "size match: "
             << (lenA < lenB ? 1 - (lenA / lenB) : 1 - (lenB / lenA))
             << endl;*/
        return lenA < lenB ? 1 - (lenA / lenB) : 1 - (lenB / lenA);
    });

    //function scoring perpendicularity between lines
    //* returns double in range [0, 1], where 0 is the best match
    auto perpendicular = make_shared<Constraint>([](const LineWrap &a,
                                                    const LineWrap &b) {
        return abs(a.GetCos(b));
    });

    //function scoring parallelity between lines
    //* returns double in range [0, 1], where 0 is the best match
    auto parallel = make_shared<Constraint>([](const LineWrap &a,
                                               const LineWrap &b) {
        return 1.0 - abs(a.GetCos(b));
    });

    auto notParallel = make_shared<Constraint>([](const LineWrap &a,
                                                  const LineWrap &b) {
        return 0.9 < abs(a.GetCos(b)) ? 1.0 : 0.0;
    });

    //function scoring closeness to 60 degrees between lines
    //* returns double in range [0, 1], where 0 is the best match
    auto angle60 = make_shared<Constraint>([](const LineWrap &a,
                                              const LineWrap &b) {

        return 2 * abs(0.5 - abs(a.GetCos(b)));
    });

    //function scoring adjacency between lines
    //* returns double in range [0, 1], where 0 is the best match
    auto adjacent = make_shared<Constraint>([](const LineWrap &a,
                                               const LineWrap &b) {
        double d = a.Distance(b);
        return d / (d + 300);
    });

    //function opposite to adjacent
    //* returns double in range [0, inf), where inf is touching
    auto far = make_shared<Constraint>([](const LineWrap & a,
                                          const LineWrap & b) {
        return 10.0 / a.Distance(b);
    });

    //function scoring adjacency between start of line a and line b
    //* returns double in range [0, 1], where 0 is the best match
    auto adjacentStart = make_shared<Constraint>([](const LineWrap & a,
                                                    const LineWrap & b) {
        auto d = b.Distance(a.start);
        return d / (d+300.0);
    });

    //function scoring adjacency between end  of line a and line b
    //* returns double in range [0, 1], where 0 is the best match
    auto adjacentEnd = make_shared<Constraint>([](const LineWrap & a,
                                                  const LineWrap & b) {
        auto d = b.Distance(a.end);
        return d / (d+300.0);
    });

    //function scoring adjacency between vertices of line a b
    //* returns double in range [0, 1], where 0 is the best match
    auto vertexAdjacent = make_shared<Constraint>([](const LineWrap & a,
                                                     const LineWrap & b) {
        auto a1 = b.Distance(a.start);
        auto a2 = b.Distance(a.end);
        a1 = a1 < a2 ? a1 : a2;

        auto b1 = a.Distance(b.start);
        auto b2 = a.Distance(b.end);
        b1 = b1 < b2 ? b1 : b2;

        a1 = a1 < b1 ? a1 : b1;

        return a1 / (a1 + 300.0);
    });

    /*
     * ELSD image processing
     * detect segments and save them in separate image for testing purposes
     */
    //TODO make this work
    auto samples = vector<vector<weak_ptr<LineWrap>>>{};

    auto sampleFiles = vector<string>{/*"l1.PGM","l2.PGM","l3.PGM","l4.PGM"*/};
    for (auto & sampleFile : sampleFiles) {
        auto inFile = sampleFile;
        inFile = "./" + inFile;
        ImageInterface::Ptr image(new ElsdPgmFileReader(inFile));
        ShapesDetectorInterface::Ptr detector(new ElsDetector);
        detector->run(image);

        string outFile = inFile + ".svg";
        SvgWriterInterface::Ptr svg(new ElsdSvgWriter);
        svg->setImageSize(image->xsize(), image->ysize());
        const vector<LineSegment> &lines = detector->getLineSegments();
        svg->addLineSegments(lines.begin(), lines.end());
        ofstream ofs(outFile, ofstream::out);
        ofs << *svg
            << "</svg>";
        ofs.close();

        //Create vector of wrapped segments
        auto segsShared = vector<shared_ptr<LineWrap>>();
        for (const auto &line : lines)
            segsShared.push_back(make_shared<LineWrap>(line));

        auto segments = vector<weak_ptr<LineWrap>>();
        for (const auto &line : segsShared)
            segments.push_back(weak_ptr<LineWrap>(line));

        samples.push_back(segments);
    }

    auto constraints = vector<shared_ptr<Constraint>>{adjacent, sizeMatch,
                                                      parallel, perpendicular};

    auto l00 = make_shared<LineWrap>(LineWrap{{0,0},{100,0}});
    auto l01 = make_shared<LineWrap>(LineWrap{{100,100},{100,0}});
    auto l02 = make_shared<LineWrap>(LineWrap{{0,0},{0,100}});
    auto l03 = make_shared<LineWrap>(LineWrap{{0,100},{100,100}});
    auto p0 = vector<weak_ptr<LineWrap>>{l00, l01, l02, l03};

    auto l10 = make_shared<LineWrap>(LineWrap{{0,0},{500,0}});
    auto l11 = make_shared<LineWrap>(LineWrap{{500,100},{500,0}});
    auto l12 = make_shared<LineWrap>(LineWrap{{0,0},{0,100}});
    auto l13 = make_shared<LineWrap>(LineWrap{{0,100},{500,100}});
    auto p1 = vector<weak_ptr<LineWrap>>{l10, l11, l12, l13};

    auto l20 = make_shared<LineWrap>(LineWrap{{0,0},{100,0}});
    auto l21 = make_shared<LineWrap>(LineWrap{{120,97.9797},{100,0}});
    auto l22 = make_shared<LineWrap>(LineWrap{{0,0},{20,97.9797}});
    auto l23 = make_shared<LineWrap>(LineWrap{{20,97.9797},{120,97.9797}});
    auto p2 = vector<weak_ptr<LineWrap>>{l20, l21, l22, l23};

    auto l30 = make_shared<LineWrap>(LineWrap{{0,0},{100,0}});
    auto l31 = make_shared<LineWrap>(LineWrap{{0,0},{0,100}});
    auto l32 = make_shared<LineWrap>(LineWrap{{0,100},{100,0}});
    auto l33 = make_shared<LineWrap>(LineWrap{{0,0},{50,50}});
    auto n0 = vector<weak_ptr<LineWrap>>{l30, l31, l32, l33};

    auto posSamples = vector<vector<weak_ptr<LineWrap>>>{p0, p1, p2};
    auto negSamples = vector<vector<weak_ptr<LineWrap>>>{n0};

    LearningSystem ls;
    ls.GenerateModel(posSamples, negSamples, constraints);
}

//-----------------------------------------------------------------------------
void Cli::CreateModel() {
    cout << "\nPodaj sciezke zapisu nowego modelu:\n";
    string response;
    cin >> response;
    pathToModel = response;

    model = Model();
}

//-----------------------------------------------------------------------------
void Cli::LoadModel() {
    //TODO later wczytac model z pathToModel
}

//-----------------------------------------------------------------------------
void Cli::GetFilesInDirectory(std::vector<string> &out,
    const string &directory) {

    DIR *dir;
    class dirent *ent;
    class stat st;

    dir = opendir(directory.c_str());
    while ((ent = readdir(dir)) != NULL) {
        const string file_name = ent->d_name;
        const string full_file_name = directory + "/" + file_name;

        if (file_name[0] == '.')
            continue;

        if (stat(full_file_name.c_str(), &st) == -1)
            continue;

        const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        if (is_directory)
            continue;

        out.push_back(full_file_name);
    }
    closedir(dir);

}