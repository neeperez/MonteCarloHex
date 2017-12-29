//This program should use the old graph implementation to store 3 types of variables:
//R (representing red), B (representing blue), W (unoccupied, whitespace)
#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <list>
#include <utility>
#include <limits>
#include <queue>
#include <stack>
#include "Graph.hpp"
using namespace std;

int WON = 0; //This global variable will keep track of who won the game. If it is 1, red wins. If 2, blue has won.

//Overloads << operator to print out the graph

int main(){
	Graph* board = new Graph(6);
	board->print();
	board->set_edge_valueNS(3,3,1);
	board = board->MonteCarlo(2);
	board->print();
	if(board->WON == 1){
		cout << "RED wins" << endl;
	} else {
		cout << "BLUE wins" << endl;
	}
	delete board;
	return 0;
}
