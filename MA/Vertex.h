//
// Created by nietup on 02.07.17.
//

#ifndef DEGREE_ATOM_H
#define DEGREE_ATOM_H

#include "LineWrap.h"
class Edge;

class Vertex {
public:
    std::weak_ptr<LineWrap> asignment;

    //should be sorted by most constrained first
    std::vector<std::weak_ptr<Edge>> involved;
};


#endif //DEGREE_ATOM_H
