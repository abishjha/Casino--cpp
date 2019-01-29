#ifndef _LOGIC_H
#define _LOGIC_H

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
#include "build.h"
#include "move.h"
using namespace std;

class Build;

class Logic
{

public:
	Logic();

	Logic(Build* curr_build);

	~Logic();

	Move get_move(vector<string> hand, vector<string> table, bool is_human);

	bool can_make_build(string hand, vector<string> loose, vector<string> hand_cards);

	bool check_move(Move execute, vector<string> hand, vector<string> table, bool is_human = true);

	static int get_card_value(string card);


private:
	// a pointer to the current game build object
	Build* current_build;
	bool no_build;

};

#endif
