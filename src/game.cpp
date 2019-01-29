#include "game.h"

/*
 *************************************************************
 * Name:     Abish Jha                                       *
 * Project:  Casino CPP                                      *
 * Date:     October 24, 2018                                *
 *************************************************************
*/


/*************************************************************
Function Name       : Game
Purpose             : constructor 
Parameters          : None
Return Value        : None
Assistance Received : None
**************************************************************/
Game::Game()
{
	cout << "--- WELCOME TO CASINO!!! ---\n\n";
	cout << "would you like to resume a saved game? (y/n) ";
	char choice;
	cin >> choice;

	// load a saved game
	if (choice == 'Y' || choice == 'y')
	{
		string file_location = "";

		while (!load_game(file_location))
		{
			cout << " enter file name with path: ";
			cin >> file_location;
		}
		cout << " game loaded from " << file_location << "\n";

		game_logic = Logic::Logic(&current_builds);
		return;
	}

	// start a new tournament
	this->human = Player(true);
	this->computer = Player(false);
	round_number = 1;

	// a coin toss to choose the player who goes first
	coin_toss();

	// sets up the board for new round
	setup_new_round();
}


/*************************************************************
Function Name       : coin_toss
Purpose             : coin toss at the beginning of a new game
Parameters          : None
Return Value        : None
Assistance Received : None
**************************************************************/
void Game::coin_toss()
{
	srand(time(NULL));
	int toss = rand() % 2;
	// cout << "toss (h is 1 and t is 0) : " << toss << endl;

	char choice;
	cout << "\n--- coin toss ---\n"
		<< "select h (head) or t (tail): ";
	cin >> choice;

	// h (head) is 1  and  t (tail) is 0
	if (((choice == 'h' || choice == 'H') && toss == 1) || ((choice == 't' || choice == 'T') && toss == 0))
	{
		cout << " you win the toss, you start first\n";
		current_player = &human;
	}
	else
	{
		cout << " you lose the toss, computer starts first\n";
		current_player = &computer;
	}
}


/*************************************************************
Function Name       : populate_table_cards
Purpose             : fill in table cards and build cards depending on the string input. usually from serialization file
Parameters          : cards -- a space separated string containing the cards 
Return Value        : None
Assistance Received : None
**************************************************************/
// the input will be from the serialization file and
// will contain builds and loose cards
void Game::populate_table_cards(string cards)
{
	// the string is parsed and loose cards are returned in a vector
	// builds are populated into the current game's build object
	vector<string> loose_cards_ret = current_builds.parse_builds_str(cards);

	for (string card : loose_cards_ret)
		add_to_table(card);
}


/*************************************************************
Function Name       : load_game
Purpose             : load game from a file and initialize objects based on that
Parameters          : file_name -- file to load contents from
Return Value        : true if successful, false otherwise
Assistance Received : None
**************************************************************/
bool Game::load_game(string file_name)
{
	ifstream load_file;
	try
	{
		load_file.open(file_name);

		if (!load_file.good())
			throw "bad file error";
	}
	catch (...)
	{
		return false;
	}

	string line;
	while (getline(load_file, line))
	{
		istringstream ss(line);

		string word;
		ss >> word;

		if (word == "Round:")
		{
			ss >> word;
			round_number = stoi(word);
		}
		else if (word == "Computer:" || word == "Human:")
		{
			string line1, word1;
			string hand, pile;
			int score;

			for (int i = 0; i < 3; i++)
			{
				getline(load_file, line1);

				istringstream ss1(line1);
				ss1 >> word1;

				if (word1 == "Score:")
				{
					ss1 >> word1;
					score = stoi(word1);
				}
				else if (word1 == "Hand:")
				{
					hand = line1.substr(line1.find_first_of(":") + 1);
				}
				else if (word1 == "Pile:")
				{
					pile = line1.substr(line1.find_first_of(":") + 1);
				}
			}

			// setting the objects for human and computer player
			if (word == "Human:")
				this->human = Player(true, score, hand, pile);
			else if (word == "Computer:")
				this->computer = Player(false, score, hand, pile);
		}
		else if (word == "Table:")
		{
			populate_table_cards(line.substr(line.find_first_of(":") + 2));
		}
		else if (word == "Build")
		{
			ss >> word;
			if (word == "Owner:")
			{
				current_builds.parse_builds_with_owner_str(line.substr(line.find_first_of(":") + 2));
			}
			else
			{
				load_file.close();
				return false;
			}
		}
		else if (word == "Deck:")
		{
			this->game_deck = Deck(line.substr(line.find_first_of(":") + 1));
		}
		else if (word == "Next")
		{
			ss >> word;
			if (word == "Player:")
			{
				ss >> word;
				if (word == "Human")
					current_player = &human;
				else
					current_player = &computer;
			}
			else
			{
				load_file.close();
				return false;
			}
		}
		else if (word == "Last")
		{
			ss >> word;
			if (word == "Capturer:")
			{
				ss >> word;
				if (word == "Human")
					last_capture = 'h';
				else
					last_capture = 'c';
			}
			else
			{
				load_file.close();
				return false;
			}
		}
	}

	load_file.close();
	return true;
}


/*************************************************************
Function Name       : play
Purpose             : main entry point for the game execution.  prints the main menu and lets the user choose.  calls the appropriate function based on the choice and checks if the round and/or tournament has ended after every move
Parameters          : None
Return Value        : None
Assistance Received : None
**************************************************************/
void Game::play()
{
	Move help_move;

	// loop through menu options until the cards in the deck run dry and the round ends
	while (true)
	{
		cout << "\n--- Game Status ---\n"
			<< "Human Hand    : " << vector_to_string(human.get_hand_cards()) << "\n"
			<< "Computer Hand : " << vector_to_string(computer.get_hand_cards()) << "\n"
			<< "Table Cards   : " << get_table_cards_str() << "\n"
			<< "Current Turn  : " << ((current_player->is_human()) ? "Human" : "Computer") << "\n"
			<< "-------------------\n";

		// gets the user choice
		int choice = get_menu_options();  char choice_char;
		switch (choice)
		{
			// save the game
		case 1:
			if (save_game())
			{
				cout << " game saved to " << SAVE_FILE_NAME << "\n"
					<< "exiting game...\n";
				return;
			}
			else
			{
				cout << " ERROR saving game\n";
				continue;
			}

			// make a move
		case 2:
			if (move_handler())
				change_player();
			else
				cout << " error. cannot complete move\n";
			break;

			// get help, only for human
		case 3:
			help_move = game_logic.get_move(human.get_hand_cards(), loose_cards, current_player->is_human());
			help_move.pretty_print();

			cout << " do you want to execute this move? (y/n) ";
			cin >> choice_char;
			if (choice_char == 'Y' || choice_char == 'y')
			{
				if (make_move(help_move))
				{
					change_player();
					break;
				}
			}

			continue;

			// exit game
		case 4:
			cout << "exiting game...\n";
			return;
		}

		/** check to see if game has ended, if yes play another round or end tournament **/
		if (human.get_hand_cards().empty() && computer.get_hand_cards().empty() && game_deck.size() <= 0)
		{
			cout << "\n-----------------\n--- Round End ---\n-----------------\n";

			// player who made last capture gets all remaining table cards
			// at this point, builds can not and should not exist
			if (last_capture == 'c' || last_capture == 'C')
			{
				for (string card : loose_cards)
					computer.capture_card(card);

				// starting player for next round is the one who made the last capture in previous round
				current_player = &computer;
			}
			else
			{
				for (string card : loose_cards)
					human.capture_card(card);

				// starting player for next round is the one who made the last capture in previous round
				current_player = &human;
			}
			loose_cards.clear();

			// print both players card piles at end of each round
			cout << "Human Captured Pile   : " << vector_to_string(human.get_captured_cards()) << "\n"
				<< "Computer Captured Pile: " << vector_to_string(computer.get_captured_cards()) << "\n"
				<< "-----------------\n";

			// process and print round and tournament score
			process_round_score();

			cout << "-----------------\n";
			round_number++;

			if (human.get_tournament_score() >= 21 || computer.get_tournament_score() >= 21)
			{
				// tournament has ended
				tournament_end_stats();
			}
			else
			{
				// clear the table and setup the cards
				setup_new_round();
				// next round
				play();
			}
			return;
		}
		// if both players hands are empty, give them four cards each
		else if (human.get_hand_cards().empty() && computer.get_hand_cards().empty())
			deal_to_human_and_computer();
	}
}


/*************************************************************
Function Name       : move_handler
Purpose             : the handler for the moves.  calls the move menu if human player and get the move input or calls the get move function in the logic class if computer player. then executes the move
Parameters          : None
Return Value        : true if successful, false otherwise
Assistance Received : None
**************************************************************/
bool Game::move_handler()
{
	Move current_move;

	if (current_player->is_human())
		current_move = move_menu();
	else
	{
		current_move = game_logic.get_move(computer.get_hand_cards(), loose_cards, current_player->is_human());
		current_move.pretty_print();
	}

	return make_move(current_move);
}

/*************************************************************
Function Name       : get_int
Purpose             : get integer value for the passed in string
Parameters          : num_str -- string containing the number
					  num -- a pointer to an integer to store the converted number to
Return Value        : true if successful, false otherwise
Assistance Received : None
**************************************************************/
// converts the passed string to integer and stores it in the address of the second parameter
// returns true if success and false if not
bool Game::get_int(string num_str, int* num)
{
	try
	{
		*num = stoi(num_str);
	}
	catch (...)
	{
		return false;
	}

	return true;
}


/*************************************************************
Function Name       : choice_prompt
Purpose             : prompts the user for a choice depending on the input parameter and gets the chosen elements and returns them as a vector
Parameters          : choices -- choices for the prompt
Return Value        : the valid selections made by the user
Assistance Received : None
**************************************************************/
vector<string> Game::choice_prompt(vector<string> choices)
{
	if (choices.empty())
	{
		cout << "\tthere are no available choices\n";
		return vector<string>();
	}

	int choice_int, idx = 0;
	string choice_str, temp;
	istringstream ss;
	vector<string> choice_vec;

	cout << "choices (enter numbers space separated): \n";

	for (string item : choices)
		cout << "\t" << idx++ << ". " << item << "\n";

	cout << "\t: ";
	cin >> ws;
	getline(cin, choice_str);
	ss.str(choice_str);

	while (ss >> temp)
	{
		if (get_int(temp, &choice_int) && choice_int >= 0 && choice_int < choices.size())
			choice_vec.push_back(choices[choice_int]);
		else
			cout << "\tnot valid: " << temp << endl;
	}

	if (choice_vec.empty())
	{
		cout << endl;
		return choice_prompt(choices);
	}

	return choice_vec;
}


/*************************************************************
Function Name       : move_menu
Purpose             : called by move handler if human player is playing. gets and validates a complete move
Parameters          : None
Return Value        : a move object based on the selections
Assistance Received : None
**************************************************************/
Move Game::move_menu()
{
	istringstream ss;
	string choice_str;
	int choice = -1;

	string action = "", hand_card;
	vector<string> l_choice_cards;

	// option to pick a hand card
	cout << "hand card (type one): "; human.print_hand();
	while (true)
	{
		// get whole line as input and parse out the first word without any whitespace
		cout << "\t: ";
		cin >> ws;
		getline(cin, hand_card);
		ss.str(hand_card);
		ss >> hand_card;

		// transform all to upper case
		transform(hand_card.begin(), hand_card.end(), hand_card.begin(), ::toupper);

		if (hand_card.length() == 2 && human.check_in_hand(hand_card))
			break;

		cout << "\tinvalid choice. retry\n";
	}


	cout << "what do you want to do?  (enter number)\n"
		<< "\t1. " << ACTION_BUILD << "\n"
		<< "\t2. " << ACTION_EXTEND << "\n"
		<< "\t3. " << ACTION_MULTI << "\n"
		<< "\t4. " << ACTION_CAPTURE << "\n"
		<< "\t5. " << ACTION_TRAIL << "\n"
		<< "\t: ";

	cin >> choice_str;
	// filter out everything but integer input from cin
	while (!get_int(choice_str, &choice) || choice < 1 || choice > 5)
	{
		cout << "\tinvalid choice. retry\n"
			<< "\t: ";
		cin >> choice_str;
	}

	vector<string> opponent_builds, curr_builds, temp_vec;
	string temp_str;

	switch (choice)
	{
	case 1: // build
		action = ACTION_BUILD;

		l_choice_cards = choice_prompt(loose_cards);
		break;

	case 2: // extend
		action = ACTION_EXTEND;

		opponent_builds = current_builds.get_player_builds(!current_player->is_human());
		l_choice_cards = choice_prompt(opponent_builds);
		break;

	case 3: // multi
		action = ACTION_MULTI;

		/* for the build */
		curr_builds = current_builds.get_builds();
		// add loose cards to the choice list
		for (string s : loose_cards)
			curr_builds.push_back(s);

		l_choice_cards = choice_prompt(curr_builds);
		break;

	case 4: // capture
		action = ACTION_CAPTURE;

		/* for the build */
		curr_builds = current_builds.get_builds();
		// add loose cards to the choice list
		for (string s : loose_cards)
			curr_builds.push_back(s);

		l_choice_cards = choice_prompt(curr_builds);
		break;

	case 5: // trail
		action = ACTION_TRAIL;
		break;
	}

	Move ret = Move(action, hand_card, l_choice_cards);

	if (game_logic.check_move(ret, current_player->get_hand_cards(), this->loose_cards))
		return ret;
	else
	{
		cout << "\tinvalid move. retry\n\n";
		return move_menu();
	}
}

bool Game::make_move(Move execute)
{
	bool flag = true;

	if (execute.get_action() == ACTION_BUILD)
	{
		current_builds.create_build(execute.get_all_cards(), current_player->is_human());
	}
	else if (execute.get_action() == ACTION_EXTEND)
	{
		if (current_builds.is_build(execute.get_loose_cards().front()))
			current_builds.add_to_build(execute.get_loose_cards().front(), execute.get_hand_card());
		else flag = false;
	}
	else if (execute.get_action() == ACTION_MULTI)
	{
		string build;
		for (string s : execute.get_loose_cards())
		{
			if (current_builds.is_build(s))
				build = s;
		}

		vector<string> other_cards;
		for (string s : execute.get_all_cards())
		{
			if (s != build)
				other_cards.push_back(s);
		}

		current_builds.add_build_multi(build, other_cards, current_player->is_human());
	}
	else if (execute.get_action() == ACTION_CAPTURE)
	{
		current_player->capture_card(execute.get_hand_card());

		for (string s : execute.get_loose_cards())
		{
			// will also remove the build object from the table and all its cards
			if (current_builds.is_build(s))
			{
				vector<string> t_cards = current_builds.capture_build(s);

				for (string c : t_cards)
					current_player->capture_card(c);

				continue;
			}

			current_player->capture_card(s);
		}

		/***  also set the last capture here... 'h' for human and 'c' for comp  ***/
		if (current_player->is_human()) last_capture = 'h';
		else last_capture = 'c';
	}
	else if (execute.get_action() == ACTION_TRAIL)
	{
		// do nothing as the card is removed from hand below
		add_to_table(execute.get_hand_card());
	}
	else
		flag = false;

	if (flag)
	{
		// remove hand card from players hand
		current_player->remove_from_hand(execute.get_hand_card());

		// remove the used cards from the table
		for (string s : execute.get_loose_cards())
		{
			// dont do anything to builds as this is handled by the build class
			if (current_builds.is_build(s))
				continue;

			remove_from_table(s);
		}
	}

	return flag;
}

// routine to print out stats at the end of the tournament
void Game::tournament_end_stats()
{
	cout << "\n--- Tournament End ---\n";

	if (human.get_tournament_score() > computer.get_tournament_score())
		cout << "||  HUMAN wins  ||\n";
	else if (human.get_tournament_score() < computer.get_tournament_score())
		cout << "||  COMPUTER wins  ||\n";
	else
		cout << "||  GAME TIE  ||\n";

	cout << "----------------------\nexiting game...\n";
}

// prints the menu and gets the choice
int Game::get_menu_options()
{
	cout << "1. Save the game\n";
	cout << "2. Make a move\n";

	if (current_player->is_human())
		cout << "3. Ask for help\n";

	cout << "4. Quit the game\n";

	string choice_str;  int choice;

	cout << " : ";
	cin >> choice_str;

	// filter out everything but integer input from cin and in required range i.e. 1 - 4
	while (!get_int(choice_str, &choice) || choice < 1 || choice > 4 || (!current_player->is_human() && choice == 3))
	{
		cout << " invalid choice. retry\n"
			<< " : ";
		cin >> choice_str;
	}

	return choice;
}

// change the current_player pointer
void Game::change_player()
{
	if (current_player->is_human())
		current_player = &computer;
	else
		current_player = &human;
}

void Game::add_to_table(string card)
{
	loose_cards.push_back(card);
}

void Game::deal_to_human_and_computer()
{
	// give four cards to human
	for (int i = 0; i < 4; i++)
	{
		human.take_card(game_deck.get_one());
	}

	// give four cards to computer
	for (int i = 0; i < 4; i++)
	{
		computer.take_card(game_deck.get_one());
	}
}

string Game::get_table_cards_str(string separator)
{
	string ret = "";
	ret += current_builds.get_builds_str(separator);

	for (string card : loose_cards)
		ret += card + separator;

	return ret;
}

string Game::get_loose_cards_str(string separator)
{
	string ret = "";

	for (string card : loose_cards)
		ret += card + separator;

	return ret;
}

// do all automatic stuff here i.e. setting the game, menu is provided in parent
bool Game::setup_new_round()
{
	cout << "\nstarting new round...\n";

	current_builds = Build::Build();
	human.reset_for_new_round();
	computer.reset_for_new_round();

	game_logic = Logic::Logic(&current_builds);

	// initialize the game deck
	this->game_deck = Deck();

	// deal four cards to human and then computer
	deal_to_human_and_computer();

	// put four cards on the table
	for (int i = 0; i < 4; i++)
	{
		add_to_table(game_deck.get_one());
	}

	return true;
}

void Game::print_stats()
{
	cout << "human hand\n";
	human.print_hand();
	cout << "human captured\n";
	human.print_captured();

	cout << "comp hand\n";
	computer.print_hand();
	cout << "comp captured\n";
	computer.print_captured();

	cout << "table cards\n"
		<< get_table_cards_str() << endl;

	cout << "game deck after\n";
	game_deck.print_deck();

	cout << "build stuff\n";
	cout << current_builds.get_builds_str() << endl;
	cout << current_builds.get_builds_with_owner_str() << endl;


}

// process the total score from the captured cards and add to the respective tournament totals for both players
void Game::process_round_score()
{
	int human_score = 0, computer_score = 0;

	vector<string> human_cards = human.get_captured_cards();
	vector<string> computer_cards = computer.get_captured_cards();

	// addding three points to player with most captured cards
	if (human_cards.size() > computer_cards.size())
		human_score += 3;
	else if (human_cards.size() < computer_cards.size())
		computer_score += 3;

	int c_spade = 0, h_spade = 0;

	// calculations for human player's captured cards
	for (string s : human_cards)
	{
		// counting the number of spades in human cards
		if (s[0] == 'S')
			h_spade++;

		// player with 10 of diamond gets two points
		if (s == "DX")
			human_score += 2;

		// player with 2 of spade gets one points
		if (s == "S2")
			human_score += 1;

		// each ace earns a point
		if (s[1] == 'A')
			human_score += 1;
	}

	// calculations for computer player's captured cards
	for (string s : computer_cards)
	{
		// counting the number of spades in computer cards
		if (s[0] == 'S')
			c_spade++;

		// player with 10 of diamond gets two points
		if (s == "DX")
			computer_score += 2;

		// player with 2 of spade gets one points
		if (s == "S2")
			computer_score += 1;

		// each ace earns a point
		if (s[1] == 'A')
			computer_score += 1;
	}


	cout << "spades in human pile  : " << h_spade << "\n"
		<< "spades in comp  pile  : " << c_spade << "\n"
		<< "-----------------\n";
	if (h_spade > c_spade)
		human_score += 1;
	else if (h_spade < c_spade)
		computer_score += 1;

	// add the calculated round score to the tournament score in the individual player's object
	human.add_tournament_score(human_score);
	computer.add_tournament_score(computer_score);

	cout << "ROUND       || Human score: " << human_score << "  ||  Computer score: " << computer_score << endl;
	cout << "TOURNAMENT  || Human score: " << human.get_tournament_score() << "  ||  Computer score: " << computer.get_tournament_score() << endl;
}

string Game::vector_to_string(vector<string> cards_vector, string separator)
{
	string ret = "";
	for (string s : cards_vector)
		ret += s + separator;

	return ret;
}

bool Game::save_game()
{
	try
	{
		ofstream save_file(SAVE_FILE_NAME);
		save_file << "Round: " << round_number << "\n\n"
			<< "Computer:\n"
			<< "   Score: " << computer.get_tournament_score() << "\n"
			<< "   Hand: " << vector_to_string(computer.get_hand_cards()) << "\n"
			<< "   Pile: " << vector_to_string(computer.get_captured_cards()) << "\n\n"
			<< "Human:\n"
			<< "   Score: " << human.get_tournament_score() << "\n"
			<< "   Hand: " << vector_to_string(human.get_hand_cards()) << "\n"
			<< "   Pile: " << vector_to_string(human.get_captured_cards()) << "\n\n"
			<< "Table: " << get_table_cards_str() << "\n\n"
			<< "Build Owner: " << current_builds.get_builds_with_owner_str() << "\n\n"
			<< "Last Capturer: " << ((last_capture == 'h') ? "Human" : "Computer") << "\n\n"
			<< "Deck: " << vector_to_string(game_deck.get_deck()) << "\n\n"
			<< "Next Player: " << ((current_player->is_human()) ? "Human" : "Computer") << "\n";

		save_file.close();
		return true;
	}
	catch (...)
	{
		return false;
	}
}

void Game::remove_from_table(string card)
{
	vector<string>::iterator it = find(loose_cards.begin(), loose_cards.end(), card);
	loose_cards.erase(it);
}

