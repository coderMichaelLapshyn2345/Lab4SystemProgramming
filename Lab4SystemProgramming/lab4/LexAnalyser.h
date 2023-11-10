#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <utility>
using namespace std;

class LexAnalyser {
private:
	vector<string> terminals = {}; /*When reading the grammar, the two alphabets are read, the all the inference rules*/
	vector<string> nonterminals = {};
	vector<pair<string, vector<string>>> rules = {}; /*In my concept the rules are stored as pairs of nonterminal
	(left part of the rule) and vector of lexems (right part)*/

	vector<string>epsilonNonTerm = {};
	vector<pair<string, vector<string>>> firstKVec = {};
	vector<pair<string, vector<string>>> followKVec = {}; /*these arrays are stored in the same way as the rules*/
	
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
