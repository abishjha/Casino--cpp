#ifndef _BUILD_H
#define _BUILD_H

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
#include <map>
#include <sstream>
#include "logic.h"
using namespace std;

static const string HUMAN = "Human";
static const string COMPUTER = "Computer";


// this class is only instantionized once per game
// and can be used to check and do things related to build
class Build
{

public:
	Build();

	void create_build(string b_build, bool b_is_human, int b_sum = -1);

	void create_build(vector<string> cards, bool is_human, int sum = -1);

	void add_build_multi(string build, vector<string> other_cards, bool is_human);

	bool is_build(string build);

	bool is_multi_build(string build);

	vector<string> get_builds();

	string get_builds_str(string separator = " ");

	string get_builds_with_owner_str(string separator = " ");

	// goes through a string and return the start and end position of the first build
	bool get_first_build_start_end_pos(string build, size_t *start, size_t *end);

	// to be used for the loading serialization
	vector<string> parse_builds_str(string build);

	// to be used for the loading serialization
	void parse_builds_with_owner_str(string build_w_owner);

	bool delete_build(string build);

	// remove the build because somebody captured it and return all cards in the build
	vector<string> capture_build(string build);

	void change_ownership(string build);

	string get_owner(string build);

	vector<string> get_cards_in_build(string build);

	int get_build_sum(string build);

	// check if the player has any builds
	bool has_a_build(bool is_human);

	vector<string> get_player_builds(bool is_human);

	// will have to see if i need this function
	vector<int> get_player_build_sums(bool is_human);

	vector<int> get_all_build_sums();

	// used to extend a build
	string add_to_build(string build, string new_card);

	void print_stats();


private:
	vector<string> builds;

	// map == build --> owner
	map<string, string> owner;

	// map == build --> sum
	map<string, int> sum;

};


#endif
