#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <list>
#include <fstream>
#include <map>
#include <set>
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
    vector<string> combineAllWords(vector<string> first, vector<char> second)
    {
        vector<string> result;
        for (auto const& st1 : first)
        {
            for (auto const& ch2 : second)
            {
                string secondS(1, ch2);
                string newWord = st1 + secondS;
                if (find(result.begin(), result.end(), newWord) == result.end())
                {
                    result.push_back(newWord);
                }
            }
        }
        return result;
    }
    char getFirstNotEmptyChar(string word)
    {
        for (int i = 0; i < word.size(); i++)
        {
            if (word[i] != 'e') return word[i];
        }
        return 'e';
    }
    vector<string> GetAllFirstCharactersOfWords(vector<string> words)
    {
        vector<string> result;
        for (auto const& w : words)
        {
            char firstChar = getFirstNotEmptyChar(w);
            string firstChatStr = string(1, firstChar);
            if (find(result.begin(), result.end(), firstChatStr) != result.end()) continue;
            result.push_back(firstChatStr);
        }
        return result;
    }
    
    // Function to get First_k set
    vector<pair<char, vector<char>>> First_k(vector<pair<char, vector<char>>> rules){
        map<string, vector<string>> result;
            for (auto const& currentTransition : rules) //adding first step terminals to the result
            {
                vector<string> terminals;
                for (auto const& transition : rules)
                {
                    if (transition.first != currentTransition.first) continue;
                    if (IsNonTerminal(transition.second[0]))
                    {
                        string terminal(1, transition.second[0]);
                        if (find(terminals.begin(), terminals.end(), terminal) == terminals.end())
                        {
                            terminals.push_back(terminal);
                        }
                    }
                }
                result[string(1,currentTransition.first)] = terminals;
            }

            bool isNotChanged = false;//flag that detects if previous step is equal to the current
            while (!isNotChanged)
            {
                for (auto & currentNonTerminalFirstK : result)
                {
                    string currentNonTerminal = currentNonTerminalFirstK.first;

                    vector<string> previousStepTerminals = currentNonTerminalFirstK.second;
                    for (auto const& transition : rules)
                                {
                                    if (transition.first == currentNonTerminal[0])
                                    {
                                        vector<string> previousNonTerminalFirstK = result[string(1, transition.second[0])];
                                        vector<string> combinedWords = previousNonTerminalFirstK;

                                        if (IsNonTerminal(transition.second[0]))
                                        {
                                            if (previousStepTerminals == currentNonTerminalFirstK.second)
                                            {
                                                isNotChanged = true;
                                            }
                                            continue;
                                        }

                                        if (combinedWords.empty())
                                        {
                                            if (previousStepTerminals == currentNonTerminalFirstK.second)
                                            {
                                                isNotChanged = true;
                                            }
                                            continue;
                                        }
                                        for (int i = 1; i < transition.second.size(); i++)
                                        {
                                            if (combinedWords.empty()) continue;
                                            if (IsNonTerminal(transition.second[i]))
                                            {
                                                vector<string> temp = result[string(1, transition.second[i])];
                                                if (temp.empty())
                                                {
                                                    continue;
                                                    combinedWords.clear();
                                                }
                                                vector<char> chars;
                                                for(const auto& str: temp){
                                                    for(char ch: str){
                                                        chars.push_back(ch);
                                                    }
                                                }
                                                
                                                combinedWords = combineAllWords(combinedWords, chars);
                                            }
                                            else
                                            {
                                                                           
                                                combinedWords = combineAllWords(combinedWords, transition.second);
                                            }
                                        }
                                        vector<string> firstChars = GetAllFirstCharactersOfWords(combinedWords);
                                        for (auto const& fc : firstChars)
                                        {
                                            if (find(currentNonTerminalFirstK.second.begin(), currentNonTerminalFirstK.second.end(), fc) == currentNonTerminalFirstK.second.end())
                                            {
                                                currentNonTerminalFirstK.second.push_back(fc);
                                            }
                                        }

                                        if (previousStepTerminals != currentNonTerminalFirstK.second)
                                        {
                                            isNotChanged = true;
                                        }
                                    }
                                }
                            }
                        }
                    map<string, vector<string>> result2;
                    for (auto& element : result)
                    {
                        if (IsNonTerminal(element.first[0]))
                        {
                            result2[element.first] = element.second;
                        }
                    }
                    for(const auto& e: result2){
                        vector<char> tmp;
                        for(const auto& str: e.second){
                            for(char c: str){
                                tmp.push_back(c);
                            }
                        }
                        firstKVec.push_back(make_pair(e.first[0], tmp));
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
