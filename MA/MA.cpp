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

unique_ptr<SModel> GenerateModel(
    const vector<vector<weak_ptr<LineWrap>>> & samples,
    const vector<shared_ptr<Constraint>> & constraints);

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

    auto constraints = vector<shared_ptr<Constraint>>{};

    auto GeneratedModel = GenerateModel(samples, constraints);
}

unique_ptr<SModel> GenerateModel(
    const vector<vector<weak_ptr<LineWrap>>> & samples,
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
    auto model = unique_ptr<SModel>(new SModel);
    auto atomCount = samples[0].size();

    return model;
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


