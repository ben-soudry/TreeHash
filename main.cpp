#include "treeHash.h"
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

int main(){
    int S = 12;
    float C = 40;

    std::vector<TreeHash::prob> P_xy(S);
    TreeHash::prob testMatrix;

    testMatrix[0] = {{0.9,0.1}};
    testMatrix[1] = {{0.1,0.9}};
    std::cout << testMatrix[0][0] << std::endl;

    for(auto it = P_xy.begin(); it != P_xy.end(); ++it){
        *it  = testMatrix;
    }
    float wtf = P_xy.at(0)[0][0];

    TreeHash* t = new TreeHash(C, S, P_xy);


    return 0;
}
