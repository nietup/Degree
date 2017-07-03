//
// Created by nietup on 02.07.17.
//

#ifndef DEGREE_PART_H
#define DEGREE_PART_H

#include "TreeNode.h"

enum BoolPlus {NO, YES, DNC};

class Edge {
public:
    std::pair<std::weak_ptr<Vertex>, std::weak_ptr<Vertex>> vertices;
    std::vector<BoolPlus> constraints;
};


#endif //DEGREE_PART_H
