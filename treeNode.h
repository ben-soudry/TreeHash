//
// Created by ben on 2/23/19.
//
#include <array>
#include <iostream>

#ifndef TREEHASH_TREENODE_H
#define TREEHASH_TREENODE_H

#endif //TREEHASH_TREENODE_H

class TreeNode
    {
    public:
        TreeNode();

        //Children of this node
        std::array<std::array<TreeNode*, 2> ,2> child;

        float P_xy; //Probability of reaching this node given a pair is distributed ~ P
        float Q_xy; //Probability of reaching this node given a pair is distributed ~ Q


        bool isReject;

        bool isBucket;
        uint64_t bucketId;
        ~TreeNode();
    };
