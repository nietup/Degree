//
// Created by nietup on 02.07.17.
//

#ifndef DEGREE_TREENODE_H
#define DEGREE_TREENODE_H

#include "Vertex.h"

class TreeNode {
public:
    std::weak_ptr<Vertex> vertex;
    std::vector<std::weak_ptr<Vertex>> discardedVertices;
    std::vector<std::weak_ptr<LineWrap>> discardedSegments;
};


#endif //DEGREE_TREENODE_H
