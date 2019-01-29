#include "deck.h"

/*
 *************************************************************
 * Name:     Abish Jha                                       *
 * Project:  Casino CPP                                      *
 * Date:     October 24, 2018                                *
 *************************************************************
*/

Deck::Deck() {
	if (LOAD_FROM_FILE)
	{
		ifstream deck_file;
		deck_file.open(DECK_FILE_PATH);

		if (!deck_file.good())
		{
			cout << "the deck file does not exist\n";
			return;
		}

		string line, card;
		while (getline(deck_file, line))
		{
			istringstream ss(line);
			ss >> card;
			cards.push_back(card);
		}

		deck_file.close();
	}
	else
	{
		string suite[] = { string(1, SPADE), string(1, HEART), string(1, DIAMOND), string(1, CLUB) };
		string card[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "X", "J", "Q", "K" };

		// insert 52 cards into the deck vector
		for (string s : suite) {
			for (string c : card) {
				cards.push_back(s + c);
			}
		}

		// shuffle the deck
		random_shuffle(cards.begin(), cards.end());
	}

	if (check_for_duplicates())
	{
		cout << "there are duplicate cards in the deck\n";
		cards.clear();
		Deck(); // recall the constructor to give it another try
	}
}

// to load from file where deck is provided as a space separated string
Deck::Deck(string deck_cards) {
	istringstream ss(deck_cards);
	string card;

	while (ss >> card) {
		cards.push_back(card);
	}

	if (check_for_duplicates())
	{
		cout << "there are duplicate cards in the deck\n";
		cards.clear();
	}
}


// true -- if duplicates
// false -- if not duplicates
bool Deck::check_for_duplicates()
{
	// store all the cards in a deck and if the size of the vector and the set is not the same, we have duplicates in the vector
	set<string> check_set(cards.begin(), cards.end());
	return check_set.size() != cards.size();
}


string Deck::get_one() {
	// remove and return the first card from the deck
	string card = cards.front();
	cards.erase(cards.begin());
	return card;
}

bool Deck::is_empty() {
	return cards.empty();
}

int Deck::size() {
	return cards.size();
}

vector<string> Deck::get_deck() {
	return cards;
}

// can also specify "\n" as separator to print each card on new line
void Deck::print_deck(string separator) {
	for (string card : cards) {
		cout << card << separator;
	}
	cout << endl;
}
