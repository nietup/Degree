#include <TGUI/TGUI.hpp>
#include "StateMachine.h"

#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "LineWrap.h"
#include "MatchingSystem.h"

using namespace std;
using namespace elsd;

void TestDetection() {
    //function scoring size match
    //* returns double in range [0, 1], where 0 is the best match
    auto sizeMatch = make_shared<Constraint>([] (const LineWrap & a,
                                                 const LineWrap & b) {
        double lenA = a.Length(),
               lenB = b.Length();
        cout << "size match: "
             << (lenA < lenB ? 1 - (lenA / lenB) : 1 - (lenB / lenA))
             << endl;
        return lenA < lenB ? 1 - (lenA / lenB) : 1 - (lenB / lenA);
    });

    //function scoring perpendicularity between lines
    //* returns double in range [0, 1], where 0 is the best match
    auto perpendicular = make_shared<Constraint>([] (const LineWrap & a,
                                                     const LineWrap & b) {
        return abs(a.GetCos(b));
    });

    //function scoring parallelity between lines
    //* returns double in range [0, 1], where 0 is the best match
    auto parallel = make_shared<Constraint>([] (const LineWrap & a,
                        const LineWrap & b) {
        return 1.0 - abs(a.GetCos(b));
    });

    //function scoring closeness to 60 degrees between lines
    //* returns double in range [0, 1], where 0 is the best match
    auto angle60 = make_shared<Constraint>([] (const LineWrap & a,
                                               const LineWrap & b) {
        cout << "angle: " << (2*abs(0.5 - abs(a.GetCos(b))))
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
            << endl
            ;
        return 2*abs(0.5 - abs(a.GetCos(b)));
    });

    //function scoring adjacency between lines
    //* returns double in range [0, 1], where 0 is the best match
    auto adjacent = make_shared<Constraint>([] (const LineWrap & a,
                                                const LineWrap & b) {
        double d = a.Distance(b);
        cout << "angle: " << (d / (d + 1000))
             << endl;
        return d / (d + 1000);
    });


    auto model = SModel{};

    auto a1 = make_shared<Atom>(Atom{ "1" });
    auto a2 = make_shared<Atom>(Atom{ "2" });
    auto a3 = make_shared<Atom>(Atom{ "3" });

    auto f = make_shared<Constraint>([](const LineWrap & a,
                                        const LineWrap & b) {
        return 100.0;
    });

    auto v = vector<weak_ptr<Constraint>>{
        weak_ptr<Constraint>(angle60),
        weak_ptr<Constraint>(adjacent),
        weak_ptr<Constraint>(sizeMatch)
    };

    auto p1 = make_shared<Part>(Part{ {a1, a2}, v });
    auto p2 = make_shared<Part>(Part{ {a1, a3}, v });
    auto p3 = make_shared<Part>(Part{ {a2, a3}, v });

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
    model.parts.push_back(p3);

    auto l1 = make_shared<LineWrap>(LineWrap{ {1.0, 0.0}, {0.5, 0.866025404} });
    auto l2 = make_shared<LineWrap>(LineWrap{ {0.0, 0.0}, {1.0, 0.0} });
    auto l3 = make_shared<LineWrap>(LineWrap{ {0.0, 0.0}, {0.5, 0.866025404} });

    auto segments = vector<weak_ptr<LineWrap>>{ weak_ptr<LineWrap>(l1),
                                                weak_ptr<LineWrap>(l2),
                                                weak_ptr<LineWrap>(l3)};

    if (Match(model, segments))
        for (auto & a : model.atoms)
            cout << a->name << " "
                 << a.get()->asignment.lock().get()->start.first << endl;
    else
        cout << "Non match";
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


