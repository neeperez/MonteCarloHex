#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <list>
#include <utility>
#include <limits>
#include <queue>
#include <stack>
using namespace std;

class Graph{
public:
	int WON;
	int turn_count;
	pair<int,int> latest_move;
	Graph(); //Default constructor
	Graph(int size);
	~Graph();
	int V();
	int E();
	bool adjacent(int x, int y);
	vector<int> neighbors(int x);
	void add(int x, int y, int edge_value);
	void remove(int x, int y);
	void set_edge_value(int x, int y, double v);
	void set_edge_valueNS(int x, int y, double v);
	int get_edge_value(int x, int y);
	vector<pair<int,int>> neighbors(int i, int j, int color);
	bool hasWon(int color);
	void print();
	void randomize();
	Graph* copy();
	Graph* MonteCarlo(int color);
	double MC_Ratio();
private:
	int** matrix;
	double density;
	int num_vertices;
	int num_edges;
	double low;
	double high;
};

#endif