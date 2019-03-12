

all: treeHash-test

treeHash-test: main.o treeHash.o treeNode.o
	g++ -std=c++11 main.o treeHash.o treeNode.o -o treeHash-test

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

treeHash.o: treeHash.cpp
	g++ -std=c++11 -c treeHash.cpp

treeNode.o: treeNode.cpp
	g++ -std=c++11 -c treeNode.cpp
