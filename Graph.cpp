//Graph ADT that will be used to store the hex board.
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

//Here the way that Graph is used has changed. Instead of storing the distances between two vertices, we're using it to represent 
//The hex board. If a position i,j has 0 stored, it is unoccupied. If a position has 1 stored, then red is occupying the space.
//If a position has 2 stored in it, then blue is occupying the space.

Graph::Graph():matrix(nullptr),density(0.0){}; //Default constructor


//These given parameters allow us to create a random list
/*Graph(double density, double low, double high, int size):
	matrix(nullptr), density(density), low(low), high(high), num_vertices(size){randomize(density, low, high, num_vertices);};*/

Graph::Graph(int size): num_vertices(size){
	matrix = new int*[num_vertices];
	for(int k = 0; k < num_vertices; k++){
		matrix[k] = new int[num_vertices];
	}
	for(int i = 0; i < num_vertices; i++){
		for(int j = i; j < num_vertices; j++){
			matrix[i][j] = matrix[j][i] = 0;
		}
	}
	num_edges = 0;
	turn_count = 0;
	WON = 0;
}

//Destructor
Graph::~Graph(){
	for(int i = 0; i < num_vertices; ++i){
		delete [] matrix[i];
	}
	delete [] matrix;
}

// Return the number of vertices/size of the list
int Graph::V(){
	return num_vertices;
}

// Return the number of edges in the graph
int Graph::E(){
	return num_edges;
}

// Tests whether there is an edge from node x to node y
bool Graph::adjacent(int x, int y){
	//Will have to calculate for x-1 since the matrix entries begin from 0 while the
	//Vertex nunmbers start from 1
	if(matrix[x - 1][y - 1] > 0.0) {
		return true;
	}
	return false;
}

// Return a vector of all of the neighbors of node x
vector<int> Graph::neighbors(int x){
	vector<int> neighbor;
	for(int i = 0; i < num_vertices; i++){
		if(matrix[x-1][i] > 0.0){
			neighbor.push_back(i+1);
		}
	}
	//cout << "There are " << neighbor.size() << "neighbors of " << x << endl;
	return neighbor;
}

// Add an edge between node x and node y
void Graph::add(int x, int y, int edge_value){
	if(matrix[x-1][y-1] == -1.0){
		matrix[x-1][y-1] = matrix[y-1][x-1] = edge_value;
		num_edges++;
	}
}


// Remove an edge between nodes x and y
void Graph::remove(int x, int y){
	if(matrix[x-1][y-1] > 0.0){
		matrix[x-1][y-1] = matrix[y-1][x-1] = -1.0;
		num_edges--;
	}
}

// Set the value of an edge between nodes x and y
void Graph::set_edge_value(int x, int y, double v){
	matrix[x-1][y-1] = matrix[y-1][x-1] = v;
}

//NonSymmetrical set edge value 
void Graph::set_edge_valueNS(int x, int y, double v){
	matrix[x-1][y-1] = v;
	num_edges++;
	turn_count++;
	latest_move = make_pair(x,y);
}
// Get the value of an edge between x and y
int Graph::get_edge_value(int x, int y){
	return (matrix[x - 1][y - 1]);
}

//A method that tests the adjacent neighbors to a certain position.
//Parameters, the position (i,j) of the hex square and its color
//Return value: a vector of pairs which indicate the positions of the adjacent squares
vector<pair<int,int>> Graph::neighbors(int i, int j, int color){
	vector<pair<int,int>> neighbor;
	//The lower right corner (southeast) has no neighbors
	if(i == num_vertices && j == num_vertices){
		return neighbor;
	}
	if((j + 1 < num_vertices+1) && matrix[i-1][j] == color){
		neighbor.push_back(make_pair(i,j+1));
	}
	if((j - 1 > 0) && matrix[i-1][j-2] == color){
		neighbor.push_back(make_pair(i,j-1));
	}
	if((i + 1 < num_vertices+1) && matrix[i][j-1] == color){
		neighbor.push_back(make_pair(i+1,j));
	}
	if((i - 1 > 0) && matrix[i-2][j-1] == color){
		neighbor.push_back(make_pair(i-1,j));
	}
	if((i + 1 < num_vertices+1 && j - 1 > 0) && matrix[i][j-2] == color){
		neighbor.push_back(make_pair(i+1,j-1));
	}
	if((i - 1 > 0 && j + 1 < num_vertices+1) && matrix[i-2][j] == color){
		neighbor.push_back(make_pair(i-1,j+1));
	}
	return neighbor;
}

//This algorithm uses some variation of DFS to check to see if there is a path from one
//end of the board to the other, and returning the number of the winner.
//In this program, we will only test to see if red won, since the test is only used after
//all of the hexes are populated.
bool Graph::hasWon(int color){
	stack<pair<int,int>> hexes; //This is the overall stack that keeps track of all of the hexes that possibly create a path from one end of the board to the other
	bool s_found = false; //This sees if we have found a hex of the same color at the southmost/eastmost row/col
	int count = 1;
	vector<bool> been_visited((num_vertices*num_vertices) + 1, false); //This keeps track of all of the hexes that have been visited by the search. 0-121, 0 will not be checked.
	
	pair<int,int> curr_hex; //This keeps track of the hex currently being tested
	stack<pair<int,int>> parent_hex;// This is a stack that contains the parent hexes of the current hex. This helps us keep track of which hexes can be considered "real" neighbors
	pair<int,int> null_hex = make_pair(0,0);// Initial parent hex is basically a sentinel value.
	parent_hex.push(null_hex);
	vector<pair<int,int>> n_hex; //This keeps track of all of the neighboring hexes of the current hex, including the parent hex.
	if(color == 1){

		//We first need to check to see if there are hexes in the northmost and southmost row
		while(count < num_vertices+1){
			//cout << "R loop1" << endl; Debug stuff
			//If there is a position that is occupied by red, push it onto hexes
			if(get_edge_value(1, count) == color){
				been_visited[count] = true;
				hexes.push(make_pair(1,count));
			}
			//We have found a hex in the southmost row
			if(get_edge_value(num_vertices, count) == color){
				s_found = true;
			}
			count++;
		}
		//No southmost hex or no hexes in the northmost row
		if(!s_found || hexes.size() == 0)
			return false;

		//Now we need to find if there is a path that reaches from north to south
		while(hexes.size() != 0){
			int real_neighbors = 0; //This vector keeps track of the neighboring hexes of the curr_hex EXCLUDING the parent hex or any hex that has already been visited
			//cout << "R " << hexes.size() << endl;
			curr_hex = hexes.top();
			been_visited[((curr_hex.first-1)*num_vertices) + curr_hex.second] = true; //We have now visited the current hex
			//cout << curr_hex.first << ", " << curr_hex.second << endl;

			//We have found a path
			if(curr_hex.first == num_vertices){
				return true;
			}
			//cout << "Parent hex: " << parent_hex.top().first << ", " << parent_hex.top().second << endl;
			n_hex = neighbors(curr_hex.first,curr_hex.second, color);
			//Add all of the neighbors that are not the parent hex or any visited hex to the real neighbors and the stack
			for(auto a = n_hex.begin(); a != n_hex.end(); ++a){
				if(((*a).first != parent_hex.top().first || (*a).second != parent_hex.top().second) 
						&& !(been_visited[(((*a).first-1)*num_vertices) + (*a).second])) {
					//cout << (*a).first << ", " << (*a).second << endl;
					++real_neighbors;
					hexes.push(*a);
				}
			}

			//There are no other unexplored neighbors
			if(real_neighbors == 0){
				//cout << curr_hex.first << ", " << curr_hex.second << " has no remaining neighbors" << endl;
				//Pop off the parent hex since we have to move back in the stack
				if(parent_hex.size() != 0 && parent_hex.top() != null_hex)
					parent_hex.pop();
				//cout << "Seggg" << endl;
				//Pop the current hex off of the stack since it cannot be in the winning path
				hexes.pop();
				continue;
			}
			//Else, the current hex can now be pushed into the parent stack.
			parent_hex.push(curr_hex);
		}
		//cout << "out of R loop 2" << endl;

		//Exited loop, there is no winning path.
		return false;
	} else if (color == 2){
		//We first need to check to see if there are hexes in the northmost and southmost row
		while(count < num_vertices+1){
			//cout << "B loop1" << endl;
			//If there is a position that is occupied by red, push it onto n
			if(get_edge_value(count, 1) == color){
				been_visited[((count-1)*num_vertices) + 1] = true;
				hexes.push(make_pair(count,1));
			}
			if(get_edge_value(count, num_vertices) == color){
				s_found = true;
			}
			count++;
		}
		
		if(!s_found || hexes.size() == 0)
			return false;
		//Now we need to find if there is a path that reaches from west to east
		while(hexes.size() != 0){
			int real_neighbors = 0;
			//cout << "B " << hexes.size() << endl;
			curr_hex = hexes.top();
			been_visited[((curr_hex.first-1)*num_vertices) + curr_hex.second] = true;
			//cout << curr_hex.first << ", " << curr_hex.second << endl;
			if(curr_hex.second == num_vertices){
				return true;
			}
			//cout << "Parent hex: " << parent_hex.top().first << ", " << parent_hex.top().second << endl;
			n_hex = neighbors(curr_hex.first,curr_hex.second, color);
			//cout << "Seg1" << endl;
			//Add all of the neighbors that are not the parent hex to the stack
			for(auto a = n_hex.begin(); a != n_hex.end(); ++a){
				
				if(((*a).first != parent_hex.top().first || (*a).second != parent_hex.top().second) 
						&& !(been_visited[(((*a).first-1)*num_vertices) + (*a).second])) {
					//cout << (*a).first << ", " << (*a).second << endl;
					++real_neighbors;
					hexes.push(*a);
				}
			}
			//cout << "Seg 2" << endl;
			if(real_neighbors == 0){
				//cout << curr_hex.first << ", " << curr_hex.second << " has no remaining neighbors" << endl;
				//cout << "Seg3" << endl;
				//cout << "Seg4" << endl;
				if(parent_hex.size() != 0 && parent_hex.top() != null_hex)
					parent_hex.pop();
				//cout << "Seg5" << endl;
				hexes.pop();
				continue;
			}
			parent_hex.push(curr_hex);
		}
		//cout << "out of B loop 2" << endl;
		return false;
	}
}

//Prints out the graph, does not overload the << operator
void Graph::print(){
	cout << "   ";
	for(int n = 0; n < V(); n++)
		cout << n+1 << " ";
	cout << endl;
	for(int i = 0; i < V(); i++){
		for(int k = 0; k < i; k++)
			cout << " ";
		if(i+1 < 10){
			cout << i+1 << "  ";
		} else {
			cout << i+1 << " ";
		}
		for (int k = 0; k < V(); k++){
			if(get_edge_value(i+1,k+1) == 0){
				cout << "_";
			} else if (get_edge_value(i+1,k+1) == 1){
				cout << "R";
			} else {
				cout << "B";
			}
			cout << " ";
		}
		cout << endl;
	}
}

// Randomly generate a board 
void Graph::randomize(){
	random_device rd;
	mt19937 mt(rd());
	//turn_count = 0;
	//bool playing = true;
	vector<pair<int,int>> available; //This will contain the hexes that are eligible for a color placement
	for(int i = 0; i < num_vertices; i++){
		for(int j = 0; j < num_vertices; j++){
			if(get_edge_value(i+1,j+1) == 0)
				available.push_back(make_pair(i,j));
		}
	}
	while(turn_count < num_vertices*num_vertices){

		//It's Red's turn
		if(turn_count % 2 == 0){	
			uniform_int_distribution<int> dis(0,available.size() - 1); //Create a distribution from 0 to the last element
			int position = dis(mt); //Use the distribution to select a random index that holds the position of the hex
			pair<int,int> rand = available[position]; 
			int i = rand.first; //obtain the position
			int j = rand.second;
			set_edge_valueNS(i+1,j+1,1); //Set the value of the position to 1, representing red
			available.erase(available.begin()+position); //Remove the hex from the availablity vector
		//It's Blue's turn
		} else {
			// Same process as red
			uniform_int_distribution<int> dis(0,available.size() - 1);
			int position = dis(mt);
			pair<int,int> rand = available[position];
			int i = rand.first;
			int j = rand.second;
			set_edge_valueNS(i+1,j+1,2);
			available.erase(available.begin()+position);
			
		}
	}

	//Test to see if red has won. If not, then blue wins.
	if(hasWon(1)){
		WON = 1;
	} else{
		WON = 2;
	}
	//cout << "SEG1" <<endl;
}

//Create a deep copy of the board to avoid invalid frees
Graph* Graph::copy(){
	Graph* G = new Graph(num_vertices);
	for(int i = 0; i < num_vertices; i++){
		for(int j = 0; j < num_vertices; j++){
			G->matrix[i][j] = matrix[i][j];
		}
	}

	G->num_edges = num_edges;
	G->turn_count = turn_count;
	G->WON = WON;
	return G;
}


//Take in the current board and generate possible moves for it
//With all of those moves, randomdly place pieces until the end of the game, 
//and determine the winner.
Graph* Graph::MonteCarlo(int color){
	//Randomly generate all possible moves
	mt19937 initial_rand(time(0));

	//Initialize every board with a move
	vector<Graph*> poss_moves;
	for(int i = 1; i <=num_vertices; ++i){
		for(int j = 1; j <= num_vertices; ++j){
			if(get_edge_value(i,j) == 0){
				Graph* temp = copy();
				temp->set_edge_valueNS(i,j,color);
				poss_moves.push_back(temp);
			}
		}
	}
	
	//cout << "SEG" << endl;
	//Now for each of those new boards, randomize the game to the end and return the winner
	//might do this in a different method
	int best_move = -1;
	double largest_ratio = 0.0;
	for(int k = 0; k < poss_moves.size(); ++k){
		double temp = poss_moves[k]->MC_Ratio();
		if(temp > largest_ratio){
			//cout << k << ": " << temp << endl;
			largest_ratio = temp;
			best_move = k;
		}
	}
	//cout << best_move << endl;
	//poss_moves[best_move]->print();
	poss_moves[best_move]->WON == 0;
	return poss_moves[best_move];
}

//Take a given graph, and randomize it 1000 times, keeping track of red and blue wins
//Return the ratio of blue to red wins
double Graph::MC_Ratio(){
	int iterations = 1;
	double red_wins = 0.0;
	double blue_wins = 0.0;
	while(iterations < 1000){
		Graph* rando = copy();
		rando->randomize();		
		//rando->print();
		if(rando->WON == 1){
			red_wins++;
			//cout << red_wins << endl;
			//cout << "RED" << endl;
		} else {
			blue_wins++;
		}
		++iterations;
		delete rando;
	}

	double ratio = 1.0 * blue_wins/iterations;
	//cout << ratio << endl;
	return ratio;
}