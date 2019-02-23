#include "treeHash.h"

TreeHash::TreeHash(float C, int S, const std::vector<TreeHash::prob>& P_xy)
 : C(C), S(S), P_xy(P_xy){
    //this->C = C;
    //this->S = S;
    //this->P_xy = P_xy;


    //Construct the Hashing Tree
    root = new TreeNode();
    root->P_xy = 1.0;
    
    currBucketId = 0;

    constructTree(root, 0);
    std::cout << "Finished!" << std::endl;
    numBuckets = currBucketId;
}

void TreeHash::constructTree(TreeNode* node, int i)
{
    std::cout << "This ran" << i << std::endl;
    for(int x = 0; x < 2; x++){
        for(int y = 0; y < 2; y++){
             if(node->P_xy*P_xy[i][x][y] > 1/C){
                TreeNode* childNode = new TreeNode();
                float p1 = node->P_xy*P_xy[i][x][y];
                childNode->P_xy = p1;
                node->child[x][y] = childNode;
                if(i < S-1){
                    //More bits remain, recurse
                    constructTree(childNode, i+1);
                } else {
                    //Reached the end of the string, make a bucket
                    std::cout << "Bucket " << currBucketId << std::endl;
                    childNode->isBucket = true;
                    childNode->bucketId = currBucketId;
                    currBucketId++; 
                }
            } else {
                //Prune the tree here, TreeHash::probability is too low
                std::cout << "Prune! " << std::endl;
                TreeNode* rejectNode = new TreeNode();
                rejectNode->isReject = true;
                node->child[x][y] = rejectNode;
            }
        }
    }
}

void TreeHash::hash(std::vector<TreeHash::bitvec>& X,
               std::vector<TreeHash::bitvec>& Y,
               std::vector<std::vector<TreeHash::bitvec>>* bucketsX,
               std::vector<std::vector<TreeHash::bitvec>>* bucketsY){
    //Sort X, Y
    std::sort(X.begin(), X.end());
    std::sort(Y.begin(), Y.end());

    //Make empty buckets
    bucketsX->assign(numBuckets, std::vector<TreeHash::bitvec>());
    bucketsY->assign(numBuckets, std::vector<TreeHash::bitvec>());
   
    auto X_begin = X.begin(); 
    auto X_end = X.end(); 
    auto Y_begin = Y.begin(); 
    auto Y_end = Y.end(); 
    //Start hashing recursively
    this->hashRecursive(this->root, X, Y, X_begin, X_end, Y_begin, Y_end, bucketsX, bucketsY, 0); 
    
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
                int stringIndex) {

    //Base cases, bucket or reject:
    if(node->isBucket){
        //copy the remaining X, Y into the buckets
        bucketsX->at(node->bucketId) = std::vector<TreeHash::bitvec>(X_begin, X_end);     
        bucketsY->at(node->bucketId) = std::vector<TreeHash::bitvec>(Y_begin, Y_end);
        return;
    }     
    if(node->isReject){
        return;
    }
    if(stringIndex >= X[0].size()){
        std::cout << "Error string len exceeded " << std::endl;
        return;
    } 
    //Find first value in X, Y that starts with 1
    std::vector<std::vector<bool>>::iterator X_mid;
    std::vector<std::vector<bool>>::iterator Y_mid;

    while(X_mid != X.end()) {
        if((*X_mid)[stringIndex] == 1){
            break;
        }
        ++X_mid;
    }
    while(Y_mid != Y.end()) {
        if((*Y_mid)[stringIndex] == 1){
            break;
        }
         ++Y_mid;
    }

    //For each child, recurse using X0, X1, Y0, Y1 if data points remain
    if(X_begin != X_mid && Y_begin != Y_mid){        
        hashRecursive(node->child[0][0], X, Y, X_begin, X_mid, Y_begin, Y_mid,
                      bucketsX, bucketsY, stringIndex+1); 
    }
    if(X_begin != X_mid && Y_end != Y_mid){
        hashRecursive(node->child[0][1], X, Y, X_begin, X_mid, Y_mid, Y_end, 
                      bucketsX, bucketsY, stringIndex+1);
    }
    if(X_end != X_mid && Y_begin != Y_mid){
        hashRecursive(node->child[1][0], X, Y, X_mid, X_end, Y_begin, Y_mid,
                      bucketsX, bucketsY, stringIndex+1);
    } 
    if(X_end != X_mid && Y_end != Y_mid){
        hashRecursive(node->child[1][1], X, Y, X_mid, X_end, Y_mid, Y_end, 
                      bucketsX, bucketsY, stringIndex+1);
    }
}
