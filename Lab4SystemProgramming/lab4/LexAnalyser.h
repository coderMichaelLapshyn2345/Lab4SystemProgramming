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

        int addNonTerminal(char symbol) { //minor function for nonterminal adding
	        for (int nonTerminalIndex = 0; nonTerminalIndex < nonterminals.size(); nonTerminalIndex++) {
		        if (nonterminals[nonTerminalIndex] == symbol) return 0; 
		}
	        return 1; 
	}

        int addTerminal(char symbol) { //minor function for terminal adding
	        if (addNonTerminal(symbol) == 0) return 0;
	        for (int terminalIndex = 0; terminalIndex < terminals.size(); terminalIndex++) {
		        if (terminals[terminalIndex] == symbol)	return 0;
		}
	        return 1;
	}

        int getRule(FILE *input) { //adds next rule if it exists, updates "terminals", "nonterminals" and "rules"
                /*Assuming rule has the following format: first symbol (non-terminal) - left part,
                next symbols - right part. Rule is ended by ';'. If rule consists of first symbol
                and ';' and only, it will be interpreted as epsilon-rule*/
		char left_part, right_part_symbol;
		vector<char> right_part;
		if (fscanf(input, "%c", &left_part) == EOF) return -1; //there is no more rules: end of file is reached
		while (left_part == '\n' || left_part == ' ') { //ignoring whitespace characters
			if (fscanf(input, "%c", &left_part) == EOF) return -1; //there is no more rules: end of file is reached
		}
		if (addNonTerminal(left_part) == 1) nonterminals.push_back(left_part); //adding nonterminal
		fscanf(input, "%c", &right_part_symbol);
		while (right_part_symbol == '\n' || right_part_symbol == ' ') { //ignoring whitespace characters
			fscanf(input, "%c", &right_part_symbol);
		}
		while (right_part_symbol != ';') { //';' means current rule is fully read
			right_part.push_back(right_part_symbol);
			fscanf(input, "%c", &right_part_symbol);
			while (right_part_symbol == '\n' || right_part_symbol == ' ') { //ignoring whitespace characters
				fscanf(input, "%c", &right_part_symbol);
			}
		}
		rules.push_back(make_pair(left_part, right_part)); //adding current rule to vector "rules" 
		return 0;
	}

	void ReadGrammar(string path){ //reading of the grammar
		FILE *input;
		input = fopen(path.c_str(), "r");
		int checker = getRule(input);
		while (checker != -1) { //reading rule by rule from file while it's end is not reached
			checker = getRule(input);
		}
		//recording terminal alphabet
		for (int ruleIndex = 0; ruleIndex < rules.size(); ruleIndex++) { //iterating over rules
			for (int symbolIndex = 0; symbolIndex < (rules[ruleIndex].second).size(); symbolIndex++) { //iterating over right part of the rule
				if (addTerminal((rules[ruleIndex].second)[symbolIndex]) == 1) terminals.push_back((rules[ruleIndex].second)[symbolIndex]);
			}
		}
	}
public:
	LexAnalyser(string path){ //constructor takes the path to the file containing grammar and supposedly calls the method to read it
		ReadGrammar(path);
	}

	void FirstK(){}
	void FollowK(){}
	
	void Epsilons(){}//potentially necessary function for locating epsilon-nonterminals

	void Table(){}//table creation

	void Analysis(string input){} //analysis of the input string


};
