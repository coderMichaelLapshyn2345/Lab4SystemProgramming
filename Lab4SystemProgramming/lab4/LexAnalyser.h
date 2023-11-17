#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <list>
#include <fstream>
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

        int getRule(FILE* input) { //adds next rule if it exists, updates "terminals", "nonterminals" and "rules"
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
    bool IsTerminal(char symbol){
        if(find(terminals.begin(), terminals.end(), symbol) != terminals.end()){
            return true;
        }
        return false;
    }
    bool IsNonTerminal(char symbol){
        if(find(nonterminals.begin(), nonterminals.end(), symbol) != nonterminals.end()){
            return true;
        }
        return false;
    }
    
    // Function to get First_k set
	vector<pair<char, vector<char>>> FirstK(char symbol, int k){
        LexAnalyser lexAnalyzer("grammar.txt");
        // When non-terminal is present in the grammar rules
        // through checking the vector of non-terminals
//        if(find(nonterminals.begin(), nonterminals.end(), nonTerminal) != nonterminals.end()){
//            vector<char> rightPart;
//            string rpartToString;
//            for(const auto& pair: rules){
//                if(pair.first == nonTerminal){
//                    rightPart = pair.second;
//                    rpartToString = string(rightPart.begin(), rightPart.end());
//                }
//            }
//            // Got the rules for instance for A -> abbab | epsilon
//            // from previous loop
//            // Now we need to get a string "abbab"
//            int indexOfDelim = (int)rpartToString.rfind("|");
//            vector<char> beforeDelim;
//            vector<char> afterDelim;
//            for(int i = 0; i < indexOfDelim; i++){
//                beforeDelim[i] = rpartToString[i];
//            }
//            for(int i = indexOfDelim; i < rpartToString.size(); i++){
//                afterDelim[i] = rpartToString[i];
//            }
//            // Got two separated strings
//            string before = string(beforeDelim.begin(), beforeDelim.end());
//            string after = string(afterDelim.begin(), afterDelim.end());
//            if(k > indexOfDelim){
//                for(int i = 0; i <= k; i++){
//                    terminals.push_back(before[i]);
//                    firstKVec.push_back(make_pair(nonTerminal, terminals));
//                }
//                for(int i = indexOfDelim + 1; i < after.length(); i++){
//                    for(int j = i; j <= k; j++){
//                        terminals.push_back(after[j]);
//                        firstKVec.push_back(make_pair(nonTerminal, terminals));
//                    }
//                }
//            }
//            else {
//                for(int i = 0; i <= k; i++){
//                    terminals.push_back(before[i]);
//                    firstKVec.push_back(make_pair(nonTerminal, terminals));
//                }
//            }
//        }
//        return this->firstKVec;
        vector<char> rightPart;
        vector<char> result;
        string toString;
        int pos;
        for(const auto& p: rules){
            if(IsNonTerminal(p.first)){
                rightPart = p.second;
                toString = string(rightPart.begin(), rightPart.end());
                pos = (int)toString.rfind(p.first);
                if(pos != string::npos){
                    rightPart.erase(rightPart.begin() + pos);
                    rightPart.insert(rightPart.begin() + pos, p.second.begin(), p.second.end());
                }
                int indexOfDelimiter = (int)toString.find("|");
                string beforeDelimiter, afterDelimiter;
                if(k > indexOfDelimiter){
                    for(int i = 0; i < indexOfDelimiter; i++){
                        beforeDelimiter[i] = toString[i];
                        for(int j = i + 2; j < toString.length(); j++){
                            afterDelimiter[j] = toString[j];
                        }
                    }
                    for(int i = 0; i < indexOfDelimiter; i++){
                        result.push_back(beforeDelimiter[i]);
                    }
                    for(int j = indexOfDelimiter + 1; j <= k; j++){
                        result.push_back(afterDelimiter[j]);
                    }
                    firstKVec.push_back(make_pair(symbol, result));
                 }
                else if(k < indexOfDelimiter){
                    for(int i = 0; i <= k; i++){
                        result.push_back(toString[i]);
                    }
                    firstKVec.push_back(make_pair(symbol, result));
                }
            }
            else if(IsTerminal(symbol)){
                toString = string(rightPart.begin(), rightPart.end());
                string before, after;
                int pos = 0;
                int index = (int)toString.find("|");
                if(k > index && k < toString.length()){
                    for(int i = 0; i < index; i++){
                        before[i] = toString[i];
                    }
                    for(int j = index + 1; j < toString.length(); j++){
                        after[pos] = toString[j];
                        pos++;
                    }
                    for(int i = 0; i < index; i++){
                        result.push_back(before[i]);
                    }
                    for(int j = index + 1; j <= k; j++){
                        result.push_back(after[j]);
                    }
                    firstKVec.push_back(make_pair(symbol, result));
                }
                else if(k < index){
                    for(int i = 0; i <= k; i++){
                        result.push_back(toString[i]);
                    }
                    firstKVec.push_back(make_pair(symbol, result));
                }
            }
        }
        
        return this->firstKVec;
    }
	void FollowK(){
        
    }
	
	void Epsilons(){
        
    }//potentially necessary function for locating epsilon-nonterminals

	void Table(){
        
    }//table creation

	void Analysis(string input){
        
    } //analysis of the input string


};
