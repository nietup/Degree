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
    /*string outFile = inFile + ".svg";
    SvgWriterInterface::Ptr svg(new ElsdSvgWriter);
    svg->setImageSize(image->xsize(), image->ysize());
    svg->addLineSegments(detector->getLineSegments().begin(), detector->getLineSegments().end());
    ofstream ofs(outFile, ofstream::out);
    ofs << *svg;
    ofs.close();*/

    vector<LineSegment> ls = detector->getLineSegments();

    //matcher seems to work for now, lets focus on scoring
    /*Matcher mat(20, parts, &ls);
    int * match = mat.Match();
    for (int i = 0; i < 3; i++)
        cout << "\n" << match[i];*/
    TestScore(ls[2],ls[3]);
}

/*
 anlge a between v and u

 cos a    = dot(v,u) / (len(v)*len(u))
 dot(v,u) = v.x*u.x + v.y*u.y
 len(v)   = sqrt(v.x^2 + v.y^2)
*/
int TestScore(LineSegment & a, LineSegment & b) {
    double x1 = a.startPoint[0],
           y1 = a.startPoint[1],
           x2 = a.endPoint[0],
           y2 = a.endPoint[1],
           bx1 = b.startPoint[0],
           by1 = b.startPoint[1],
           bx2 = b.endPoint[0],
           by2 = b.endPoint[1],
           lenA = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)),
           lenB = sqrt((bx2-bx1)*(bx2-bx1)+(by2-by1)*(by2-by1)),
           dot = (x2-x1)*(bx2-bx1) + (y2-y1)*(by2-by1),
           cos = dot/(lenA*lenB);

    cout << "\nfirst we will count line length using point coords"
         << "\nx1: " << x1 << " y1: " << y1
         << "\nx2: " << x2 << " y2: " << y2
         << "\nsqrt((x2-x1)^2+(y2-y1)^2): " << lenA
         << "\n\nnow lets calculate cos angle between two line segments"
         << "\nbx1: " << bx1 << " by1: " << by1
         << "\nbx2: " << bx2 << " by2: " << by2
         << "\nlenB: " << lenB
         << "\ndot(a, b): " << dot
         << "\ncos: " << cos;



    /*double by1 = b.startPoint[1];
    double by2 = b.endPoint[1];

    double hard1 = 361.538;
    double hard2 = 361.53;

    cout.precision(17);
    cout << "\nb.startPoint[1]:\t\t\t\t\t"                << b.startPoint[1]
         << "\nb.endPoint[1]:\t\t\t\t\t\t"                << b.endPoint[1]
         << "\nb.startPoint[1] - b.endPoint[1]:\t"        << b.startPoint[1] - b.endPoint[1]
         << "\ndouble by1 = b.startPoint[1]:\t\t"         << by1
         << "\ndouble by2 = b.endPoint[1]:\t\t\t"         << by2
         << "\nby1-by2:\t\t\t\t\t\t\t"                    << by1-by2
         << "\ndouble hard1 = 361.538:\t\t\t\t"           << hard1
         << "\ndouble hard2 = 361.53:\t\t\t\t"            << hard2
         << "\nhard1 - hard2:\t\t\t\t\t\t"                << hard1 - hard2;*/

    return 0;
}