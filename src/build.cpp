#include "build.h"

/*
 *************************************************************
 * Name:     Abish Jha                                       *
 * Project:  Casino CPP                                      *
 * Class:    CMPS 366 Oraganization of Programming Languages *
 * Date:     October 24, 2018                                *
 *************************************************************
*/

Build::Build()
{
	builds.clear();
	owner.clear();
	sum.clear();
}

void Build::create_build(string b_build, bool b_is_human, int b_sum)
{
	// add the build to the list of builds, exit if not a build
	if (is_build(b_build))
		builds.push_back(b_build);
	else
		return;

	// set the owner of the build
	if (b_is_human)
		owner[b_build] = HUMAN;
	else
		owner[b_build] = COMPUTER;

	// calculate sum if not provided or store provided sum
	if (b_sum == -1)
		sum[b_build] = get_build_sum(b_build);
	else
		sum[b_build] = b_sum;
}

void Build::create_build(vector<string> cards, bool is_human, int sum)
{
	bool first = true;

	string b_builder = "[";
	for (string card : cards)
	{
		if (first) first = false;
		else b_builder += " ";

		b_builder += card;
	}
	b_builder += "]";

	create_build(b_builder, is_human, sum);
}

// is_human for new owner
void Build::add_build_multi(string build, vector<string> other_cards, bool is_human)
{
	int sum_local = sum[build];

	delete_build(build);

	string b_builder;
	bool first;

	if (is_build(build) && !is_multi_build(build))
	{
		first = true;

		// build multi build string
		b_builder = "[ " + build + " [";
		for (string card : other_cards)
		{
			if (first) first = false;
			else b_builder += " ";

			b_builder += card;
		}
		b_builder += "] ]";
	}
	else if (is_multi_build(build))
	{
		first = true;

		size_t strt = build.find("[");
		b_builder = build.substr(0, strt + 1);
		b_builder += " [";
		for (string card : other_cards)
		{
			if (first) first = false;
			else b_builder += " ";

			b_builder += card;
		}
		b_builder += "] " + build.substr(strt + 2);
	}

	builds.push_back(b_builder);
	sum[b_builder] = sum_local;
	owner[b_builder] = ((is_human) ? HUMAN : COMPUTER);
}


bool Build::is_build(string build)
{
	if ((build.find("[") != string::npos) && (build.find("]") != string::npos))
		return true;

	return false;
}

bool Build::is_multi_build(string build)
{
	// look for first '['
	size_t found = build.find("[");
	// look for second '['
	found = build.find("[", found + 1, 1);

	if (found != string::npos)
		return true;

	return false;
}

vector<string> Build::get_builds()
{
	return builds;
}

string Build::get_builds_str(string separator)
{
	string ret = "";
	for (string build : builds)
		ret += build + separator;

	return ret;
}

string Build::get_builds_with_owner_str(string separator)
{
	string ret = "";
	for (string build : builds)
		ret += build + " " + owner[build] + separator;

	return ret;
}

// goes through a string and return the start and end position of the first build
bool Build::get_first_build_start_end_pos(string build, size_t *start, size_t *end)
{
	if (!is_build(build))
		return false;

	int open_b = 0, close_b = 0;
	bool first_b = false; // first bracket found flag

	for (int i = 0; i < build.length(); i++)
	{
		if (build[i] == '[')
		{
			open_b++;

			if (!first_b)
			{
				*start = i;
				first_b = true;
			}
		}
		else if (build[i] == ']')
			close_b++;

		if (first_b && (open_b == close_b))
		{
			*end = i;
			return true;
		}
	}

	// inconsistent number of open and close bracket
	return false;
}

// to be used for the loading serialization
vector<string> Build::parse_builds_str(string build)
{
	size_t start, end;

	while (get_first_build_start_end_pos(build, &start, &end))
	{
		int length = end - start + 1;

		string parsed_build = build.substr(start, length);
		builds.push_back(parsed_build);

		// remove the parsed build from the input string
		build.erase(start, length);

		sum[parsed_build] = get_build_sum(parsed_build);
	}

	// parse and return loose cards in a vector for table
	istringstream ss(build);
	string card;
	vector<string> ret_list;

	while (ss >> card)
		ret_list.push_back(card);

	return ret_list;
}

// to be used for the loading serialization
void Build::parse_builds_with_owner_str(string build_w_owner)
{
	for (string build : builds)
	{
		size_t idx = build_w_owner.find(build);
		if (idx == string::npos)
		{
			cout << "[error] build owner not found for " << build << endl;
			continue;
		}

		istringstream ss(build_w_owner.substr(idx + build.length() + 1));
		string word;
		ss >> word;

		if (word == "Human" || word == "human")
			owner[build] = HUMAN;
		else
			owner[build] = COMPUTER;
	}
}

bool Build::delete_build(string build)
{
	// find iterator to element in builds vector
	vector<string>::iterator it = find(builds.begin(), builds.end(), build);

	// build does not exist
	if (it == builds.end())
		return false;

	builds.erase(it);
	// remove from maps
	owner.erase(build);
	sum.erase(build);
	return true;
}

// remove the build because somebody captured it and got all the cards
vector<string> Build::capture_build(string build)
{
	if (delete_build(build))
		return get_cards_in_build(build);
	else
		return vector<string>();
}

void Build::change_ownership(string build)
{
	// change the map that points from build to owner
	if (owner[build] == COMPUTER)
		owner[build] = HUMAN;
	else
		owner[build] = COMPUTER;
}

string Build::get_owner(string build)
{
	return owner[build];
}

vector<string> Build::get_cards_in_build(string build)
{
	// divide build into individual cards so it can be added to a player's
	// captured pile
	vector<string> cards;
	build.erase(remove(build.begin(), build.end(), '['), build.end());
	build.erase(remove(build.begin(), build.end(), ']'), build.end());

	istringstream ss(build);
	string card;

	while (ss >> card)
		cards.push_back(card);

	return cards;
}

int Build::get_build_sum(string build)
{
	// get sum from map for pre-calculated builds
	if (sum.find(build) != sum.end())
		return sum[build];

	// calculate sum for new builds
	size_t end = build.find("]");
	if (end == string::npos)
		return -1; // not a build

	size_t begin = build.find("[");
	while (true)
	{
		size_t next = build.find("[", begin + 1, 1);

		if (next == string::npos || next > end)
			break;

		begin = next;
	}

	vector<string> cards = get_cards_in_build(build.substr(begin, end - begin + 1));
	int _sum = 0, val;
	for (string card : cards)
	{
		val = Logic::get_card_value(card);
		if (val == -1)
			return -1; // invalid card in build

		_sum += val;
	}

	return _sum;
}

// check if player has any builds
bool Build::has_a_build(bool is_human)
{
	for (string build : builds)
	{
		if (is_human && owner[build] == HUMAN)
			return true;
		else if (!is_human && owner[build] == COMPUTER)
			return true;
	}
	return false;
}

vector<string> Build::get_player_builds(bool is_human)
{
	vector<string> ret;

	for (string build : builds)
	{
		if (is_human && owner[build] == HUMAN)
			ret.push_back(build);
		else if (!is_human && owner[build] == COMPUTER)
			ret.push_back(build);
	}

	return ret;
}

// will have to see if i need this function
vector<int> Build::get_player_build_sums(bool is_human)
{
	vector<int> player_sums;

	for (string build : builds)
	{
		if (is_human && owner[build] == HUMAN)
			player_sums.push_back(sum[build]);
		else if (!is_human && owner[build] == COMPUTER)
			player_sums.push_back(sum[build]);
	}

	return player_sums;
}

vector<int> Build::get_all_build_sums()
{
	vector<int> sums;
	for (string build : builds)
		sums.push_back(sum[build]);

	return sums;
}

string Build::add_to_build(string build, string new_card)
{
	// extending a build can only be done to the opponent's build and the ownership is changed therewith
	string b_owner = owner[build];
	bool is_human = false;
	if (b_owner == HUMAN)
		is_human = true;

	// remove old build
	delete_build(build);

	// create new build
	vector<string> tot_cards = get_cards_in_build(build);
	tot_cards.push_back(new_card);

	// as the ownership is transferred for the build
	create_build(tot_cards, !is_human);

	return "";
}

void Build::print_stats()
{
	cout << "build with owner "
		<< get_builds_with_owner_str() << endl;
}
