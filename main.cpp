#include "treeHash.h"
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <math.h>
#include <chrono>
#include <memory>
#include <unordered_map>
void testTreeHash(int S, int M, int N, double C, int b){
  std::cout << "========================================================"<< std::endl;
  std::cout << "Testing TreeHash with S = " << S << " N = " << N << " C = " << C << " b = " << b << std::endl;
  std::cout << "========================================================"<< std::endl;


  std::cout << "Constructing Tree" << std::endl;
  auto startTime = std::chrono::system_clock::now();

  TreeHash::prob P_xy;
  P_xy[0] = {{ 0.215, 0.0025}};
  P_xy[1] = {{0.255, 0.5275}};


  TreeHash* treeHash = new TreeHash(C, S, M, N, P_xy);

  auto constructionTime = std::chrono::system_clock::now();
  std::chrono::duration<double> construction_seconds = constructionTime-startTime;
  std::cout << "Constructed Tree in "  << construction_seconds.count() << " seconds" << std::endl;

  std::cout << "Generating Samples" << std::endl;
  auto sampleStartTime = std::chrono::system_clock::now();

  //Generate Random Samples:
  std::random_device rd;  //Will be used to obtain a seed for the random number engine

  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  auto dice = std::bind (dis, gen);


  std::vector<TreeHash::bitvec> X;
  std::vector<TreeHash::bitvec> Y;

  for(int i = 0; i < N; i++){
    TreeHash::bitvec Xi;
    TreeHash::bitvec Yi;
    Xi.vec = std::vector<bool>(S, 0);
    Yi.vec = std::vector<bool>(S, 0);
    for(int j = 0; j < S; j++) {
      //Give Xi, Yi a common id.
      Xi.id = i;
      Yi.id = i;
      //Sample the bit
      double R = dice();
      if (R < P_xy[0][0]) {
        Xi.vec[j] = 0;
        Yi.vec[j] = 0;
      } else if (R < P_xy[0][1] + P_xy[0][0]) {
        Xi.vec[j] = 0;
        Yi.vec[j] = 1;
      } else if (R < P_xy[1][0] + P_xy[0][1] + P_xy[0][0]) {
        Xi.vec[j] = 1;
        Yi.vec[j] = 0;
      } else {
        Xi.vec[j] = 1;
        Yi.vec[j] = 1;
      }
    }
    X.push_back(Xi);
    Y.push_back(Yi);
  }
  auto sampleEndTime = std::chrono::system_clock::now();
  std::chrono::duration<double> sample_seconds = sampleEndTime-sampleStartTime;
  std::cout << "Generated Samples in "  << sample_seconds.count() << " seconds" << std::endl;

  std::cout << "Hashing" << std::endl;
  auto hashingStartTime = std::chrono::system_clock::now();


  std::unique_ptr<std::vector<std::vector<TreeHash::bitvec>>> bucketsX(new std::vector<std::vector<TreeHash::bitvec>>);
  std::unique_ptr<std::vector<std::vector<TreeHash::bitvec>>> bucketsY(new std::vector<std::vector<TreeHash::bitvec>>);



  treeHash->hash(X, Y, bucketsX.get(), bucketsY.get(), b);
  auto hashingEndTime = std::chrono::system_clock::now();
  std::chrono::duration<double> hash_seconds = hashingEndTime-hashingStartTime;
  std::cout << "Finished Hashing in "  << hash_seconds.count() << " seconds" << std::endl;

  std::cout << "Matching Pairs" << std::endl;
  auto matchingStartTime = std::chrono::system_clock::now();

  int matchedPairs = 0;
  int unmatchedPairs = 0;
  //Check all pairs in each bucket for matches:
  std::unordered_map<int, bool> visited;

  for(int i = 0; i < (*bucketsX).size(); i++){
    //compare all pairs in the ith bucket
    for(int j = 0; j < (*bucketsX)[i].size(); j++){
      for(int k = 0; k < (*bucketsY)[i].size(); k++){
        if((*bucketsX)[i][j].id == (*bucketsY)[i][k].id){
            if(visited.find((*bucketsX)[i][j].id) == visited.end()){
                visited.insert(std::make_pair((*bucketsX)[i][j].id, true));
                matchedPairs += 1;
            }
        } else {
          unmatchedPairs += 1;
        }
      }
    }
  }




  auto endTime = std::chrono::system_clock::now();
  std::chrono::duration<double> matching_seconds = endTime-matchingStartTime;
  std::cout << "Finished Matching in "  << matching_seconds.count() << " seconds" << std::endl;

  std::chrono::duration<double> elapsed_seconds = endTime-startTime;

  std::cout << "Time Elapsed: " << elapsed_seconds.count() << " seconds" << std::endl;
  std::cout << "Matched Pairs: " << matchedPairs << std::endl;
  std::cout << "Unmatched Pairs: " << unmatchedPairs << std::endl;
  std::cout << "True Positive:" << ((double) matchedPairs) / ((double) N) << std::endl;

  std::cout << "Probability Sum: "<< treeHash->bucketProbSum << std::endl;
  std::cout << "(M+N)/C: "<< ((double)(M+N))/C << std::endl;

  delete treeHash;
}

int main(){
  int S = 2000;

  int M = 10000;
  int N = 10000;
  double C;
  int b;

  //M,N = 10^4
  std::cout << "M,N = 10^4 " << std::endl;

  C = (double) N;
  b = 100;
  //testTreeHash(S, M, N, C, b);

  C = (double) 10*N;
  b = 100;
  //testTreeHash(S, M, N, C, b);

  C = (double) 100*N;
  b = 42;
  testTreeHash(S, M, N, C, b);

  b = 3;
  C = (double) 1000*N;
  testTreeHash(S, M, N, C, b);

  b = 1;
  C = (double) 10000*N;
  testTreeHash(S, M, N, C, b);

  M = 100000;
  N = 100000;
  //M,N = 10^5
  std::cout << "M,N = 10^5 " << std::endl;

  C = (double) N;
  b = 100;
  //testTreeHash(S, M, N, C, b);

  C = (double) 10*N;
  b = 100;
  //testTreeHash(S, M, N, C, b);

  C = (double) 100*N;
  b = 17;
  testTreeHash(S, M, N, C, b);

  C = (double) 1000*N;
  b = 2;
  testTreeHash(S, M, N, C, b);

  C = (double) 10000*N;
  b = 1;
  testTreeHash(S, M, N, C, b);

  C = (double) 100000*N;
  b = 1;
  testTreeHash(S, M, N, C, b);

  M = 1000000;
  N = 1000000;
  //M,N = 10^6
  std::cout << "M,N = 10^6 " << std::endl;
  C = (double) N;
  b = 100;
  testTreeHash(S, M, N, C, b);

  C = (double) N*10;
  b = 100;
  testTreeHash(S, M, N, C, b);

  C = (double) N*100;
  testTreeHash(S, M, N, C, b);

  C = (double) N*1000;
  testTreeHash(S, M, N, C, b);

  C = (double) N*10000;
  testTreeHash(S, M, N, C, b);

  C = (double) N*100000;
  testTreeHash(S, M, N, C, b);

  C = (double) N*1000000;
  testTreeHash(S, M, N, C, b);
}
