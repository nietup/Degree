#include <TGUI/TGUI.hpp>
#include "StateMachine.h"

#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Matcher.h"

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
    cout << "In this testing case we hardcode triangle model and test classification";

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

    vector<LineSegment> ls = detector->getLineSegments();

    //matcher seems to work for now, lets focus on
    /*Matcher mat(20, parts, &ls);
    int * match = mat.Match();
    for (int i = 0; i < 3; i++)
        cout << "\n" << match[i];*/
    TestScore(ls[0],ls[1]);
}

int TestScore(LineSegment & a, LineSegment & b) {
    double x1 = a.startPoint[0],
           y1 = a.startPoint[1],
           x2 = a.endPoint[0],
           y2 = a.endPoint[1];
    cout << "\nfirst we will count line length using point coords"
         << "\nx1: " << x1 << " y1: " << y1
         << "\nx2: " << x2 << " y2: " << y2
         << "\nsqrt((x2-x1)^2+(y2-y1)^2): " << sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    cout << "\n\nnow lets calculate angle between two line segments";

    return 0;
}