#ifndef _MOVE_H
#define _MOVE_H

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
using namespace std;

static const string ACTION_BUILD = "build";
static const string ACTION_CAPTURE = "capture";
static const string ACTION_TRAIL = "trail";
static const string ACTION_EXTEND = "extend_build";
static const string ACTION_MULTI = "multi_build";

class Move
{

public:
	Move();

	Move(string action, string hand_card, vector<string> loose_cards);

	void set_action(string action);

	string get_action();

	void set_hand_card(string hand_card);

	string get_hand_card();

	void set_loose_cards(vector<string> loose_cards);

	vector<string> get_loose_cards();

	string get_loose_cards_str();

	vector<string> get_all_cards();

	void pretty_print();


private:
	string action;
	string hand_card;
	vector<string> loose_cards;

};

#endif
