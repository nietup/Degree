#include <TGUI/TGUI.hpp>
#include "StateMachine.h"

#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "LineWrap.h"
#include "MatchingSystem.h"

using namespace std;
using namespace elsd;

void TestDetection() {
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

    //function scoring closeness to 60 degrees between lines
    //* returns double in range [0, 1], where 0 is the best match
    auto angle60 = make_shared<Constraint>([](const LineWrap &a,
                                              const LineWrap &b) {
        /*cout << "angle: " << (2*abs(0.5 - abs(a.GetCos(b))))
             << endl
            << " ax1: " << a.start.first

            << " | ay1: " << a.start.second
            << endl
            << " ax2: " << a.end.first

            << " | ay2: " << a.end.second
            << endl << endl
            << " bx1: " << b.start.first

            << " | by1: " << b.start.second
            << endl
            << " bx2: " << b.end.first

            << " | by2: " << b.end.second
            << endl << endl

            << "cos ab: " << a.GetCos(b)
            << endl;*/

        return 2 * abs(0.5 - abs(a.GetCos(b)));
    });

    //function scoring adjacency between lines
    //* returns double in range [0, 1], where 0 is the best match
    auto adjacent = make_shared<Constraint>([](const LineWrap &a,
                                               const LineWrap &b) {
        double d = a.Distance(b);
        /*cout << " ("
             << a.start.first << ", "
             << a.start.second << ")("
             << a.end.first << ", "
             << a.end.second << ")"
             << " ("
             << b.start.first << ", "
             << b.start.second << ")("
             << b.end.first << ", "
             << b.end.second << ") "
             << "adjacent: " << (d / (d + 1000))
             << endl;*/
        return d / (d + 1000);
    });


    auto model = SModel{};

    /*
     * foremny Triangle model
     */
    /*
    auto a1 = make_shared<Atom>(Atom{"1"});
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

    /*auto a1 = make_shared<Atom>(Atom{"1"});
    auto a2 = make_shared<Atom>(Atom{"2"});
    auto a3 = make_shared<Atom>(Atom{"3"});

    auto v = vector<weak_ptr<Constraint>>{
        weak_ptr<Constraint>(adjacent)
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
     * square model
     */

    auto a1 = make_shared<Atom>(Atom{"1"});
    auto a2 = make_shared<Atom>(Atom{"2"});
    auto a3 = make_shared<Atom>(Atom{"3"});
    auto a4 = make_shared<Atom>(Atom{"4"});

    auto close = vector<weak_ptr<Constraint>>{
        weak_ptr<Constraint>(adjacent),
        weak_ptr<Constraint>(sizeMatch),
        weak_ptr<Constraint>(perpendicular)
    };

    auto far = vector<weak_ptr<Constraint>>{
        weak_ptr<Constraint>(parallel),
        weak_ptr<Constraint>(sizeMatch),
    };

    auto p1 = make_shared<Part>(Part{{a1, a2}, close});
    auto p2 = make_shared<Part>(Part{{a1, a3}, far});
    auto p3 = make_shared<Part>(Part{{a1, a4}, close});
    auto p4 = make_shared<Part>(Part{{a2, a3}, close});
    auto p5 = make_shared<Part>(Part{{a2, a4}, far});
    auto p6 = make_shared<Part>(Part{{a3, a4}, close});

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
    model.parts.push_back(p6);

    /*
     * ELSD image processing
     * detect segments and save them in separate image for testing purposes
     */

    cout << "file name: ";
    auto inFile = string{};
    cin >> inFile;
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
    ofs << *svg << "</svg>";
    ofs.close();

    /*
     * Create vector of wrapped segments
     * and apply it to the matcher
     */

    //TODO fix so there is no copy
    auto segsShared = vector<shared_ptr<LineWrap>>();
    for (const auto &line : lines)
        segsShared.push_back(make_shared<LineWrap>(line));

    auto segments = vector<weak_ptr<LineWrap>>();
    for (const auto &line : segsShared)
        segments.push_back(weak_ptr<LineWrap>(line));


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
}

int main() {
/*	StateMachine SM(800, 600, 60, "Akwizycja Modeli");
	MenuState Menu(&SM);
	SM.ChangeState(&Menu);

	while (SM.IsRunning()) {
		SM.Update();
		SM.Render();
	}*/

    TestDetection();

	return 0;
}


