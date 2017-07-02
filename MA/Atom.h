//
// Created by nietup on 02.07.17.
//

#ifndef DEGREE_ATOM_H
#define DEGREE_ATOM_H

#include "LineWrap.h"
class Part;

class Atom {
public:
    std::weak_ptr<LineWrap> asignment;

    //should be sorted by most constrained first
    std::vector<std::weak_ptr<Part>> involved;
};


#endif //DEGREE_ATOM_H
