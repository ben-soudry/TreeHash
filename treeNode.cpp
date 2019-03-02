#include "treeNode.h"

TreeNode::TreeNode(): P_xy(1.0), Q_xy(1.0), isReject(false), isBucket(false){
  //std::cout<< "make tree"<< std::endl;
  child[0][0] = NULL;
  child[0][1] = NULL;
  child[1][0] = NULL;
  child[1][1] = NULL;
}

TreeNode::~TreeNode(){

  //std::cout << "wtf: " << this  << std::endl;
  //std::cout<< "delete tree, isBucket: " << isBucket << " isReject: " << isReject << std::endl;

  delete child[0][0];
  delete child[0][1];
  delete child[1][0];
  delete child[1][1];
}

