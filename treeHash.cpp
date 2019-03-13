#include "treeHash.h"

TreeHash::TreeHash(double C, int S, int M, int N, const TreeHash::prob& P_xy)
 : C(C), S(S), N(N), M(M), P_xy(P_xy){

    //Calculate Q_xy - assumes, x, y distributed independently
    this->Q_xy[0][0] = (P_xy[0][0]+P_xy[0][1])*(P_xy[0][0]+P_xy[1][0]);
    this->Q_xy[1][0] = (P_xy[1][0]+P_xy[1][1])*(P_xy[0][0]+P_xy[1][0]);
    this->Q_xy[0][1] = (P_xy[0][0]+P_xy[0][1])*(P_xy[0][1]+P_xy[1][1]);
    this->Q_xy[1][1] = (P_xy[1][0]+P_xy[1][1])*(P_xy[0][1]+P_xy[1][1]);

    //Construct the Hashing Tree
    root = new TreeNode();
    root->P_xy = 1.0;
    root->Q_xy = 1.0;

    currBucketId = 0;
    bucketProbSum = 0.0;
    root->isBucket = false;
    root->isReject = false;

    //Start hashing recursively
    constructTree(root, 0);
    std::cout << "Finished constructing tree" << std::endl;
    numBuckets = currBucketId;
}
TreeHash::~TreeHash(){
  delete root;
}


void TreeHash::constructTree(TreeNode* node, int i)
{
    //std::cout << "Depth" << i << std::endl;
    for(int x = 0; x < 2; x++){
        for(int y = 0; y < 2; y++){
            TreeNode* childNode = new TreeNode();
            childNode->P_xy = node->P_xy*P_xy[x][y];
            childNode->Q_xy = node->Q_xy*Q_xy[x][y];
            node->child[x][y] = childNode;
            if(node->P_xy*P_xy[x][y] < 1/C) {
                 //Prune the tree here, TreeHash::probability is too low
                 //std::cout << "Prune! " << std::endl;
                 childNode->isReject = true;
             } else if(1+M*N*node->Q_xy < C*node->P_xy || i == S) {
                //Make a bucket
                //std::cout << "Bucket " << currBucketId << std::endl;
                childNode->isBucket = true;
                childNode->bucketId = currBucketId;
                bucketProbSum += childNode->P_xy;
                currBucketId++;
            } else {
                 //regular child node
                 childNode->isBucket = false;
                 childNode->isReject = false;
                 constructTree(childNode, i+1);
             }
        }
    }
}

void TreeHash::hash(std::vector<TreeHash::bitvec>& X,
               std::vector<TreeHash::bitvec>& Y,
               std::vector<std::vector<TreeHash::bitvec>>* bucketsX,
               std::vector<std::vector<TreeHash::bitvec>>* bucketsY,
               int b){

    //Hash with b bands, the number of permutations of the strings to use.

    std::vector<int>* perm = new std::vector<int>(S, 0);


    for(int j = 0; j < S; j++){
      (*perm)[j] = j;
    }

    for(int i = 0; i < b; i++){

      for(int k = 0; k < X.size(); k++){
        X[k].perm = perm;
      }
      for(int k = 0; k < Y.size(); k++){
        Y[k].perm = perm;
      }
      std::random_shuffle(perm->begin(), perm->end());
      //Sort X, Y - according to the current permutation, see bitvec operator < for details
      //std::cout << "theperm: "<< perm << std::endl;
      std::sort(X.begin(), X.end());
      std::sort(Y.begin(), Y.end());

      auto X_begin = X.begin();
      auto X_end = X.end();
      auto Y_begin = Y.begin();
      auto Y_end = Y.end();

      //Start hashing recursively
      this->hashRecursive(this->root, X, Y, X_begin, X_end, Y_begin, Y_end, bucketsX, bucketsY, 0, perm);
    }
    delete perm;
}

void TreeHash::hashRecursive(TreeNode* node,
                const std::vector<TreeHash::bitvec>& X,
                const std::vector<TreeHash::bitvec>& Y,
                std::vector<TreeHash::bitvec>::iterator X_begin,
                std::vector<TreeHash::bitvec>::iterator X_end,
                std::vector<TreeHash::bitvec>::iterator Y_begin,
                std::vector<TreeHash::bitvec>::iterator Y_end,
                std::vector<std::vector<TreeHash::bitvec>>* bucketsX,
                std::vector<std::vector<TreeHash::bitvec>>* bucketsY,
                int index,
                std::vector<int>* indexPerm) {

    int stringIndex = (*indexPerm)[index];
    //std::cout << "hashRecursive X(" <<  X_begin-X.begin()  <<  " , " << X_end-X.begin()
    //         << ") Y(" << Y_begin - Y.begin() << " , " << Y_end - Y.begin() << ") " << stringIndex << std::endl;
    //Base cases, bucket or reject:
    if(node->isBucket){
        //copy the remaining X, Y into the buckets
        //std::cout << "filled bucket #" << node->bucketId << " with "
        //<< std::distance(X_begin, X_end)*std::distance(X_begin, X_end) << " pairs" << std::endl;
        auto x = std::vector<TreeHash::bitvec>(X_begin, X_end);
        auto y = std::vector<TreeHash::bitvec>(Y_begin, Y_end);

        bucketsX->push_back(x);
        bucketsY->push_back(y);
        return;
    }
    if(node->isReject){
        return;
    }
    if(stringIndex >= X[0].vec.size()){
        //std::cout << "Error string len exceeded " << std::endl;
        return;
    }

    //auto X_mid = X_begin;
    //auto Y_mid = Y_begin;
    auto compareLambda = [=](const TreeHash::bitvec &a, const TreeHash::bitvec &b) -> bool
    {
      return a.vec[stringIndex] < b.vec[stringIndex];
    };
    TreeHash::bitvec one;
    one.vec = std::vector<bool>(S, 0);
    one.vec[stringIndex] = 1;

    //Find first value in X, Y that starts with 1 at string index
    auto X_mid = std::lower_bound(X_begin, X_end, one, compareLambda);
    auto Y_mid = std::lower_bound(Y_begin, Y_end, one, compareLambda);

    /*
    while(X_mid != X_end) {
        if((*X_mid).vec[stringIndex] == 1){
            break;
        }
        ++X_mid;
    }
    while(Y_mid != Y_end) {
        if((*Y_mid).vec[stringIndex] == 1){
            break;
        }
         ++Y_mid;
    }*/

    //For each child, recurse using X0, X1, Y0, Y1 if data points remain
    if(X_begin != X_mid && Y_begin != Y_mid){
        hashRecursive(node->child[0][0], X, Y, X_begin, X_mid, Y_begin, Y_mid,
                      bucketsX, bucketsY, index+1, indexPerm);
    }
    if(X_begin != X_mid && Y_end != Y_mid){
        hashRecursive(node->child[0][1], X, Y, X_begin, X_mid, Y_mid, Y_end,
                      bucketsX, bucketsY, index+1, indexPerm);
    }
    if(X_end != X_mid && Y_begin != Y_mid){
        hashRecursive(node->child[1][0], X, Y, X_mid, X_end, Y_begin, Y_mid,
                      bucketsX, bucketsY, index+1, indexPerm);
    }
    if(X_end != X_mid && Y_end != Y_mid){
        hashRecursive(node->child[1][1], X, Y, X_mid, X_end, Y_mid, Y_end,
                      bucketsX, bucketsY, index+1, indexPerm);
    }
}
