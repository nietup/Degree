#include <TGUI/TGUI.hpp>
#include "StateMachine.h"

#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "LineWrap.h"
#include "MatchingSystem.h"
#include "Learning.h"

using namespace std;
using namespace elsd;

void TestGeneration() {
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
        //cout << "angle: " << (2*abs(0.5 - abs(a.GetCos(b))))
            /* << endl
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

            << "cos ab: " << a.GetCos(b)*/
         //   << endl;

        return 2 * abs(0.5 - abs(a.GetCos(b)));
    });

    //function scoring adjacency between lines
    //* returns double in range [0, 1], where 0 is the best match
    auto adjacent = make_shared<Constraint>([](const LineWrap &a,
                                               const LineWrap &b) {
        double d = a.Distance(b);
        /*cout /*<< " ("
             << a.start.first << ", "
             << a.start.second << ")("
             << a.end.first << ", "
             << a.end.second << ")"
             << " ("
             << b.start.first << ", "
             << b.start.second << ")("
             << b.end.first << ", "
             << b.end.second << ") "*/
             //<< "adjacent: " << (d / (d + 1000))
             //<< endl;
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
        /*cout << "start\na: " << a.start.first << " " << a.start.second
             << "  "  << a.end.first << " " << a.end.second << endl
             << "b: " << b.start.first << " " << b.start.second
             << "  "  << b.end.first << " " << b.end.second << endl
             << "res: " << d / (d + 300.0) << endl;*/
        return d / (d+300.0);
    });

    //function scoring adjacency between end  of line a and line b
    //* returns double in range [0, 1], where 0 is the best match
    auto adjacentEnd = make_shared<Constraint>([](const LineWrap & a,
                                                    const LineWrap & b) {
        auto d = b.Distance(a.end);
        /*cout << "end\na: " << a.start.first << " " << a.start.second
             << "  "  << a.end.first << " " << a.end.second << endl
             << "b: " << b.start.first << " " << b.start.second
             << "  "  << b.end.first << " " << b.end.second << endl
             << "res: " << d / (d + 300.0) << endl;*/
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

        /*cout << "a: " << a.start.first << " " << a.start.second
             << "  "  << a.end.first << " " << a.end.second << endl
             << "b: " << b.start.first << " " << b.start.second
             << "  "  << b.end.first << " " << b.end.second << endl
             << "res: " << a1 / (a1 + 1000.0) << endl;*/

        return a1 / (a1 + 300.0);
    });

    /*
     * ELSD image processing
     * detect segments and save them in separate image for testing purposes
     */

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
        //TODO fix so there is no copy
        auto segsShared = vector<shared_ptr<LineWrap>>();
        for (const auto &line : lines)
            segsShared.push_back(make_shared<LineWrap>(line));

        auto segments = vector<weak_ptr<LineWrap>>();
        for (const auto &line : segsShared)
            segments.push_back(weak_ptr<LineWrap>(line));

        samples.push_back(segments);
    }

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

    GenerateModel(posSamples, negSamples, constraints);
}

int main() {
/*	StateMachine SM(800, 600, 60, "Akwizycja Modeli");
	MenuState Menu(&SM);
	SM.ChangeState(&Menu);

	while (SM.IsRunning()) {
		SM.Update();
		SM.Render();
	}*/

    TestGeneration();

	return 0;
}


