#include "player.h"

/*
 *************************************************************
 * Name:     Abish Jha                                       *
 * Project:  Casino CPP                                      *
 * Class:    CMPS 366 Oraganization of Programming Languages *
 * Date:     October 24, 2018                                *
 *************************************************************
*/


/*************************************************************
Function Name       : Player
Purpose             : default constructor the player class
Parameters          : None
Return Value        : None
Assistance Received : None
**************************************************************/
Player::Player()
{
	this->is_human_flag = true;
	this->tournament_score = 0;
}


/*************************************************************
Function Name       : Player
Purpose             : constuctor for the player class
Parameters          : is_human_flag -- boolean parameter, true for human
Return Value        : None
Assistance Received : None
**************************************************************/
Player::Player(bool is_human_flag)
{
	this->is_human_flag = is_human_flag;
	this->tournament_score = 0;
}


/*************************************************************
Function Name       : Player
Purpose             : constructor for the player class
Parameters          : is_human_flag -- boolean parameter, true for human
					  tournament_score -- the current tournament score for the player
					  card_hand_str -- the hand cards of the player as a space separated string
					  card_captured_str -- the captured cards of the player as a space separated string
Return Value        : None
Assistance Received : None
**************************************************************/
Player::Player(bool is_human_flag, int tournament_score, string card_hand_str, string card_captured_str)
{
	this->is_human_flag = is_human_flag;
	this->tournament_score = tournament_score;

	string card;

	istringstream ss1(card_hand_str);
	while (ss1 >> card)
	{
		card_hand.push_back(card);
	}

	istringstream ss2(card_captured_str);
	while (ss2 >> card)
	{
		card_captured.push_back(card);
	}
}


/*************************************************************
Function Name       : is_human
Purpose             : check to see if player is human
Parameters          : None
Return Value        : true for human, false for computer
Assistance Received : None
**************************************************************/
bool Player::is_human()
{
	return is_human_flag;
}


/*************************************************************
Function Name       : take_card
Purpose             : take the passed in card and add it to the players hand
Parameters          : card -- a string containing the card to be taken
Return Value        : None
Assistance Received : None
**************************************************************/
// take a card and add it to the player's hand
void Player::take_card(string card)
{
	card_hand.push_back(card);
}


/*************************************************************
Function Name       : capture_card
Purpose             : take the passed in card and add it to the players pile
Parameters          : card -- a string containing the card to be captured
Return Value        : None
Assistance Received : None
**************************************************************/
// add a card to the player's captured cards
void Player::capture_card(string card)
{
	card_captured.push_back(card);
}


/*************************************************************
Function Name       : throw_card
Purpose             : throw a card from the hand. for example throwing it on the table
Parameters          : None
Return Value        : a string containing the card thrown from the hand
Assistance Received : None
**************************************************************/
// throw a card out of the hand and return it
string Player::throw_card()
{
	// make this more complicated
	string card = card_hand.back();
	card_hand.pop_back();
	return card;
}


/*************************************************************
Function Name       : remove_from_hand
Purpose             : remove the passed in card from hand
Parameters          : card -- card to remove from hand
Return Value        : true if successful, false otherwise
Assistance Received : None
**************************************************************/
// remove a card from the hand
bool Player::remove_from_hand(string card)
{
	vector<string>::iterator it = find(card_hand.begin(), card_hand.end(), card);

	// card not found
	if (it == card_hand.end())
	{
		return false;
	}

	// card found and removed
	card_hand.erase(it);
	return true;
}


/*************************************************************
Function Name       : add_tournament_score
Purpose             : add the passed in score to the tournament score of the player
Parameters          : score -- score to add
Return Value        : None
Assistance Received : None
**************************************************************/
void Player::add_tournament_score(int score)
{
	tournament_score += score;
}


/*************************************************************
Function Name       : check_in_hand
Purpose             : check if the card is present in the players hand
Parameters          : card -- card to check for
Return Value        : true if found, false otherwise
Assistance Received : None
**************************************************************/
bool Player::check_in_hand(string card)
{
	for (string c : card_hand)
	{
		if (c == card)
			return true;
	}
	return false;
}


/*************************************************************
Function Name       : get_captured_cards
Purpose             : get all the player's captured cards
Parameters          : None
Return Value        : vector of string with all the captured cards
Assistance Received : None
**************************************************************/
vector<string> Player::get_captured_cards()
{
	return card_captured;
}


/*************************************************************
Function Name       : get_hand_cards
Purpose             : get all the player's hand cards
Parameters          : None
Return Value        : vector of string with all the hand cards
Assistance Received : None
**************************************************************/
vector<string> Player::get_hand_cards()
{
	return card_hand;
}


/*************************************************************
Function Name       : set_tournament_score
Purpose             : set the tournament score for the player
Parameters          : score -- the score for the player
Return Value        : true is successful, false otherwise
Assistance Received : None
**************************************************************/
bool Player::set_tournament_score(int score)
{
	if (score < 0 || score > 99)
		return false;

	this->tournament_score = score;
	return true;
}


/*************************************************************
Function Name       : get_tournament_score
Purpose             : get the tournament score of rhe player
Parameters          : None
Return Value        : integer containing the score
Assistance Received : None
**************************************************************/
int Player::get_tournament_score()
{
	return tournament_score;
}


/*************************************************************
Function Name       : reset_for_new_round
Purpose             : reset the stats for when starting a new round
Parameters          : None
Return Value        : None
Assistance Received : None
**************************************************************/
void Player::reset_for_new_round()
{
	card_hand.clear();
	card_captured.clear();
}


/*************************************************************
Function Name       : print_hand
Purpose             : prints the user hand
Parameters          : separator -- what to separate the cards with
				      new_line -- if there should be a new line at the end
Return Value        : None
Assistance Received : None
**************************************************************/
// can also specify "\n" as separator to print each card on new line
void Player::print_hand(string separator, bool new_line)
{
	for (string card : card_hand)
	{
		cout << card << separator;
	}

	if (new_line)
		cout << endl;
}


/*************************************************************
Function Name       : print_captured
Purpose             : prints the user captured cards
Parameters          : separator -- what to separate the cards with
					  new_line -- if there should be a new line at the end
Return Value        : None
Assistance Received : None
**************************************************************/
// can also specify "\n" as separator to print each card on new line
void Player::print_captured(string separator, bool new_line)
{
	for (string card : card_captured)
	{
		cout << card << separator;
	}

	if (new_line)
		cout << endl;
}
