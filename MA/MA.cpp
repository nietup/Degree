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
void Fit();
int Pair();
pair<int, int> Unpair();
int TestScore(LineSegment & a, LineSegment & b);

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

void DetectionTest() {
    //cout << "In this testing case we hardcode triangle model and test classification";

    srand((uint)time(NULL));

    auto score = [] (LineWrap * a, LineWrap * b) {
        cout << "\n-------------\n" << a->Length() << "\n-------------\n";
        return a->Length();
    };

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

    /* function scoring adjacency between lines
    *  returns double in range [0, 1], where 0 is the best match
    */
    auto adjacent = [] (LineWrap * a, LineWrap * b) {
        double d = a->Distance(*b);
        cout << "\n-------------\n" << d << "\n-------------\n";
        return d / (d + 1000);
    };

    Relationship angle("kat");
    angle.SetScoringFunction(adjacent);
    Relationship cons("przystawanie");
    cons.SetScoringFunction(adjacent);
    Relationship size("podobny rozmiar");
    size.SetScoringFunction(adjacent);

    vector<Relationship*> vertex;
    vertex.push_back(&angle);
    vertex.push_back(&cons);
    vertex.push_back(&size);

    vector<vector<Relationship*>*> * parts = new vector<vector<Relationship*>*>();
    parts->push_back(&vertex);
    parts->push_back(&vertex);
    parts->push_back(&vertex);

    Primitive triangle;
    triangle.AssignParts(parts);

    string inFile = "./7.pgm";
    ImageInterface::Ptr image(new ElsdPgmFileReader(inFile));
    ShapesDetectorInterface::Ptr detector(new ElsDetector);
    detector->run(image);

    //save image
    string outFile = inFile + ".svg";
    SvgWriterInterface::Ptr svg(new ElsdSvgWriter);
    svg->setImageSize(image->xsize(), image->ysize());
    svg->addLineSegments(detector->getLineSegments().begin(), detector->getLineSegments().end());
    ofstream ofs(outFile, ofstream::out);
    ofs << *svg;
    ofs.close();

    vector<LineWrap> ls;
    for (auto a : detector->getLineSegments()) {
        LineWrap l(a);
        ls.push_back(l);
    }

    //matcher seems to work for now, lets focus on scoring
    Matcher mat(20, parts, &ls);
    int * match = mat.Match();
    for (int i = 0; i < 3; i++)
        cout << "\n" << match[i];
}

