#include <vector>
#include <array>
#include <algorithm>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include "treeNode.h"
#include <numeric>
#include <functional>
#include <iterator>

#ifndef TREEHASH_TREEHASH_H
#define TREEHASH_TREEHASH_H
#endif //TREEHASH_TREEHASH_H

class TreeHash
{
    public:

    typedef std::array<std::array<double, 2>,2> prob;
    typedef struct bitvec{
        int id;
        std::vector<bool> vec;
        std::vector<int>* perm;
        bool operator < (const struct bitvec& other) const{
          //std::cout << "perm: " << perm << std::endl;
          //std::cout << "perm len: " << perm->size() << " vec len: " << vec.size() << std::endl;
          //std::cout << "perm[0] " << (*perm)[0] << std::endl;

          for(int i = 0; i < this->vec.size(); ++i){
            if(this->vec[(*perm)[i]] < other.vec[(*perm)[i]]){
              return true;
            }
            if(this->vec[(*perm)[i]] > other.vec[(*perm)[i]]){
              return false;
            }
          }
          //The vectors are equal
          return false;
        }

    } bitvec;

    TreeHash(double C, int S, int N, const TreeHash::prob& P_xy);
    ~TreeHash();

    void hash(std::vector<bitvec>& X, std::vector<TreeHash::bitvec>& Y,
              std::vector<std::vector<bitvec>>* bucketsX,
              std::vector<std::vector<bitvec>>* bucketsY, int b);

    double C; //bucket cutoff parameter
    int S; //length of the bit strings
    int N; //Number of Xs/ Ys

    double bucketProbSum;

    //The elements of P are P(x_i,y_i) in 2x2 matricies
    const TreeHash::prob P_xy;
    TreeHash::prob Q_xy;

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
                       int index,
                       std::vector<int>* indexPerm);



    TreeNode* root;
    uint64_t currBucketId;
};
