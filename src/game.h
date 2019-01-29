#ifndef _GAME_H
#define _GAME_H

/*
 *************************************************************
 * Name:     Abish Jha                                       *
 * Project:  Casino CPP                                      *
 * Class:    CMPS 366 Oraganization of Programming Languages *
 * Date:     October 24, 2018                                *
 *************************************************************
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <random>
#include "player.h"
#include "deck.h"
#include "logic.h"
#include "build.h"
#include "move.h"
using namespace std;


const string SAVE_FILE_NAME = "./save.txt";

class Game
{

public:
	/** use this game by creating a round object,
		then calling the play function       **/

	Game();

	void coin_toss();

	// the input will be from the serialization file and
	// will contain builds and loose cards
	void populate_table_cards(string cards);

	bool load_game(string file_name);

	void play();

	bool move_handler();

	bool get_int(string num_str, int* num);

	vector<string> choice_prompt(vector<string> choices);

	Move move_menu();

	bool make_move(Move execute);

	// routine to print out stats at the end of the tournament
	void tournament_end_stats();

	// prints the menu and gets the choice
	int get_menu_options();

	// change the current_player pointer
	void change_player();

	void add_to_table(string card);

	void deal_to_human_and_computer();

	string get_table_cards_str(string separator = " ");

	string get_loose_cards_str(string separator = " ");

	// do all automatic stuff here i.e. setting the game, menu is provided in parent
	bool setup_new_round();

	void print_stats();

	// process the total score from the captured cards and add to the respective tournament totals for both players
	void process_round_score();

	string vector_to_string(vector<string> cards_vector, string separator = " ");

	bool save_game();

	void remove_from_table(string card);


private:
	vector<string> loose_cards; // the cards on the table
	char last_capture;          // 'c' -- computer or 'h' -- human
	Deck game_deck;             // the active game's deck
	Player human;               // the human player object
	Player computer;            // the computer player object
	Player *current_player;     // a pointer to the current player
	int round_number;
	Logic game_logic;
	Build current_builds;

};

#endif
