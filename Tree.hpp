// Header file for the Tree ADT
#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <vector>
#include <algorithm>
#include "Graph.hpp"
using namespace std;

class Node{
public:
	int nodeID;
	int parentID;
	int depth;
	int heuristic;
	Graph* position;

	Node():nodeID(0), parentID(0), depth(0), position(nullptr){}

	Node(Graph* G): nodeID(0), parentID(0), depth(0), position(G), heuristic(0){}

	Node(Graph* G, int depth): nodeID(0), parentID(0), depth(depth), position(nullptr), heuristic(0){
		position = G->copy();
	}

	void newNode(){
		nodeID = 0;
		parentID = -1;
		depth = 0;
		heuristic = 0;
		position = nullptr;
	}

	~Node(){
		delete position;
	}
	
};

class Tree{
public:
	vector<vector<Node*>> adj;
	Tree(Node* root, int size);
	Tree(int size);
	~Tree();
	void buildTree(Node* node, int ply);
	int alphabeta(Node* node, int depth, int alpha, int beta, bool maxPlayer);
	void insertChild(Node* p, Node* c);
	int evaluate(Node* node);
	void clear(Node* node);
private:
	int order;
	Node* root; 
	int total_nodes = 0;
	vector<pair<int,int>> nodeLP;
};

#endif