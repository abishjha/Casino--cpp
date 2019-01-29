#include "move.h"

/*
 *************************************************************
 * Name:     Abish Jha                                       *
 * Project:  Casino CPP                                      *
 * Date:     October 24, 2018                                *
 *************************************************************
*/

/*************************************************************
Function Name       : Move
Purpose             : default constructor
Parameters          : None
Return Value        : None
Assistance Received : None
**************************************************************/
Move::Move()
{
	action = "";
	hand_card = "";
	loose_cards.clear();
}


/*************************************************************
Function Name       : Move
Purpose             : constructor
Parameters          : action -- the action to take.  one of five options (capture, trail, build, multi build, extend build)
Return Value        : None
Assistance Received : None
**************************************************************/
Move::Move(string action, string hand_card, vector<string> loose_cards)
{
	set_action(action);
	set_hand_card(hand_card);
	set_loose_cards(loose_cards);
}


/*************************************************************
Function Name       : set_action
Purpose             : setter for the action
Parameters          : action -- the action performed in the move
Return Value        : None
Assistance Received : None
**************************************************************/
void Move::set_action(string action)
{
	this->action = action;
}


/*************************************************************
Function Name       : get_action
Purpose             : getter for the action
Parameters          : None
Return Value        : action -- the action performed in the move
Assistance Received : None
**************************************************************/
string Move::get_action()
{
	return action;
}


/*************************************************************
Function Name       : set_hand_card
Purpose             : setter for the hand card
Parameters          : hand_card -- the hand card
Return Value        : None
Assistance Received : None
**************************************************************/
void Move::set_hand_card(string hand_card)
{
	this->hand_card = hand_card;
}


/*************************************************************
Function Name       : get_hand_card
Purpose             : getter for the hand card
Parameters          : None
Return Value        : hand_card -- the hand card
Assistance Received : None
**************************************************************/
string Move::get_hand_card()
{
	return hand_card;
}


/*************************************************************
Function Name       : set_loose_cards
Purpose             : setter for the loose cards
Parameters          : loose_cards -- the loose cards in a vector of string
Return Value        : None
Assistance Received : None
**************************************************************/
void Move::set_loose_cards(vector<string> loose_cards)
{
	this->loose_cards = loose_cards;
}


/*************************************************************
Function Name       : get_loose_cards
Purpose             : getter for the loose cards
Parameters          : None
Return Value        : loose_cards -- the loose cards in a vector of string
Assistance Received : None
**************************************************************/
vector<string> Move::get_loose_cards()
{
	return loose_cards;
}


/*************************************************************
Function Name       : get_loose_cards_str
Purpose             : get the loose cards as a string
Parameters          : None
Return Value        : the loose cards as a string
Assistance Received : None
**************************************************************/
string Move::get_loose_cards_str()
{
	string ret = "";
	for (string s : get_loose_cards())
		ret += s + " ";

	return ret;
}


/*************************************************************
Function Name       : get_all_cards
Purpose             : get the hand card and the loose cards as a string
Parameters          : None
Return Value        : the hand card and the loose cards as a string
Assistance Received : None
**************************************************************/
vector<string> Move::get_all_cards()
{
	vector<string> ret;

	ret.push_back(hand_card);
	ret.insert(ret.end(), loose_cards.begin(), loose_cards.end());

	return ret;
}


/*************************************************************
Function Name       : pretty_print
Purpose             : pretty print the move and the reason for it
Parameters          : None
Return Value        : None
Assistance Received : None
**************************************************************/
void Move::pretty_print()
{
	string reason = "";

	if (get_action() == ACTION_EXTEND)
		reason = "as a defensive move to potentially stop the other user from capturing his/her build";
	else if (get_action() == ACTION_MULTI)
		reason = "extend one of our own builds to make a multi build so we can maximize capture card number in the next turn";
	else if (get_action() == ACTION_BUILD)
		reason = "so we can maximize capture card number in the next turn";
	else if (get_action() == ACTION_CAPTURE)
		reason = "no suitable build related option so we capture the maximum amount of card, set, and/or build possible";
	else if (get_action() == ACTION_TRAIL)
		reason = "no better move available";

	cout << " +--------------------------------------------------+\n"
		<< " | action      : " << get_action() << "\n"
		<< " | hand card   : " << get_hand_card() << "\n"
		<< " | loose cards : " << get_loose_cards_str() << "\n"
		<< " | reason      : " << reason << "\n"
		<< " +--------------------------------------------------+\n";
}
