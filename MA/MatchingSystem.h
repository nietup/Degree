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
#include "Utilities.h"

using namespace std;
using namespace elsd;

const auto threshold = 0.1;

using SearchTree = vector<TreeNode>;

class Matcher {
public:
    Matcher() {}


    pair<weak_ptr<Vertex>, weak_ptr<Edge>> FindVertex(const SearchTree &tree) {
        int node = tree.size() - 1;
        auto &discarded = tree[node].discardedVertices;
        auto furthestPart = tree[0].vertex.lock()->involved[0];
        auto util = Utilities();
        for (; node >= 0; node--) {
            auto &vertex = *tree[node].vertex.lock();

            for (auto &part : vertex.involved) {
                auto atoms = part.lock().get()->vertices;
                if (atoms.first.lock().get() == &vertex) {
                    if (atoms.second.lock().get()->asignment.expired()) {
                        if (!util.myContains<Vertex>(atoms.second, discarded)) {
                            furthestPart = part;
                            return {atoms.second, weak_ptr<Edge>(furthestPart)};
                        }
                    }
                } else {
                    if (atoms.first.lock().get()->asignment.expired()) {
                        if (!util.myContains<Vertex>(atoms.first, discarded)) {
                            furthestPart = part;
                            return {atoms.first, weak_ptr<Edge>(furthestPart)};
                        }
                    }
                }
            }
        }
        return pair<weak_ptr<Vertex>, weak_ptr<Edge>>{{},
                                                      {}};
    }

//is it so far consistent to match segment with vertex?
    pair<bool, weak_ptr<Edge>>
    Consistent(const LineWrap &segment, const Vertex &vertex,
               const Model &model) {
        for (auto &part : vertex.involved) {
            auto atoms = part.lock().get()->vertices;

            //if both vars in part are unassigned then it cannot be contradictory
            if (atoms.first.lock()->asignment.expired() &&
                atoms.second.lock()->asignment.expired())
                continue;

            weak_ptr<LineWrap> otherSegment;
            if (atoms.first.lock()->asignment.expired()) {
                otherSegment = atoms.second.lock().get()->asignment;
            } else {
                otherSegment = atoms.first.lock().get()->asignment;
            }


            auto constrSize = part.lock()->constraints.size();
            for (auto i = 0; i < constrSize; i++) {
                if (DNC == part.lock()->constraints[i]) {
                    continue;
                }
                if (YES == part.lock()->constraints[i]) {
                    if (part.lock()->vertices.first.lock().get() ==
                        const_cast<Vertex *>(&vertex)) {
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
                    if (part.lock()->vertices.first.lock().get() ==
                        const_cast<Vertex *>(&vertex)) {
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
        const vector<weak_ptr<LineWrap>> &segments,
        vector<weak_ptr<LineWrap>> &discarded,
        const Vertex &atom, const Model &model) {

        auto util = Utilities();
        auto furthestPart = weak_ptr<Edge>{};
        for (auto &segment : segments) {
            if (!segment.lock().get()->matched) {
                if (!util.myContains<LineWrap>(segment, discarded)) {
                    auto consistent = bool{};
                    tie(consistent, furthestPart) =
                        Consistent(*segment.lock().get(), atom, model);
                    if (consistent) {
                        return {segment, {}};
                    } else {
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
                                     const vector<weak_ptr<LineWrap>> &segments) {
        //intro stuff
        auto furthestEdge = weak_ptr<Edge>{};
        auto prevEdge = weak_ptr<Edge>{};
        auto furthestVector = vector<int>{};
        auto edgesMatched = vector<int>{};
        auto VerticesSize = model.vertices.size();
        auto EdgesSize = model.edges.size();
        if (!VerticesSize || !EdgesSize)
            return {false, weak_ptr<Edge>{}};
        model.vertices[0]->asignment = segments[0];
        segments[0].lock()->matched = true;
        auto match = SearchTree{TreeNode {model.vertices[0]}};

        //main loop
        auto i = 1u;
        while (i < VerticesSize) {
            //if root didn't work out -- move to separate function later
            if (i == 0) {
                //find next segment for root
                match[0].vertex.lock()->asignment.lock()->matched = false;
                match[0].discardedVertices.clear();
                auto nextSegment = weak_ptr<LineWrap>();
                int j = 1;
                for (auto &seg : segments) {
                    if (seg.lock() ==
                        match[0].vertex.lock()->asignment.lock()) {
                        if (j < segments.size())
                            nextSegment = *(&seg + 1);
                        break;
                    }
                    j++;
                }

                if (nextSegment.expired()) {
                    //if we have tried all segments as roots then it is non match
                    if (!furthestVector.empty()) {
                        furthestEdge =
                            model.edges[furthestVector[furthestVector.size() -
                                                       1]];
                    }
                    return {false, furthestEdge};
                } else { //we found next segment for tree
                    match[0].vertex.lock()->asignment = nextSegment;
                    nextSegment.lock()->matched = true;
                    i++;
                }
            }

            auto nextAtom = weak_ptr<Vertex>{};
            auto nextPart = weak_ptr<Edge>{};
            tie(nextAtom, nextPart) = FindVertex(match);

            auto wasPut = false;
            if (!nextPart.expired()) {
                //get index of nextPart
                auto npi = 0;
                for (auto &p : model.edges) {
                    if (nextPart.lock() == p) {
                        break;
                    }
                    npi++;
                }

                //check if its already added and if it is not in matched prevs
                if (furthestVector.end() ==
                    find(furthestVector.begin(), furthestVector.end(), npi)) {
                    if (edgesMatched.end() ==
                        find(edgesMatched.begin(), edgesMatched.end(), npi)) {
                        furthestVector.push_back(npi);
                        wasPut = true;
                    }
                }
            }

            if (nextAtom.expired()) {
                if (furthestEdge.expired()) {
                    furthestEdge = model.vertices[0]->involved[0];
                }
                if (i > 1) {
                    match[i - 2].discardedSegments.
                        push_back(match[i - 1].vertex.lock()->asignment);
                    match[i -
                          1].vertex.lock()->asignment.lock()->matched = false;
                    match[i - 1].vertex.lock()->asignment.reset();
                    match.pop_back();
                }

                i--;
                continue;
            }

            auto nextSegment = weak_ptr<LineWrap>{};
            tie(nextSegment, nextPart) = FindSegment(segments, match[i -
                                                                     1].discardedSegments,
                                                     *nextAtom.lock(), model);
            if (nextSegment.expired()) {
                //get index of nextPart
                if (!nextPart.expired()) {
                    auto npi = 0;
                    for (auto &p : model.edges) {
                        if (nextPart.lock() == p) {
                            break;
                        }
                        npi++;
                    }
                    if (edgesMatched.end() ==
                        find(edgesMatched.begin(), edgesMatched.end(), npi)) {
                        furthestVector.push_back(npi);
                    }
                }
                match[i - 1].discardedVertices.push_back(nextAtom);
                match[i - 1].discardedSegments.clear();
                continue;
            }

            if (wasPut) {
                edgesMatched.push_back(
                    furthestVector[furthestVector.size() - 1]);
                furthestVector.pop_back();
                wasPut = false;
            }

            nextSegment.lock().get()->matched = true;
            nextAtom.lock().get()->asignment = nextSegment;
            match.push_back(TreeNode{nextAtom});

            i++;
        }

        return {true, weak_ptr<Edge>{}};
    }
};


#endif //DEGREE_MATCHINGSYSTEM_H
