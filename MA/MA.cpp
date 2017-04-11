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

    //SvgArc i think derives form LineSegment
    auto score = [] (LineSegment * a, LineSegment * b) { return 1; };

    Relationship angle("kat");
    angle.SetScoringFunction(score);
    Relationship cons("przystawanie");
    cons.SetScoringFunction(score);
    Relationship size("podobny rozmiar");
    size.SetScoringFunction(score);

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

    string inFile = "./test.pgm";
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

    vector<LineSegment> ls = detector->getLineSegments();

    //matcher seems to work for now, lets focus on scoring
    /*Matcher mat(20, parts, &ls);
    int * match = mat.Match();
    for (int i = 0; i < 3; i++)
        cout << "\n" << match[i];*/
    TestScore(ls[1],ls[3]);
}

int TestScore(LineSegment & a, LineSegment & b) {
    LineWrap la(a);
    LineWrap lb(b);
    cout << "\nlenA: " << la.Length()
         << "\nlenB: " << lb.Length()
         << "\ndot(a, b): " << la.Dot(lb)
         << "\ncos: " << la.GetCos(lb)
         << "\ndist: " << la.Distance(lb);

    return 0;
}
