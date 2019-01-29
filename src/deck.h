#ifndef _DECK_H
#define _DECK_H

/*
 *************************************************************
 * Name:     Abish Jha                                       *
 * Project:  Casino CPP                                      *
 * Class:    CMPS 366 Oraganization of Programming Languages *
 * Date:     October 24, 2018                                *
 *************************************************************
*/

#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<sstream>
#include<set>
#include<fstream>
using namespace std;

static const char SPADE = 'S';
static const char HEART = 'H';
static const char DIAMOND = 'D';
static const char CLUB = 'C';

class Deck {

public:
	Deck();

	// to load from file where deck is provided as a space separated string
	Deck(string deck_cards);

	bool check_for_duplicates();

	string get_one();

	bool is_empty();

	int size();

	vector<string> get_deck();

	// can also specify "\n" as separator to print each card on new line
	void print_deck(string separator = " ");


private:
	vector<string> cards;
	bool LOAD_FROM_FILE = false;
	string DECK_FILE_PATH = "./deck.txt";

};


#endif