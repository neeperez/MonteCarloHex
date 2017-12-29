/* This is the Tree ADT that will be used to store the game tree. It is basically an adjacency list that
   keeps track the children of each of the internal nodes.
*/
#include "Tree.hpp"
#include "Graph.hpp"
using namespace std;

/*** Constructors ***/
Tree::Tree(Node* root, int size){
	//rootl.nodeID = total_nodes;
	this->root = root; 
	root->position->lP_traversal(1); root->position->lP_traversal(2); //Find the longest paths of the root node
	nodeLP.push_back(make_pair(root->position->lR_path.size(), root->position->lB_path.size())); //Store the sizes of the paths to a vector of pairs, first being Red, second Blue
	this->order = size;
	adj.resize(size); //The size will be the maximum number of turns for the given board size
	total_nodes++;
}
Tree::Tree(int size){
	this->order = size;
	adj.resize(size+1);

}

/*** Destructor ***/
Tree::~Tree(){
	//clear(root);
	//delete root;
}

// This function builds the tree of hex new boards, generating the moves of a node and then creating children with those boards
void Tree::buildTree(Node* node, int ply){
	if(ply == 4 || node->position->hasWon(1) || node->position->hasWon(2)){ //max depth that we wull go for now
		return;
	}
	Graph* curr_board = node->position->copy();
	//cout << "SEG2.0" << endl;
	vector<Graph*> moves = curr_board->genMoves();
	//cout << "SEG2.1" << endl;
	for(int i = 0; i < moves.size(); ++i){
		//cout << "SEG2.1.0" << endl;
		Node* curr_node = new Node(moves[i]);
		//cout << "SEG2.1.1" << endl;
		insertChild(node, curr_node);
		//cout << "SEG2.1.2" << endl;
		buildTree(curr_node, ply+1);
		//cout << "SEG2.1.3" << endl;		
	}
	//cout << "SEG2.2" << endl;
}

// Min-max evaluation using the alpha beta pruning algorithm provided by Wikipedia
int Tree::alphabeta(Node* node, int depth, int alpha, int beta, bool maxPlayer){
	//cout << "In depth: " << depth << endl;
	if(depth == 0 || adj[node->nodeID].size() == 0){
		int h = evaluate(node);
		node->heuristic = h; // Store the heuristic value so that we know which move to make at the termination of the alpha-beta function
		//cout << "Returning leaf value: " << h << endl;
		return h;
	}
	if(maxPlayer){
		int bestValue = -101;
		for(int i = 0; i < adj[node->nodeID].size(); ++i){
			Node* n = adj[node->nodeID].at(i);
			//cout << "This is node: " << n.nodeID << endl;
			bestValue = max(bestValue, alphabeta(n, depth -1, alpha, beta, false));
			//cout << "Back in depth: " << depth << endl;
			//cout << "bestValue is: " << bestValue << " v is: " << v << endl;
			alpha = max(alpha, bestValue);
			if(beta <= alpha)
				break;
			//cout << "Max bestValue is now: " << bestValue << endl;
		}
		//cout << "Returning: " << bestValue << endl;
		node->heuristic = bestValue;
		return bestValue;
	} else {
		int bestValue = 101;
		for(int i = 0; i < adj[node->nodeID].size(); ++i){
			Node* n = adj[node->nodeID].at(i);
			//cout << "This is node: " << n.nodeID << endl;
			bestValue = min(bestValue, alphabeta(n, depth -1, alpha, beta, true));
			//cout << "Back in depth: " << depth << endl;
			//cout << "bestValue is: " << bestValue << " v is: " << v << endl;
			beta = min(bestValue, bestValue);
			if(beta <= alpha)
				break;
			//cout << "Min bestValue is now: " << bestValue << endl;
		}
		//cout << "Returning: " << bestValue << endl;
		node->heuristic = bestValue;
		return bestValue;
	}
}

// Inserts a node into the adjacency list of the given parent node.
void Tree::insertChild(Node* p, Node* c){
	if(p->nodeID >= adj.size())
		adj.resize(p->nodeID*adj.size());
	//cout << "SEG2.1.1.0" << endl;
	c->parentID = p->nodeID;
	c->nodeID = total_nodes;
	c->depth = p->depth + 1;
	//cout << "SEG2.1.1.1" << endl;
	c->position->lP_traversal(1); c->position->lP_traversal(2);
	//cout << "SEG2.1.1.2" << endl;
	//cout << "The parent ID is: " << p->nodeID << endl << "The size of adj is: " << adj.size() << endl;
	adj[p->nodeID].push_back(c);
	//cout << "SEG2.1.1.3" << endl;
	nodeLP.push_back(make_pair(c->position->lR_path.size(), c->position->lB_path.size()));
	//cout << "SEG2.1.1.4" << endl;
	total_nodes++;

	/*if(total_nodes == order)
		adj.resize(total_nodes*order);*/

}


// Very, VERY simplistic evaluation of the move
int Tree::evaluate(Node* node){
	/*if(node->nodeID == 3)
		return 5;
	if(node->nodeID == 4 || node->nodeID == 5)
		return 8;
	if(node->nodeID == 6)
		return 3;
	if(node->nodeID == 7)
		return 9;*/
	if(node->position->hasWon(2)){
		return 100;
	}
	if(node->position->hasWon(1)){
		return -100;
	}

	if(node->position->turn_count % 2 == 1){ //Red (Player)

		//Find the largest path among all of the children
		vector<Node*> siblings = adj[node->parentID];
		int largest_Red = nodeLP[siblings[0]->nodeID].first;
		int largest_ID = siblings[0]->nodeID;
		for(int i = 1; i < siblings.size(); ++i){
			if(largest_Red < nodeLP[siblings[i]->nodeID].first){
				largest_Red = nodeLP[siblings[i]->nodeID].first;
				largest_ID = siblings[i]->nodeID;
			}
			
		}

		//Check to see if this node has the largest path
		if(largest_ID == node->nodeID){
			return -90;
		}
		//now see if the player extended their path in comparison to the previous board
		if(nodeLP[node->parentID].first < nodeLP[node->nodeID].first){ //They've extended their largest path
			return -80;
		}
		if(nodeLP[node->parentID].first == nodeLP[node->nodeID].first){ //they've blocked their opponents largest path
			return -87;
		} else {
			return -50; //Other move made
		}
	} else {
		vector<Node*> siblings = adj[node->parentID];
		int largest_Blue = nodeLP[siblings[0]->nodeID].second;
		int largest_ID = siblings[0]->nodeID;
		
		for(int i = 1; i < siblings.size(); ++i){
			//curr_pos = siblings[i]->position;
			if(largest_Blue < nodeLP[siblings[i]->nodeID].second){
				largest_Blue = nodeLP[siblings[i]->nodeID].second;
				largest_ID = siblings[i]->nodeID;
			}
			
		}
		

		if(largest_ID == node->nodeID){
			return 90;
		}
		//now see if we extended our path in comparison to the previous board
		if(nodeLP[node->parentID].second < nodeLP[node->nodeID].second){ //We've extended our largest path
			return 80;
		}
		if(nodeLP[node->parentID].second == nodeLP[node->nodeID].second){ //we've blocked our opponents largest path
			return 97;
		} else {
			return 50; //Other move made
		}
	}
}

//Clear the tree, deleting each node recursively
void Tree::clear(Node* node){
	if(adj[node->nodeID].size() == 0){
		delete node;
		return;
	}
	for(int i = 0; i < adj[node->nodeID].size(); ++i){
		Node* n = adj[node->nodeID].at(i);
		clear(n);
		//delete n;
	}
	//delete node;
}