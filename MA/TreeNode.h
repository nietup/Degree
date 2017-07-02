//
// Created by nietup on 02.07.17.
//

#ifndef DEGREE_TREENODE_H
#define DEGREE_TREENODE_H

#include "Atom.h"

class TreeNode {
public:
    std::weak_ptr<Atom> atom;
    std::vector<std::weak_ptr<Atom>> discardedAtoms;
    std::vector<std::weak_ptr<LineWrap>> discardedSegments;
};


#endif //DEGREE_TREENODE_H
