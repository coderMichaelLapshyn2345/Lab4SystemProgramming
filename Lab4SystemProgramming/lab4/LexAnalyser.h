#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <list>
using namespace std;

class LexAnalyser {
private:
	vector<char> terminals = {}; /*When reading the grammar, the two alphabets are read, the all the inference rules*/
	vector<char> nonterminals = {};
	vector<pair<char, vector<char>>> rules = {}; /*In my concept the rules are stored as pairs of nonterminal
	(left part of the rule) and vector of lexems (right part)*/

	vector<char>epsilonNonTerm = {};
	vector<pair<char, vector<char>>> firstKVec = {};
	vector<pair<char, vector<char>>> followKVec = {}; /*these arrays are stored in the same way as the rules*/
	
	//there should be some attributes for the syntax table
	
	void ReadGrammar(string path){} //reading of the grammar
public:
	LexAnalyser(string path){} //constructor takes the path to the file containing grammar and supposedly calls the method to read it

	void FirstK(){}
	void FollowK(){}
	
	void Epsilons(){}//potentially necessary function for locating epsilon-nonterminals

	void Table(){}//table creation

	void Analysis(string input){} //analysis of the input string


};
