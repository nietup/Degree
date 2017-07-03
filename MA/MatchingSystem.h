//
// Created by nietup on 18.05.17.
//

#ifndef DEGREE_MATCHINGSYSTEM_H
#define DEGREE_MATCHINGSYSTEM_H

#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Model.h"

using namespace std;
using namespace elsd;

const auto threshold = 0.1;

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

pair<weak_ptr<Vertex>, weak_ptr<Edge>> FindAtom(const SearchTree & tree) {
    int node = tree.size() - 1;
    auto & discarded = tree[node].discardedAtoms;
    auto furthestPart = tree[0].atom.lock()->involved[0];
    for (; node >= 0; node--) {
        auto & atom = *tree[node].atom.lock();

        for (auto & part : atom.involved) {
            auto atoms = part.lock().get()->atoms;
            if (atoms.first.lock().get() == &atom) {
                if (atoms.second.lock().get()->asignment.expired()) {
                    if (!myContains<Vertex>(atoms.second, discarded)) {
                        furthestPart = part;
                        return {atoms.second, weak_ptr<Edge>(furthestPart)};
                    }
                }
            } else {
                if (atoms.first.lock().get()->asignment.expired()) {
                    if (!myContains<Vertex>(atoms.first, discarded)) {
                        furthestPart = part;
                        return {atoms.first, weak_ptr<Edge>(furthestPart)};
                    }
                }
            }
        }
    }
    return pair<weak_ptr<Vertex>, weak_ptr<Edge>>{{}, {}};
}

//is it so far consistent to match segment with atom?
pair<bool, weak_ptr<Edge>> Consistent(const LineWrap & segment, const Vertex & atom,
                const Model & model) {
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


        auto constrSize = part.lock()->constraints.size();
        for (auto i = 0; i < constrSize; i++) {
            if (DNC == part.lock()->constraints[i]) {
                continue;
            }
            if (YES == part.lock()->constraints[i]) {
                if (part.lock()->atoms.first.lock().get() ==
                    const_cast<Vertex *>(&atom)) {
                    if (threshold < model.constraints[i]->
                        operator()(segment, *otherSegment.lock())) {
                        return {false, part};
                    }
                } else {
                    if (threshold < model.constraints[i]->
                        operator()(*otherSegment.lock(), segment)) {
                        return {false, part};
                    }
                }
            } else if (NO == part.lock()->constraints[i]) {
                if (part.lock()->atoms.first.lock().get() ==
                    const_cast<Vertex *>(&atom)) {
                    if (threshold > model.constraints[i]->
                        operator()(segment, *otherSegment.lock())) {
                        return {false, part};
                    }
                } else {
                    if (threshold > model.constraints[i]->
                        operator()(*otherSegment.lock(), segment)) {
                        return {false, part};
                    }
                }
            }
        }
    }

    return {true, {}};
}

pair<weak_ptr<LineWrap>, weak_ptr<Edge>> FindSegment(
        const vector<weak_ptr<LineWrap>> & segments,
        vector<weak_ptr<LineWrap>> & discarded,
        const Vertex & atom, const Model & model) {
    auto furthestPart = weak_ptr<Edge>{};
    for (auto & segment : segments) {
        if (!segment.lock().get()->matched) {
            if (!myContains<LineWrap>(segment, discarded)) {
                auto consistent = bool{};
                tie(consistent, furthestPart) =
                    Consistent(*segment.lock().get(), atom, model);
                if (consistent) {
                    return {segment, {}};
                }
                else {
                    discarded.push_back(segment);
                }
            }
        }
    }
    return {weak_ptr<LineWrap>(), furthestPart};
}

//assumption: no independent graphs in model
//return value: true if match, false if non match
//if no nmatch returns weak prt to the furthest achieved part
pair<bool, weak_ptr<Edge>> Match(Model model,
                                 const vector<weak_ptr<LineWrap>> & segments) {
    //intro stuff
    auto furthestPart = weak_ptr<Edge>{};
    auto prevPart = weak_ptr<Edge>{};
    auto furthestVector = vector<int>{};
    auto partsMatched = vector<int>{};
    auto AtomsSize = model.atoms.size();
    auto PartsSize = model.parts.size();
    if (!AtomsSize || !PartsSize)
        return {false, weak_ptr<Edge>{}};
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
            for (auto &seg : segments) {
                if (seg.lock() == match[0].atom.lock()->asignment.lock()) {
                    if (j < segments.size())
                        nextSegment = *(&seg + 1);
                    break;
                }
                j++;
            }

            if (nextSegment.expired()) {
                //if we have tried all segments as roots then it is non match
                if (!furthestVector.empty()) {
                    furthestPart =
                        model.parts[furthestVector[furthestVector.size()-1]];
                }
                return {false, furthestPart};
            } else { //we found next segment for tree
                match[0].atom.lock()->asignment = nextSegment;
                nextSegment.lock()->matched = true;
                i++;
            }
        }

        auto nextAtom = weak_ptr<Vertex>{};
        auto nextPart = weak_ptr<Edge>{};
        tie(nextAtom, nextPart) = FindAtom(match);

        auto wasPut = false;
        if (!nextPart.expired()) {
            //get index of nextPart
            auto npi = 0;
            for (auto & p : model.parts) {
                if (nextPart.lock() == p) {
                    break;
                }
                npi++;
            }

            //check if its already added and if it is not in matched prevs
            if (furthestVector.end() ==
                find(furthestVector.begin(), furthestVector.end(), npi)) {
                if (partsMatched.end() ==
                    find(partsMatched.begin(), partsMatched.end(), npi)) {
                    furthestVector.push_back(npi);
                    wasPut = true;
                }
            }
        }

        if (nextAtom.expired()) {
            if (furthestPart.expired()) {
                furthestPart = model.atoms[0]->involved[0];
            }
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

        auto nextSegment = weak_ptr<LineWrap>{};
        tie(nextSegment, nextPart) = FindSegment(segments, match[i - 1].discardedSegments,
                                       *nextAtom.lock(), model);
        if (nextSegment.expired()) {
            //get index of nextPart
            if (!nextPart.expired()) {
                auto npi = 0;
                for (auto &p : model.parts) {
                    if (nextPart.lock() == p) {
                        break;
                    }
                    npi++;
                }
                if (partsMatched.end() ==
                    find(partsMatched.begin(), partsMatched.end(), npi)) {
                    furthestVector.push_back(npi);
                }
            }
            match[i - 1].discardedAtoms.push_back(nextAtom);
            match[i - 1].discardedSegments.clear();
            continue;
        }

        if (wasPut) {
            partsMatched.push_back(furthestVector[furthestVector.size() - 1]);
            furthestVector.pop_back();
            wasPut = false;
        }

        nextSegment.lock().get()->matched = true;
        nextAtom.lock().get()->asignment = nextSegment;
        match.push_back(TreeNode{ nextAtom });

        i++;
    }

    return {true, weak_ptr<Edge>{}};
}



#endif //DEGREE_MATCHINGSYSTEM_H
