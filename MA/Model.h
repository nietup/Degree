//
// Created by nietup on 02.07.17.
//

#ifndef DEGREE_MODEL_H
#define DEGREE_MODEL_H

#include "Edge.h"

using Constraint = std::function<double(const LineWrap &, const LineWrap &)>;

class Model {
public:
    std::vector<std::shared_ptr<Edge>> parts;

    //should be sorted by most constrained first
    std::vector<std::shared_ptr<Vertex>> atoms;

    std::vector<std::shared_ptr<Constraint>> constraints;
};


#endif //DEGREE_MODEL_H
