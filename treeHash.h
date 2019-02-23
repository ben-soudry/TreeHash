#include <vector>
#include <array>
#include <algorithm>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include "treeNode.h"


#ifndef TREEHASH_TREEHASH_H
#define TREEHASH_TREEHASH_H
#endif //TREEHASH_TREEHASH_H

class TreeHash 
{
    public:

    typedef std::array<std::array<float, 2>,2> prob;
    typedef std::vector<bool> bitvec;
    TreeHash(float C, int S, const std::vector<TreeHash::prob>& P_xy);
    
    void hash(std::vector<bitvec>& X, std::vector<TreeHash::bitvec>& Y,
              std::vector<std::vector<bitvec>>* bucketsX,
              std::vector<std::vector<bitvec>>* bucketsY);
    
    float C; //bucket cutoff parameter
    int S; //length of the bit strings
    
    //The elements of P are P(x_i,y_i) in 2x2 matricies
    const std::vector<prob> P_xy;
    
    uint64_t numBuckets;
         
        
    private:

    void constructTree(TreeNode* node, int i);
    
    void hashRecursive(TreeNode* node, 
                       const std::vector<TreeHash::bitvec>& X, 
                       const std::vector<TreeHash::bitvec>& Y, 
                       std::vector<TreeHash::bitvec>::iterator X_begin,
                       std::vector<TreeHash::bitvec>::iterator X_end,
                       std::vector<TreeHash::bitvec>::iterator Y_begin,
                       std::vector<TreeHash::bitvec>::iterator Y_end,
                       std::vector<std::vector<TreeHash::bitvec>>* bucketsX,
                       std::vector<std::vector<TreeHash::bitvec>>* bucketsY,
                       int stringIndex);
     
    
    TreeNode* root;
    uint64_t currBucketId;
};
