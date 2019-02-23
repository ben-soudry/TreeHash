cmake_minimum_required(VERSION 3.13)
project(TreeHash)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(TreeHash
        main.cpp
        Makefile
        treeHash.cpp
        treeHash.h
        treeNode.cpp
        treeNode.h)
