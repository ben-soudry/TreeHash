//
// Created by ben on 2/23/19.
//
#include <array>

#ifndef TREEHASH_TREENODE_H
#define TREEHASH_TREENODE_H

#endif //TREEHASH_TREENODE_H

class TreeNode
    {
    public:
        TreeNode();
        //Children of this node
        std::array<std::array<TreeNode*, 2> ,2> child;

        float P_xy; //Probability of reaching this node - 1 for the root

        bool isReject;

        bool isBucket;
        uint64_t bucketId;
    };
