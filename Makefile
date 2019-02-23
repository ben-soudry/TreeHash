

all: treeHash-test

treeHash-test: main.o treeHash.o treeNode.o
	g++ -std=c++14 main.o treeHash.o treeNode.o -o treeHash-test

main.o: main.cpp
	g++ -std=c++14 -c main.cpp

treeHash.o: treeHash.cpp
	g++ -std=c++14 -c treeHash.cpp

treeNode.0: treeNode.cpp
	g++ -std=c++14 -c treeNode.cpp
