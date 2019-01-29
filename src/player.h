#ifndef _PLAYER_H
#define _PLAYER_H

/*
 *************************************************************
 * Name:     Abish Jha                                       *
 * Project:  Casino CPP                                      *
 * Date:     October 24, 2018                                *
 *************************************************************
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;


class Player
{

public:
	Player();

	Player(bool is_human_flag);

	Player(bool is_human_flag, int tournament_score, string card_hand_str, string card_captured_str);

	bool is_human();

	// take a card and add it to the player's hand
	void take_card(string card);

	// take a card and add it to the player's captured pile
	void capture_card(string card);

	// throw a card out of the hand and return it
	string throw_card();

	// remove a card from the hand
	bool remove_from_hand(string card);

	void add_tournament_score(int score);

	bool check_in_hand(string card);

	vector<string> get_captured_cards();

	vector<string> get_hand_cards();

	bool set_tournament_score(int score);

	int get_tournament_score();

	void reset_for_new_round();

	// can also specify "\n" as separator to print each card on new line
	void print_hand(string separator = " ", bool new_line = true);

	// can also specify "\n" as separator to print each card on new line
	void print_captured(string separator = " ", bool new_line = true);


private:
	vector<string> card_hand;
	vector<string> card_captured;
	int tournament_score;
	bool is_human_flag; // true for human

};

#endif
