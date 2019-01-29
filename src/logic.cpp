#include "logic.h"

/*
 *************************************************************
 * Name:     Abish Jha                                       *
 * Project:  Casino CPP                                      *
 * Date:     October 24, 2018                                *
 *************************************************************
*/


/*************************************************************
Function Name       : Logic
Purpose             : default constructor
Parameters          : None
Return Value        : None
Assistance Received : None
**************************************************************/
Logic::Logic() {
	current_build = new Build();
	no_build = true;
}


/*************************************************************
Function Name       : Logic
Purpose             : constructor 
Parameters          : pointer to a build object that holds the builds of the current game
Return Value        : None
Assistance Received : None
**************************************************************/
Logic::Logic(Build* curr_build) {
	this->current_build = curr_build;
	no_build = false;
}


/*************************************************************
Function Name       : ~Logic
Purpose             : destructor for the class
Parameters          : None
Return Value        : None
Assistance Received : None
**************************************************************/
Logic::~Logic() {
	if (no_build)
		delete current_build;
}


/*************************************************************
Function Name       : split
Purpose             : split a string based on the specified delimiter
Parameters          : s --  a reference to the string
                      delim -- a character to split the string on
Return Value        : a vector containing the sub string after the split
Assistance Received : stackoverflow
**************************************************************/
vector<string> split(const string &s, char delim)
{
	vector<string> elems;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
		elems.push_back(item);

	return elems;
}


/*************************************************************
Function Name       : get_all_subsets
Purpose             : find all possible susbsets of the given set
Parameters          : arr -- a vector containing elements whose subsets are to be returned
Return Value        : a vector of vector of strings where each element contains a susbet of the superset
Assistance Received : stackoverflow
**************************************************************/
// Function to find all subsets of given set. Any repeated 
// subset is considered only once in the output 
vector<vector<string>> get_all_subsets(vector<string> arr)
{
	vector<string> list;
	int n = arr.size();

	/* Run counter i from 000..0 to 111..1*/
	for (int i = 0; i < (int)pow(2, n); i++)
	{
		string subset = "";

		// consider each element in the set 
		for (int j = 0; j < n; j++)
		{
			// Check if jth bit in the i is set. If the bit 
			// is set, we consider jth element from set 
			if ((i & (1 << j)) != 0)
				subset += arr[j] + "|";
		}

		// if subset is encountered for the first time 
		// If we use set<string>, we can directly insert 
		if (find(list.begin(), list.end(), subset) == list.end())
			list.push_back(subset);
	}

	vector<vector<string>> ret_list;

	// consider every subset 
	for (string subset : list)
		// split the subset and print its elements 
		ret_list.push_back(split(subset, '|'));

	return ret_list;
}


/*************************************************************
Function Name       : get_move
Purpose             : get a move based on the current card conditions
Parameters          : hand -- the cards in the hand of the player
					  table -- the cards on the table
					  is_human -- true is player is human, false otherwise
Return Value        : a Move object containing the action to be performed, the loose cards involved and the hand card involved
Assistance Received : None
**************************************************************/
Move Logic::get_move(vector<string> hand, vector<string> table, bool is_human) {
	// try build... extend build, or multi build, or create new build
	{
		int max_card_count = 0;
		vector<string> max_capture_cards;  string hand_card;

		// check if opponent has builds you can extend
		if (current_build->has_a_build(!is_human))
		{
			vector<string> opponent_builds = current_build->get_player_builds(!is_human);

			// see if it is possible to extend the build
			for (string build : opponent_builds)
			{
				for (string s : hand)
				{
					if (can_make_build(s, current_build->get_cards_in_build(build), hand))
					{
						if (max_card_count < current_build->get_cards_in_build(build).size())
						{
							hand_card = s;
							max_card_count = current_build->get_cards_in_build(build).size();
							max_capture_cards.push_back(build);
						}
					}
				}
			}

			Move curr_move = Move(ACTION_EXTEND, hand_card, max_capture_cards);
			if (max_card_count > 0 && check_move(curr_move, hand, table, is_human))
				return curr_move;
		}

		// check if there are any build you can make a multi build out of
		// only make your own builds into multiple builds as making the oppenents build a multi build with only give them more cards to capture
		if (current_build->has_a_build(is_human))
		{
			vector<string> builds = current_build->get_player_builds(is_human);
			Move curr_move;

			for (string build : builds)
			{
				int build_sum = current_build->get_build_sum(build);

				for (string h : hand)
				{
					if (build_sum < Logic::get_card_value(h))  continue;
					else if (build_sum == Logic::get_card_value(h))
					{
						curr_move = Move(ACTION_MULTI, h, vector<string>{build});
						if (check_move(curr_move, hand, table, is_human))
							return curr_move;
					}

					vector<string> loose_cards = table;

					// find subset where the total val is build_sum - hand card value
					vector<vector<string>> subsets = get_all_subsets(loose_cards);
					for (vector<string> subset : subsets)
					{
						int loose_cards_sum = 0;
						for (string s : subset)
							loose_cards_sum += Logic::get_card_value(s);

						if (loose_cards_sum == (build_sum - Logic::get_card_value(h)))
						{
							subset.push_back(build);
							curr_move = Move(ACTION_MULTI, h, subset);
							if (check_move(curr_move, hand, table, is_human))
								return curr_move;
						}
					}
				}
			}
		}

		// check if you can create a build
		max_card_count = -1;

		vector<vector<string>> subsets = get_all_subsets(table);
		for (string h : hand)
		{
			for (vector<string> loose_card_set : subsets)
			{
				int loose_cards_size = loose_card_set.size();

				if (can_make_build(h, loose_card_set, hand))
				{
					if (loose_cards_size > max_card_count)
					{
						max_capture_cards = loose_card_set;
						hand_card = h;
						max_card_count = loose_card_set.size();
					}
				}
			}
		}

		Move curr_move = Move(ACTION_BUILD, hand_card, max_capture_cards);
		if (max_card_count != -1 && check_move(curr_move, hand, table, is_human))
			return curr_move;
	}

	// try capture
	{
		int max_card_count = 0;
		vector<string> max_capture_cards;  string hand_card;

		// check to see which card lets you capture the most number of cards
		for (string play_card : hand)
		{
			int play_card_val = Logic::get_card_value(play_card);
			vector<string> capture_cards, other_loose_cards;
			int card_count = 0;

			for (string t : table)
			{
				if (play_card_val == Logic::get_card_value(t))
				{
					capture_cards.push_back(t);
					card_count++;
				}
				else
					other_loose_cards.push_back(t);
			}

			for (string t : current_build->get_builds())
			{
				if (play_card_val == current_build->get_build_sum(t))
				{
					capture_cards.push_back(t);
					card_count += current_build->get_cards_in_build(t).size();
				}
			}

			while (true)
			{
				vector<vector<string>> subsets = get_all_subsets(other_loose_cards);

				if (subsets.empty() || subsets.size() == 0)
					break;

				bool any_subset = false;
				for (vector<string> subset : subsets)
				{
					int tot_subset_sum = 0;

					for (string item : subset)
						tot_subset_sum += Logic::get_card_value(item);

					if (tot_subset_sum == play_card_val)
					{
						any_subset = true;

						for (string item : subset)
						{
							capture_cards.push_back(item);
							other_loose_cards.erase(find(other_loose_cards.begin(), other_loose_cards.end(), item));

							card_count++;
						}
						// break out to the outer loop to recalculate subsets
						break;
					}
				}

				if (!any_subset)  break;
			}

			if (card_count > max_card_count)
			{
				max_card_count = card_count;
				max_capture_cards = capture_cards;
				hand_card = play_card;
			}
		}

		if (max_card_count > 0)
			return Move(ACTION_CAPTURE, hand_card, max_capture_cards);
	}


	// then try trail with least significant card
	{
		string card_to_trail = "";

		for (string h : hand)
		{
			if (h != "DX" && h != "S2")
			{
				card_to_trail = h;
				break;
			}
		}

		if (card_to_trail == "")
			card_to_trail = hand.front();

		Move curr_move = Move(ACTION_TRAIL, card_to_trail, vector<string>());
		if (check_move(curr_move, hand, table, is_human))
			return curr_move;
	}

	// return an empty move object if none of the above find a solution
	// not finding a move is not possible as the user can always trail, however this statement is here so all paths return a value
	return Move("", "", vector<string>());
}


/*************************************************************
Function Name       : can_make_build
Purpose             : check if the given set of cards can make a build and if it is legit i.e. the player has a card equal to the sum of the build in hand
Parameters          : hand       -- the card that is taken from the hand
					  loose      -- vector of loose cards that are to be used to make the build
					  hand_cards -- the other cards in the players hand
Return Value        : true if can make build, false otherwise
Assistance Received : None
**************************************************************/
// check if the given set of cards can make a build
//   
bool Logic::can_make_build(string hand, vector<string> loose, vector<string> hand_cards) {
	int hand_card_val = Logic::get_card_value(hand);
	int loose_card_val = 0;

	for (string c : loose)
		loose_card_val += Logic::get_card_value(c);

	// sum of build
	int sum = hand_card_val + loose_card_val;

	for (string c : hand_cards)
	{
		// dont look at the card that is being played
		if (c == hand)
			continue;

		// check if the user has the card that is equal to the sum of the build in his hand
		if (Logic::get_card_value(c) == sum)
			return true;
	}

	return false;
}


/*************************************************************
Function Name       : check_move
Purpose             : check if the move is valid based on the game rules
Parameters          : execute -- the move to be checked
					  hand -- the cards in the players hand
					  table -- the cards on the table
					  is_human -- if the player is human
Return Value        : true if move is valid, false otherwise
Assistance Received : None
**************************************************************/
bool Logic::check_move(Move execute, vector<string> hand, vector<string> table, bool is_human) {
	/** also get table build cards from build object **/

	if (execute.get_action() == ACTION_BUILD)
	{
		if (execute.get_loose_cards().empty() || execute.get_hand_card().empty())
			return false;

		return can_make_build(execute.get_hand_card(), execute.get_loose_cards(), hand);
	}
	else if (execute.get_action() == ACTION_EXTEND)
	{
		if (execute.get_loose_cards().empty() || execute.get_hand_card().empty())
			return false;

		string ex_build = execute.get_loose_cards().front();

		// cannot extend multi builds
		if (!current_build->is_build(ex_build) || current_build->is_multi_build(ex_build))
			return false;

		// player cannot extend his own build
		if (is_human && current_build->get_owner(ex_build) == HUMAN)
			return false;
		else if (!is_human && current_build->get_owner(ex_build) == COMPUTER)
			return false;

		return can_make_build(execute.get_hand_card(), current_build->get_cards_in_build(ex_build), hand);
	}
	else if (execute.get_action() == ACTION_MULTI)
	{
		if (execute.get_hand_card().empty())
			return false;

		bool one_build = false;
		string ex_build;

		// find the first build in the input and store it in the ex_build variable
		// then check if the input has other builds, if yes, move is not valid as there is ambiguity
		for (string s : execute.get_loose_cards())
		{
			if (current_build->is_build(s) && !one_build)
			{
				ex_build = s;
				one_build = true;
			}
			else if (current_build->is_build(s) && one_build)
				return false;
		}

		// if there is no build in the input, exit
		if (!one_build) return false;

		int build_sum = current_build->get_build_sum(ex_build);

		int loose_sum = 0;
		vector<string> tot_cards = execute.get_all_cards();
		for (string c : tot_cards)
		{
			// skip the build object
			if (current_build->is_build(c))
				continue;

			loose_sum += Logic::get_card_value(c);
		}

		// check if the loose cards sum adds up to the build sum so it can be made into a multi build, and,
		// check if the user has the card that is equal to the sum of the build in his hand
		if (loose_sum == build_sum)
		{
			for (string s : hand)
			{
				// dont look at the card that is being played
				if (s == execute.get_hand_card())
					continue;

				// check if the user has the card that is equal to the sum of the build in his hand
				if (Logic::get_card_value(s) == build_sum)
					return true;
			}
		}
	}
	else if (execute.get_action() == ACTION_CAPTURE)
	{
		if (execute.get_loose_cards().empty() || execute.get_hand_card().empty())
			return false;

		int card_val = Logic::get_card_value(execute.get_hand_card());
		vector<string> selected_capture_cards = execute.get_loose_cards();

		// check if all cards that have the same value as the hand card are captured
		for (string s : table)
		{
			// if table card has different value, continue to next
			if (card_val != Logic::get_card_value(s))
				continue;

			// every loose card that matches the card value must be captured
			if (find(selected_capture_cards.begin(), selected_capture_cards.end(), s) == selected_capture_cards.end())
				return false;

		}

		// this vector will store all the values for the loose cards that have been selected to capture
		vector<int> loose_vals;

		// check if the sums match up to the capture card value
		for (string s : execute.get_loose_cards())
		{
			// if is a build but the build sum is different than capture card, return
			if (current_build->is_build(s))
			{
				if (current_build->get_build_sum(s) != card_val)
					return false;
				continue;
			}

			loose_vals.push_back(Logic::get_card_value(s));
		}

		// loop to check if the given loose cards add up to the hand card value
		// note: the sets of loose cards that add up to the hand card value have to be in order
		for (int i = 0; i < loose_vals.size(); i++)
		{
			int temp = loose_vals[i];

			if (temp > card_val)
				return false;
			else if (temp == card_val)
				continue;

			int t_sum = 0, j = i;
			while (true)
			{
				t_sum += loose_vals[j];

				if (t_sum == card_val)
				{
					i = j;
					break;
				}
				else if (t_sum > card_val)
					return false;

				j++;
				if (j == loose_vals.size())
					return false;
			}
		}

		return true;
	}
	else if (execute.get_action() == ACTION_TRAIL)
	{
		// card not specified
		if (execute.get_hand_card().empty())
			return false;

		// dont let players who have a build trail
		if (current_build->has_a_build(is_human))
			return false;

		int card_val = Logic::get_card_value(execute.get_hand_card());

		// check if card matches any loose cards.  if yes, dont let them trail
		for (string c : table)
		{
			if (card_val == Logic::get_card_value(c))
				return false;
		}

		return true;
	}
	return false;
}


/*************************************************************
Function Name       : get_card_value
Purpose             : function returns the numeric value of the passed in card 
Parameters          : card -- the card to get the value of 
Return Value        : integer representing the value of the card
Assistance Received : None
**************************************************************/
int Logic::get_card_value(string card) {
	// make card represent only the value of the card
	if (card.length() == 2)
		card = card[1];
	else if (card.length() != 1)
		return -1;  // invalid card    

	switch (card[0]) {
	case 'A': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'X': return 10;
	case 'J': return 11;
	case 'Q': return 12;
	case 'K': return 13;
	default: return -1;  // invalid card
	}
}
