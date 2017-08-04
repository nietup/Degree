//
// Created by nietup on 13.07.17.
//

#include "Cli.h"
#include <sstream>

//-----------------------------------------------------------------------------
Cli::Cli() {
    //sample model

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

    constraints.push_back(sizeMatch);       //0
    constraints.push_back(angle60);         //1
    constraints.push_back(adjacent);        //2
    constraints.push_back(perpendicular);   //3
    constraints.push_back(parallel);        //4
    constraints.push_back(notParallel);     //5
    constraints.push_back(far);             //6
    constraints.push_back(adjacentStart);   //7
    constraints.push_back(adjacentEnd);     //8
    constraints.push_back(vertexAdjacent);  //9

    auto a1 = make_shared<Vertex>(Vertex{});
    auto a2 = make_shared<Vertex>(Vertex{});
    auto a3 = make_shared<Vertex>(Vertex{});

    constraintArray = {0, 1, 2};

    model.constraints = vector<shared_ptr<Constraint>>();
    model.constraints.push_back(constraints[0]);
    model.constraints.push_back(constraints[1]);
    model.constraints.push_back(constraints[2]);

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
    cout << "\nProsze wprowadzic sciezke folderu z pozytywnymi przykladami "
        << "uczacymi:\n";
    string response;
    cin >> response;
    pathToPos = response;

    cout << "\nProsze wprowadzic sciezke folderu z negatywnymi przykladami "
        << "uczacymi:\n";
    cin >> response;
    pathToNeg = response;

    auto posfiles = vector<string>();
    GetFilesInDirectory(posfiles, pathToPos);

    for (auto file : posfiles) {
        ifstream infile (file);

        auto posSample = vector<shared_ptr<LineWrap>>{};

        string line;
        while (getline(infile, line)) {
            //if not starting with '<line' then skip
            string prefix("<line");
            if (!line.compare(0, prefix.size(), prefix)) {
                auto l = parseSVGLine(line);
                posSample.push_back(l);
            }
        }

        posSamples.push_back(posSample);
    }

    auto negfiles = vector<string>();
    GetFilesInDirectory(negfiles, pathToNeg);

    for (auto file : negfiles) {
        ifstream infile (file);

        auto negSample = vector<shared_ptr<LineWrap>>{};

        string line;
        while (getline(infile, line)) {
            //if not starting with '<line' then skip
            string prefix("<line");
            if (!line.compare(0, prefix.size(), prefix)) {
                auto l = parseSVGLine(line);
                negSample.push_back(l);;
            }
        }

        negSamples.push_back(negSample);
    }
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

        xsize = image->xsize();
        ysize = image->ysize();

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

    for (auto i : constraintArray) {
        ofs << i << " ";
    }
    ofs << "\n";

    for (auto & edge : model.edges) {
        //get indices of vertices
        auto index = 0;
        auto verticesNo = model.vertices.size();
        for (; index < verticesNo; index++) {
            if (model.vertices[index] == edge->vertices.first.lock()) {
                break;
            }
        }

        ofs << index << " ";

        index = 0;
        for (; index < verticesNo; index++) {
            if (model.vertices[index] == edge->vertices.second.lock()) {
                break;
            }
        }

        ofs << index << " ";

        for (auto & cons : edge->constraints) {
            ofs << cons << " ";
        }
        ofs << "\n";
    }

    //todo add saving G to file

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
            cout << " -> ("
                 << a.get()->asignment.lock().get()->start.first << ", "
                 << a.get()->asignment.lock().get()->start.second << ") ("
                 << a.get()->asignment.lock().get()->end.first << ", "
                 << a.get()->asignment.lock().get()->end.second << ")"
                 << endl;
        }
    }
    else {
        cout << "\nNon match" << endl;
    }
}

//-----------------------------------------------------------------------------
void Cli::Learn() {
    auto modelConstraints = vector<shared_ptr<Constraint>>();

    if (!constraintArray.size()) {
        cout << "\nBledny model, constraint array puste.";
    }

    for (auto i : constraintArray) {
        modelConstraints.push_back(constraints[i]);
    }

    auto wPosSamples = vector<vector<weak_ptr<LineWrap>>>();
    auto wNegSamples = vector<vector<weak_ptr<LineWrap>>>();

    for (auto & a : posSamples) {
        auto v = vector<weak_ptr<LineWrap>>();
        for (auto & b : a) {
            v.push_back(weak_ptr<LineWrap>{b});
        }
        wPosSamples.push_back(v);
    }
    for (auto & a : negSamples) {
        auto v = vector<weak_ptr<LineWrap>>();
        for (auto & b : a) {
            v.push_back(weak_ptr<LineWrap>{b});
        }
        wNegSamples.push_back(v);
    }

    LearningSystem ls;
    auto spmodel = shared_ptr<Model>{};
    auto spg = shared_ptr<Hypothesis>{};
    tie(spmodel, spg) = ls.GenerateModel(wPosSamples, wNegSamples, modelConstraints, model);
    model = *spmodel;
    g = *spg;
}

//-----------------------------------------------------------------------------
void Cli::CreateModel() {
    cout << "\nPodaj sciezke zapisu nowego modelu:\n";
    string response;
    cin >> response;
    pathToModel = response;

    model = Model();
    constraintArray.clear();

    cout << "Wybierz ograniczenia:\n"
        << "(0) podobny rozmiar\n"
        << "(1) kat 60 stopni\n"
        << "(2) przystawanie\n"
        << "(3) prostopadly\n"
        << "(4) rownowlegly\n"
        << "(5) nie rownolegly\n"
        << "(6) daleki\n"
        << "(7) przystawanie do poczatku\n"
        << "(8) przystawanie do konca\n"
        << "(9) przystawanie wierzcholkow\n";

    cin >> response;
    for (auto c : response) {
        constraintArray.push_back(c-'0');
    }
}

//-----------------------------------------------------------------------------
void Cli::LoadModel() {
    ifstream infile (pathToModel);

    model = Model();
    constraintArray.clear();

    auto constraintOfEdges = vector<vector<BoolPlus>>{};
    auto verticesNo = int{};

    string line;
    while (getline(infile, line)) {
        istringstream iss(line);

        //get constraints from the first line
        if (!constraintArray.size()) {
            int i;
            while (iss >> i) {
                constraintArray.push_back(i);
            }
            continue;
        }
        if (!constraintArray.size()) {
            cout << "\nBledny plik";
        }
        else {
            //get constraints of all edges
            auto v1 = int{};
            auto v2 = int{};

            iss >> v1 >> v2;
            verticesNo = v2 + 1;

            auto constraints = vector<BoolPlus>{};
            auto i = int{};
            while (iss >> i) {
                switch (i) {
                    case 0:
                        constraints.push_back(NO);
                        break;
                    case 1:
                        constraints.push_back(YES);
                        break;
                    case 2:
                        constraints.push_back(DNC);
                        break;
                    default:
                        break;
                }
            }

            constraintOfEdges.push_back(constraints);
        }
    }

    //create vertices
    for (auto i = 0; i < verticesNo; i++) {
        model.vertices.push_back(make_shared<Vertex>(Vertex{}));
    }

    //create edges and connect them to vertices and give constraints
    Utilities u;

    auto constraintOfEdgesSize = constraintOfEdges.size();
    for (auto i = 0; i < constraintOfEdgesSize; i++) {
        auto e = Edge{};
        auto vi = u.unpair(i);
        e.vertices = {weak_ptr<Vertex>(model.vertices[vi.first]),
                      weak_ptr<Vertex>(model.vertices[vi.second])};
        e.constraints = constraintOfEdges[i];
        model.edges.push_back(make_shared<Edge>(e));

        //connect vertices to edges
        auto weakEdge = weak_ptr<Edge>{model.edges[i]};
        model.vertices[vi.first]->involved.push_back(weakEdge);
        model.vertices[vi.second]->involved.push_back(weakEdge);
    }

    //put constraints in model
    for (auto i : constraintArray) {
        model.constraints.push_back(constraints[i]);
    }
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

shared_ptr<LineWrap> Cli::parseSVGLine(string line) {
    //skip chars until '"'
    int i = 0;
    for (; line[i] != '"'; i++) {}

    //write to x1 until '"'
    string s_x1 = "";
    i++;
    for (; line[i] != '"'; i ++) {
        s_x1.push_back(line[i]);
    }

    //skip chars until '"'
    i++;
    for (; line[i] != '"'; i++) {}

    //write to y1 until '"'
    string s_y1 = "";
    i++;
    for (; line[i] != '"'; i ++) {
        s_y1.push_back(line[i]);
    }

    //skip chars until '"'
    i++;
    for (; line[i] != '"'; i++) {}

    //write to x2 until '"'
    string s_x2 = "";
    i++;
    for (; line[i] != '"'; i ++) {
        s_x2.push_back(line[i]);
    }

    //skip chars until '"'
    i++;
    for (; line[i] != '"'; i++) {}

    //write to y2 until '"'
    string s_y2 = "";
    i++;
    for (; line[i] != '"'; i ++) {
        s_y2.push_back(line[i]);
    }

    //skip the rest of the line

    //transform strings to doubles
    double x1, x2, y1, y2;
    x1 = stod(s_x1);
    x2 = stod(s_x2);
    y1 = stod(s_y1);
    y2 = stod(s_y2);

    //construct line wrap
    //auto lw = LineWrap{pair{x1, y1}, pair{x2, y2}};
    return make_shared<LineWrap>(LineWrap{{x1, y1}, {x2, y2}});
}
