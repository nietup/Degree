//
// Created by nietup on 02.07.17.
//

#ifndef DEGREE_PART_H
#define DEGREE_PART_H

#include "TreeNode.h"

enum BoolPlus {NO, YES, DNC};

class Part {
public:
    std::pair<std::weak_ptr<Atom>, std::weak_ptr<Atom>> atoms;
    std::vector<BoolPlus> constraints;
};


#endif //DEGREE_PART_H
