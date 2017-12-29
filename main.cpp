/*  This is the main client program that allows the user to play hex against a really bad AI
	player.
*/
#include <iostream>
#include <string>
#include <sstream>
#include "Graph.hpp"
using namespace std;
int WINNER = 0;

int main(){
	cout << "Play Hex! Red (R) goes from north to south, Blue (B) goes from west to east.\n" << endl;
	cout << "Please enter the size of the board (ex: entering '6' will create a 6x6 board): " << endl;
	int game_size;
	cin >> game_size;
	cout << endl;
	Graph* board = new Graph(game_size);
	bool game_end = false;
	while(!game_end){
		if(board->turn_count % 2 == 0){
			board->print();
			cout << "It is your turn. Enter your move 'row# col#':" << endl;
			//Read the input from the user
			//string input;
			bool isPlaced = false;
			int row, col;
			cin >> row >> col;
			while(!isPlaced){
				if(row < 1 || row > game_size || col < 1 || col > game_size){
					cout << "Out of bounds entry, please enter a different move: " << endl;
					cin >> row >> col;
				} else if(board->get_edge_value(row, col) == 0){
					isPlaced = true;
				} else {
					cout << "That hex is already occupied, please enter a different move: " << endl;
					cin >> row >> col;
				}
			}
			board->set_edge_valueNS(row, col, 1);
			cout << endl;
			board->print();
			if(board->hasWon(1)){
				game_end = true;
				WINNER = 1;
				break;
			}
		} else {
			cout << endl;
			cout << "***** Opponent's move... *****" << endl;
			cout << endl;
			board = board->MonteCarlo(2); //AI move using monte carlo randomization
			if(board->hasWon(2)){
				game_end = true;
				WINNER = 2;
				break;
			}
		}
	}

	board->print();
	if(WINNER == 1){
		cout << "\n****** YOU WIN!! ******" << endl;
	} else {
		cout << "\n...... You lose :( ......" << endl;
	}

	delete board;
	return 0;
}