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
    auto score = [] (LineSegment * a, LineSegment * b) { return rand()%100; };

    Relationship angle("kat");
    //angle.SetScoringFunction(score);

    Relationship cons("przystawanie");
    //cons.SetScoringFunction(score);
    Relationship size("podobny rozmiar");
    //size.SetScoringFunction(score);

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
    Matcher mat(5, 4, parts, &ls);
    mat.Match();
}

int Pair(int x, int y) {
    return (int) (0.5 * (y * y - 2 * x + y) - 1);
}

pair<int, int> Unpair(int z) {
    int y = (int) floor(0.5 * (sqrt(8.0 * z + 1.0) + 1.0));
    int x = (int) (0.5 * (y * y + y - 2 * z) - 1);
    pair<int, int> result(x,y);
    return result;
}