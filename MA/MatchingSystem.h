//
// Created by nietup on 18.05.17.
//

#ifndef DEGREE_MATCHINGSYSTEM_H
#define DEGREE_MATCHINGSYSTEM_H

#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "LineWrap.h"

using namespace std;
using namespace elsd;

const auto threshold = 0.1;

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
    //vector<weak_ptr<Constraint>> constraints;
    vector<uint> constraints;
};

struct SModel {
    vector<shared_ptr<Part>> parts;

    //should be sorted by most constrained first
    vector<shared_ptr<Atom>> atoms;

    vector<shared_ptr<Constraint>> constraints;
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

pair<weak_ptr<Atom>, weak_ptr<Part>> FindAtom(const SearchTree & tree) {
    int node = tree.size() - 1;
    auto & discarded = tree[node].discardedAtoms;

    for (; node >= 0; node--) {
        auto & atom = *tree[node].atom.lock();

        for (auto & part : atom.involved) {
            auto atoms = part.lock().get()->atoms;
            if (atoms.first.lock().get() == &atom) {
                if (atoms.second.lock().get()->asignment.expired()) {
                    if (!myContains<Atom>(atoms.second, discarded)) {
                        return {atoms.second, weak_ptr<Part>(part)};
                    }
                }
            }
            else {
                if (atoms.first.lock().get()->asignment.expired()) {
                    if (!myContains<Atom>(atoms.first, discarded)) {
                        return {atoms.first, weak_ptr<Part>(part)};
                    }
                }
            }
        }
    }
    return pair<weak_ptr<Atom>, weak_ptr<Part>>();
}


//is it so far consistent to match segment with atom?
bool Consistent(const LineWrap & segment, const Atom & atom,
                const SModel & model) {
    for (auto & part : atom.involved) {
        auto atoms = part.lock().get()->atoms;

        //if both vars in part are unassigned then it cannot be contradictory
        if (atoms.first.lock()->asignment.expired() &&
            atoms.second.lock()->asignment.expired())
            continue;

        weak_ptr<LineWrap> otherSegment;
        if (atoms.first.lock()->asignment.expired()) {
            otherSegment = atoms.second.lock().get()->asignment;
        }
        else {
            otherSegment = atoms.first.lock().get()->asignment;
        }

        for (auto constraint : part.lock().get()->constraints) {
            if (part.lock()->atoms.first.lock().get() ==
                const_cast<Atom *>(&atom)) {
                if (threshold <
                    model.constraints[constraint]->operator()(segment,
                                                  *otherSegment.lock())) {
                    return false;
                }
            }
            else {
                if (threshold < model.constraints[constraint]->
                        operator()(*otherSegment.lock(), segment)) {
                    return false;
                }
            }
        }
    }

    return true;
}

weak_ptr<LineWrap> FindSegment(const vector<weak_ptr<LineWrap>> & segments,
                               vector<weak_ptr<LineWrap>> & discarded,
                               const Atom & atom, const SModel & model) {
    for (auto & segment : segments) {
        if (!segment.lock().get()->matched) {
            if (!myContains<LineWrap>(segment, discarded)) {
                if (Consistent(*segment.lock().get(), atom, model)) {
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
//if no nmatch returns weak prt to the furthest achieved part
pair<bool, weak_ptr<Part>> Match(SModel model,
                                 const vector<weak_ptr<LineWrap>> & segments) {
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
    auto furthestPart = weak_ptr<Part>{};
    auto AtomsSize = model.atoms.size();
    auto PartsSize = model.parts.size();
    if (!AtomsSize || !PartsSize)
        return {false, weak_ptr<Part>{}};
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
                return {false, furthestPart};
            }
            else { //we found next segment for tree
                match[0].atom.lock()->asignment = nextSegment;
                nextSegment.lock()->matched = true;
                i++;
            }
        }

        auto nextAtom = weak_ptr<Atom>{};
        tie(nextAtom, furthestPart) = FindAtom(match);
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
                                       *nextAtom.lock(), model);
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

    return {true, weak_ptr<Part>{}};
}



#endif //DEGREE_MATCHINGSYSTEM_H
