#include <TGUI/TGUI.hpp>
#include "StateMachine.h"

#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Matcher.h"
#include "LineWrap.h"

using namespace std;
using namespace elsd;

void DetectionTest();

int main() {
/*	StateMachine SM(800, 600, 60, "Akwizycja Modeli");
	MenuState Menu(&SM);
	SM.ChangeState(&Menu);

	while (SM.IsRunning()) {
		SM.Update();
		SM.Render();
	}*/

    DetectionTest();

	return 0;
}
//to be tested:
//filled triangle
//empty trialnge
//cute right etc triangles
//squares
void DetectionTest() {
    /* First part of this test is scoring functions definitions
     * each function takes two line segments as argument
     */

    /* function scoring size match
     * returns double in range [0, 1], where 0 is the best match
     */
    auto sizeMatch = [] (LineWrap * a, LineWrap * b) {
        double lenA = a->Length(),
               lenB = b->Length();
        return lenA < lenB ? 1 - (lenA / lenB) : 1 - (lenB / lenA);
    };

    /* function scoring perpendicularity between lines
     * returns double in range [0, 1], where 0 is the best match
     */
    auto perpendicular = [] (LineWrap * a, LineWrap * b) {
        return abs(a->GetCos(*b));
    };

    /* function scoring parallelity between lines
     * returns double in range [0, 1], where 0 is the best match
     */
    auto parallel = [] (LineWrap * a, LineWrap * b) {
        return 1.0 - abs(a->GetCos(*b));
    };

    /* function scoring closeness to 60 degrees between lines
     * returns double in range [0, 1], where 0 is the best match
     */
    auto angle60 = [] (LineWrap * a, LineWrap * b) {
        return 2*abs(0.5 - abs(a->GetCos(*b)));
    };

    /* function scoring adjacency between lines
    *  returns double in range [0, 1], where 0 is the best match
    */
    auto adjacent = [] (LineWrap * a, LineWrap * b) {
        double d = a->Distance(*b);
        return d / (d + 1000);
    };

    /* Here we define 3 aspects that will be interesting for us
     * in each vertex of a triangle
     * so each vertex must contain two line segments that are:
     * - of 60 deg angle
     * - similar in size
     * - touching each other
     */
    Relationship angle("kat");
    angle.SetScoringFunction(angle60);
    Relationship cons("przystawanie");
    cons.SetScoringFunction(adjacent);
    Relationship size("podobny rozmiar");
    size.SetScoringFunction(sizeMatch);

    vector<Relationship*> vertex;
    vertex.push_back(&angle);
    vertex.push_back(&cons);
    vertex.push_back(&size);

    /* Now that we know what constraints a vertex must fulfill
     * we have to assemble a whole model
     * in that case, a triangle consists of 3 vertices
     */
    vector<vector<Relationship*>*> * parts =
        new vector<vector<Relationship*>*>();
    parts->push_back(&vertex);
    parts->push_back(&vertex);
    parts->push_back(&vertex);

    Primitive triangle;
    triangle.AssignParts(parts);

    /* ELSD image processing
     * detect segments and save them in separate image for testing purposes
     */
    string inFile = "./7.pgm";
    ImageInterface::Ptr image(new ElsdPgmFileReader(inFile));
    ShapesDetectorInterface::Ptr detector(new ElsDetector);
    detector->run(image);

    string outFile = inFile + ".svg";
    SvgWriterInterface::Ptr svg(new ElsdSvgWriter);
    svg->setImageSize(image->xsize(), image->ysize());
    vector<LineSegment> lineSeg = detector->getLineSegments();
    svg->addLineSegments(lineSeg.begin(), lineSeg.end());
    ofstream ofs(outFile, ofstream::out);
    ofs << *svg;
    ofs.close();

    /* Create vector of wrapped segments
     * and apply it to the matcher
     * that will try to match detected segments to defined model
     */
    vector<LineWrap> ls;
    /*for (auto a : detector->getLineSegments()) {
        LineWrap l(a);
        ls.push_back(l);
    }*/
    vector<LineSegment> lines = detector->getLineSegments();
    int numberOfExecutions = lines.size();
    for (int i = 1; i < numberOfExecutions; i++) {
        LineWrap l(lines[i]);
        ls.push_back(l);
    }
    LineWrap l(lines[0]);
    ls.push_back(l);

    Matcher mat(0.5, parts, &ls);
    mat.Match();
}

