#include "treeHash.h"
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <math.h>
#include <chrono>
#include <memory>
void testTreeHash(int S, int M, int N, double C, auto dice){
  std::cout << "========================================================"<< std::endl;
  std::cout << "Testing TreeHash with S = " << S << " N = " << N << " C = " << C << std::endl;
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



  treeHash->hash(X, Y, bucketsX.get(), bucketsY.get());

  int matchedPairs = 0;
  int unmatchedPairs = 0;
  //Check all pairs in each bucket for matches:
  for(int i = 0; i < (*bucketsX).size(); i++){
    //compare all pairs in the ith bucket
    for(int j = 0; j < (*bucketsX)[i].size(); j++){
      for(int k = 0; k < (*bucketsY)[i].size(); k++){
        if((*bucketsX)[i][j].id == (*bucketsY)[i][k].id){
          matchedPairs += 1;
        } else {
          unmatchedPairs += 1;
        }
      }
    }
  }
  auto endTime = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = endTime-startTime;

  std::chrono::duration<double> hash_seconds = endTime-hashingStartTime;
  std::cout << "Finished Hashing in "  << hash_seconds.count() << " seconds" << std::endl;


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

  //Generate Random Samples:
  std::random_device rd;  //Will be used to obtain a seed for the random number engine

  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  auto dice = std::bind (dis, gen);

  //M,N = 10^4
  std::cout << "M,N = 10^4 " << std::endl;

  double C = (double) N;
  //testTreeHash(S, M, N, C, dice);

  C = (double) 10*N;
  //testTreeHash(S, M, N, C, dice);

  C = (double) 100*N;
 //testTreeHash(S, M, N, C, dice);

  C = (double) 1000*N;
  //testTreeHash(S, M, N, C, dice);

  C = (double) 10000*N;
  //testTreeHash(S, M, N, C, dice);

  M = 100000;
  N = 100000;
  //M,N = 10^5
  std::cout << "M,N = 10^5 " << std::endl;

  C = (double) N;
  testTreeHash(S, M, N, C, dice);

  C = (double) 10*N;
  testTreeHash(S, M, N, C, dice);

  C = (double) 100*N;
  testTreeHash(S, M, N, C, dice);

  C = (double) 1000*N;
  testTreeHash(S, M, N, C, dice);

  C = (double) 10000*N;
  testTreeHash(S, M, N, C, dice);

  C = (double) 100000*N;
  testTreeHash(S, M, N, C, dice);

  M = 1000000;
  N = 1000000;
  //M,N = 10^6
  std::cout << "M,N = 10^6 " << std::endl;
  C = (double) N;
  testTreeHash(S, M, N, C, dice);

  C = (double) N*10;
  testTreeHash(S, M, N, C, dice);

  C = (double) N*100;
  testTreeHash(S, M, N, C, dice);

  C = (double) N*1000;
  testTreeHash(S, M, N, C, dice);

  C = (double) N*10000;
  testTreeHash(S, M, N, C, dice);

  C = (double) N*100000;
  testTreeHash(S, M, N, C, dice);

  C = (double) N*1000000;
  testTreeHash(S, M, N, C, dice);
}
