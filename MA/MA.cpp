#include <TGUI/TGUI.hpp>
#include "StateMachine.h"

#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "LineWrap.h"

using namespace std;
using namespace elsd;

void DetectionTest();

using Constraint = function<double(const LineWrap &, const LineWrap &)>;

struct Part;

struct Atom {
    string name;
    weak_ptr<LineWrap> asignment;

    //should be sorted by most constrained first
    vector<weak_ptr<Part>> involved;
};

struct Part {
    pair<weak_ptr<Atom>, weak_ptr<Atom>> atoms;
    vector<weak_ptr<Constraint>> constraints;
};

struct SModel {
    vector<shared_ptr<Part>> parts;

    //should be sorted by most constrained first
    vector<shared_ptr<Atom>> atoms;
};

struct TreeNode {
    weak_ptr<Atom> atom;
    vector<weak_ptr<Atom>> discardedAtoms;
    vector<weak_ptr<LineWrap>> discardedSegments;
};

using SearchTree = vector<TreeNode>;

template <class T>
bool myContains(weak_ptr<T> object, const vector<weak_ptr<T>> & v) {
    for (auto & a : v) {
        if (object.lock().get() == a.lock().get()) {
            return true;
        }
    }

    return false;
}

weak_ptr<Atom> FindAtom(const SearchTree & tree) {
    int node = tree.size() - 1;
    auto & discarded = tree[node].discardedAtoms;

    for (; node >= 0; node--) {
        auto & atom = *tree[node].atom.lock();

        for (auto & part : atom.involved) {
            auto atoms = part.lock().get()->atoms;
            if (atoms.first.lock().get() == &atom) {
                if (atoms.second.lock().get()->asignment.expired()) {
                    if (!myContains<Atom>(atoms.second, discarded)) {
                        return atoms.second;
                    }
                }
            }
            else {
                if (atoms.first.lock().get()->asignment.expired()) {
                    if (!myContains<Atom>(atoms.first, discarded)) {
                        return atoms.first;
                    }
                }
            }
        }
    }
    return weak_ptr<Atom>();
}

const auto threshold = 88.0;

bool Consistent(const LineWrap & segment, const Atom & atom) {
    for (auto & part : atom.involved) {
        auto atoms = part.lock().get()->atoms;

        //if both vars in part are unassigned then it cannot be contradictory
        if (atoms.first.lock()->asignment.expired() &&
            atoms.second.lock()->asignment.expired())
            continue;

        weak_ptr<LineWrap> otherSegment;
        if (atoms.first.lock()->asignment.expired())
            otherSegment = atoms.second.lock().get()->asignment;
        else
            otherSegment = atoms.first.lock().get()->asignment;

        for (auto constraint : part.lock().get()->constraints) {
            if (threshold >
                constraint.lock()->operator()(segment, *otherSegment.lock())) {
                return false;
            }
        }
    }

    return true;
}

weak_ptr<LineWrap> FindSegment(const vector<weak_ptr<LineWrap>> & segments,
                               vector<weak_ptr<LineWrap>> & discarded,
                               const Atom & atom) {
    for (auto & segment : segments) {
        if (!segment.lock().get()->matched) {
            if (!myContains<LineWrap>(segment, discarded)) {
                if (Consistent(*segment.lock().get(), atom)) {
                    return segment;
                }
                else {
                    discarded.push_back(segment);
                }
            }
        }
    }

    return weak_ptr<LineWrap>();
}

//assumption: no independent graphs in model
//return value: true if match, false if non match
bool Match(SModel & model, const vector<weak_ptr<LineWrap>> & segments) {
    /*
    i = 0
    while i < Atoms.size() do
        Tree[i].atom = find(Atoms, first connected with Tree[i-1].atom and
            wout assigned segment and not in discardedA array in Tree[i-1])
        if Tree[i].atom empty then
            Tree[i-1].discardedS.push_back(Tree[i-1].atom.segment)
            Tree.pop(i)
            i--
            break
        end
        Tree[i].atom.segment = find(Segments, first free and
            not in discardedS array in Tree[i-1] and
            consistend with Tree[0..i-1])
        if Tree[i].atom.segment empty then
            Tree[i-1].discardedA.push_back(Tree[i].atom)
            Tree.pop(i)
            break
        end
        i++
    end
    */

    //intro stuff
    auto AtomsSize = model.atoms.size();
    auto PartsSize = model.parts.size();
    if (!AtomsSize || !PartsSize)
        return false;
    model.atoms[0]->asignment = segments[0];
    segments[0].lock()->matched = true;
    auto match = SearchTree{ TreeNode {model.atoms[0]} };

    //main loop
    auto i = 1u;
    while (i < AtomsSize) {
        //if root didn't work out -- move to separate function later
        if (i == 0) {
            //find next segment for root
            match[0].atom.lock()->asignment.lock()->matched = false;
            match[0].discardedAtoms.clear();
            auto nextSegment = weak_ptr<LineWrap>();
            int j = 1;
            for (auto & seg : segments) {
                if (seg.lock() == match[0].atom.lock()->asignment.lock()) {
                    if (j < segments.size())
                        nextSegment = *(&seg + 1);
                    break;
                }
                j++;
            }

            if (nextSegment.expired()) {
                //if we have tried all segments as roots then it is non match
                return false;
            }
            else { //we found next segment for tree
                match[0].atom.lock()->asignment = nextSegment;
                nextSegment.lock()->matched = true;
                i++;
            }
        }

        auto nextAtom = FindAtom(match);
        if (nextAtom.expired()) {
            if (i > 1) {
                match[i - 2].discardedSegments.
                    push_back(match[i - 1].atom.lock()->asignment);
                match[i - 1].atom.lock()->asignment.lock()->matched = false;
                match[i - 1].atom.lock()->asignment.reset();
                match.pop_back();
            }

            i--;
            continue;
        }

        auto nextSegment = FindSegment(segments, match[i - 1].discardedSegments,
                                       *nextAtom.lock());
        if (nextSegment.expired()) {
            match[i - 1].discardedAtoms.push_back(nextAtom);
            match[i - 1].discardedSegments.clear();
            continue;
        }

        nextSegment.lock().get()->matched = true;
        nextAtom.lock().get()->asignment = nextSegment;
        match.push_back(TreeNode{ nextAtom });

        i++;
    }

    return true;
}

int main() {
/*	StateMachine SM(800, 600, 60, "Akwizycja Modeli");
	MenuState Menu(&SM);
	SM.ChangeState(&Menu);

	while (SM.IsRunning()) {
		SM.Update();
		SM.Render();
	}*/

    auto model = SModel{};

    auto a1 = make_shared<Atom>(Atom{ "1" });
    auto a2 = make_shared<Atom>(Atom{ "2" });
    auto a3 = make_shared<Atom>(Atom{ "3" });
    auto a4 = make_shared<Atom>(Atom{ "4" });

    auto f1 = make_shared<Constraint>([](const LineWrap & a,
                                         const LineWrap & b) {
        auto w1 = 4.0;
        auto w2 = 3.0;
        if ((w1 == a.start.first || w1 == b.start.first) &&
            (w2 == a.start.first || w2 == b.start.first))
            return 100.0;
        else
            return 0.0;
    });
    auto f2 = make_shared<Constraint>([](const LineWrap & a,
                                         const LineWrap & b) {
        auto w1 = 4.0;
        auto w2 = 6.0;
        if ((w1 == a.start.first || w1 == b.start.first) &&
            (w2 == a.start.first || w2 == b.start.first))
            return 100.0;
        else
            return 0.0;
    });
    auto f3 = make_shared<Constraint>([](const LineWrap & a,
                                         const LineWrap & b) {
        auto w1 = 4.0;
        auto w2 = 1.0;
        if ((w1 == a.start.first || w1 == b.start.first) &&
            (w2 == a.start.first || w2 == b.start.first))
            return 100.0;
        else
            return 0.0;
    });
    auto f4 = make_shared<Constraint>([](const LineWrap & a,
                                         const LineWrap & b) {
        return 0.0;
        auto w1 = 3.0;
        auto w2 = 6.0;
        if ((w1 == a.start.first || w1 == b.start.first) &&
            (w2 == a.start.first || w2 == b.start.first))
            return 100.0;
        else
            return 0.0;
    });
    auto f5 = make_shared<Constraint>([](const LineWrap & a,
                                         const LineWrap & b) {
        auto w1 = 3.0;
        auto w2 = 1.0;
        if ((w1 == a.start.first || w1 == b.start.first) &&
            (w2 == a.start.first || w2 == b.start.first))
            return 100.0;
        else
            return 0.0;
    });


    auto v1 = vector<weak_ptr<Constraint>>{ weak_ptr<Constraint>(f1) };
    auto v2 = vector<weak_ptr<Constraint>>{ weak_ptr<Constraint>(f2) };
    auto v3 = vector<weak_ptr<Constraint>>{ weak_ptr<Constraint>(f3) };
    auto v4 = vector<weak_ptr<Constraint>>{ weak_ptr<Constraint>(f4) };
    auto v5 = vector<weak_ptr<Constraint>>{ weak_ptr<Constraint>(f5) };


    auto p1 = make_shared<Part>(Part{ {a1, a2}, v1 });
    auto p2 = make_shared<Part>(Part{ {a1, a3}, v2 });
    auto p3 = make_shared<Part>(Part{ {a1, a4}, v3 });
    auto p4 = make_shared<Part>(Part{ {a2, a3}, v4 });
    auto p5 = make_shared<Part>(Part{ {a2, a4}, v5 });

    a1.get()->involved.push_back(weak_ptr<Part>(p1));
    a1.get()->involved.push_back(weak_ptr<Part>(p2));
    a1.get()->involved.push_back(weak_ptr<Part>(p3));
    a2.get()->involved.push_back(weak_ptr<Part>(p1));
    a2.get()->involved.push_back(weak_ptr<Part>(p4));
    a2.get()->involved.push_back(weak_ptr<Part>(p5));
    a3.get()->involved.push_back(weak_ptr<Part>(p2));
    a3.get()->involved.push_back(weak_ptr<Part>(p4));
    a4.get()->involved.push_back(weak_ptr<Part>(p3));
    a4.get()->involved.push_back(weak_ptr<Part>(p5));

    model.atoms.push_back(a1);
    model.atoms.push_back(a2);
    model.atoms.push_back(a3);
    model.atoms.push_back(a4);

    model.parts.push_back(p1);
    model.parts.push_back(p2);
    model.parts.push_back(p3);
    model.parts.push_back(p4);
    model.parts.push_back(p5);

    auto l1 = make_shared<LineWrap>(LineWrap{ {1, 1}, {1, 1} });
    auto l2 = make_shared<LineWrap>(LineWrap{ {2, 1}, {1, 1} });
    auto l3 = make_shared<LineWrap>(LineWrap{ {3, 1}, {1, 1} });
    auto l4 = make_shared<LineWrap>(LineWrap{ {4, 1}, {1, 1} });
    auto l5 = make_shared<LineWrap>(LineWrap{ {5, 1}, {1, 1} });
    auto l6 = make_shared<LineWrap>(LineWrap{ {6, 1}, {1, 1} });

    auto segments = vector<weak_ptr<LineWrap>>{ weak_ptr<LineWrap>(l1),
                                                weak_ptr<LineWrap>(l2),
                                                weak_ptr<LineWrap>(l3),
                                                weak_ptr<LineWrap>(l4),
                                                weak_ptr<LineWrap>(l5),
                                                weak_ptr<LineWrap>(l6) };

    /*auto f = make_shared<Constraint>([](const LineWrap & a,
                                        const LineWrap & b) {
            return 10.0;
    });

    auto v = vector<weak_ptr<Constraint>>{ weak_ptr<Constraint>(f) };

    auto p1 = make_shared<Part>(Part{ {a1, a2}, v });
    auto p2 = make_shared<Part>(Part{ {a1, a3}, v });
    auto p3 = make_shared<Part>(Part{ {a1, a4}, v });
    auto p4 = make_shared<Part>(Part{ {a2, a3}, v });

    a1.get()->involved.push_back(weak_ptr<Part>(p1));
    a1.get()->involved.push_back(weak_ptr<Part>(p2));
    a1.get()->involved.push_back(weak_ptr<Part>(p3));
    a2.get()->involved.push_back(weak_ptr<Part>(p1));
    a2.get()->involved.push_back(weak_ptr<Part>(p4));
    a3.get()->involved.push_back(weak_ptr<Part>(p2));
    a3.get()->involved.push_back(weak_ptr<Part>(p4));
    a4.get()->involved.push_back(weak_ptr<Part>(p3));

    model.atoms.push_back(a1);
    model.atoms.push_back(a2);
    model.atoms.push_back(a3);
    model.atoms.push_back(a4);

    model.parts.push_back(p1);
    model.parts.push_back(p2);
    model.parts.push_back(p3);
    model.parts.push_back(p4);

    auto l1 = make_shared<LineWrap>(LineWrap{ {1.0, 1}, {1, 1} });
    auto l2 = make_shared<LineWrap>(LineWrap{ {2.0, 1}, {1, 1} });
    auto l3 = make_shared<LineWrap>(LineWrap{ {3.0, 1}, {1, 1} });
    auto l4 = make_shared<LineWrap>(LineWrap{ {4.0, 1}, {1, 1} });

    auto segments = vector<weak_ptr<LineWrap>>{ weak_ptr<LineWrap>(l1),
                                                weak_ptr<LineWrap>(l2),
                                                weak_ptr<LineWrap>(l3),
                                                weak_ptr<LineWrap>(l4)};*/

    if (Match(model, segments))
        for (auto & a : model.atoms)
            cout << a->name << " "
                 << a.get()->asignment.lock().get()->start.first << endl;
    else
        cout << "Non match";

	return 0;
}


