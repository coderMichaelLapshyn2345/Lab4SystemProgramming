#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <list>
#include <fstream>
#include <map>
#include <algorithm>
using namespace std;

class LexAnalyser {
private:
	vector<char> terminals = {}; /*When reading the grammar, the two alphabets are read, the all the inference rules*/
	vector<char> nonterminals = {};
	vector<pair<char, vector<char>>> rules = {}; /*In my concept the rules are stored as pairs of nonterminal
	(left part of the rule) and vector of lexems (right part)*/

	vector<string>epsilonNonTerm = {};
	map<string, vector<string>> firstKMap = {};
	map<string, vector<string>> followKMap = {}; /*these arrays are stored in the same way as the rules*/
	
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
    vector<string> combineAllWords(vector<string> first, vector<string> second)
    {
        vector<string> result;
        for (auto const& s1 : first)
        {
            for (auto const& s2 : second)
            {
                string newWord = s1 + s2;
                if (find(result.begin(), result.end(), newWord) == result.end())
                {
                    result.push_back(newWord);
                }
            }
        }
        return result;
    }
   
    vector<string> combineAllWords(vector<string> first, char second)
    {
        vector<string> result;
        for (auto const& st1 : first)
        {
            string secondS(1, second);
            string newWord = st1 + secondS;
            if (find(result.begin(), result.end(), newWord) == result.end())
            {
                result.push_back(newWord);
            }
        }
        return result;
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
    map<string, vector<string>> FirstK(){
        for(const auto& currentRule: rules){
            vector<string> terminals;
            for(const auto& rule: rules){
                if(rule.first != currentRule.first) continue;
                if(IsNonTerminal(rule.second[0])){
                    string terminal(1, rule.second[0]);
                    if(find(terminals.begin(), terminals.end(), terminal) == terminals.end()){
                        terminals.push_back(terminal);
                    }
                }
            }
            firstKMap[string(1, currentRule.first)] = terminals;
        }
        bool isNotChanged = false;
        while(!isNotChanged){
            for(auto& currentNonTerminalFirstK: firstKMap){
                string currentNonTerminal = currentNonTerminalFirstK.first;
                vector<string> previousStepTerminals = currentNonTerminalFirstK.second;
                for(const auto& rule: rules){
                    if(string(1, rule.first) == currentNonTerminal){
                        vector<string> previousNonTerminalFirstK = firstKMap[string(1, rule.second[0])];
                        vector<string> combinedWords = previousNonTerminalFirstK;
                        
                        if(IsNonTerminal(rule.second[0])){
                            if(previousStepTerminals == currentNonTerminalFirstK.second){
                                isNotChanged = true;
                            }
                            continue;
                        }
                        if(combinedWords.empty()){
                            if(previousStepTerminals == currentNonTerminalFirstK.second){
                                isNotChanged = true;
                            }
                            continue;
                        }
                        for(int i = 0; i < rule.second.size(); ++i){
                            if(combinedWords.empty()) continue;
                            if(IsNonTerminal(rule.second[i])){
                                vector<string> tmp = firstKMap[string(1, rule.second[i])];
                                if(tmp.empty())
                                {
                                    continue;
                                    combinedWords.clear();
                                }
                                combinedWords = combineAllWords(combinedWords, tmp);
                            }
                            else
                            {
                                combinedWords = combineAllWords(combinedWords, rule.second[i]);
                            }
                        }
                        vector<string> firstChars = GetAllFirstCharactersOfWords(combinedWords);
                        for(const auto& fc: firstChars){
                            if(find(currentNonTerminalFirstK.second.begin(), currentNonTerminalFirstK.second.end(), fc) == currentNonTerminalFirstK.second.end()){
                                currentNonTerminalFirstK.second.push_back(fc);
                            }
                        }
                        if(previousStepTerminals != currentNonTerminalFirstK.second){
                            isNotChanged = true;
                        }
                    }
                }
            }
        }
        return this->firstKMap;
    }
    
    
    
    // Function to detect Follow_1 set
	map<string, vector<string>> FollowK(map<string, vector<string>> firstK, vector<string> epsilon){
        for(int i = 0; i < rules.size(); ++i){
            string left_non_terminal = string(1, rules[i].first);
            vector <string> follow;
            if(left_non_terminal == "S"){
                follow.push_back("e");
            }
            for(int j = 0; j < rules.size(); ++j){
                int at = -1;
                auto it = find(rules[j].second.begin(), rules[j].second.end(), left_non_terminal);
                if(it != rules[j].second.end()){
                    at = (int)distance(rules[j].second.begin(), it);
                }
                if(at == rules[j].second.size() - 1){
                    string symbol = "";
                    symbol += rules[j].second[at];
                    if(string(1, rules[j].first) == symbol){
                        continue;
                    }
                    else {
                        vector<string> followLeft = followKMap[string(1, rules[j].first)];
                        for(int k = 0; k < followLeft.size(); ++k){
                            follow.push_back(followLeft[k]);
                        }
                    }
                }
                else {
                    int eps = 0;
                    for(int p = at + 1; p < rules[j].second.size(); ++p){
                        if(IsNonTerminal(rules[j].second[p])){
                            string symbol = "";
                            symbol += rules[j].second[p];
                            follow.push_back(symbol);
                            break;
                        }
                        else {
                            string symbol = "";
                            symbol += rules[j].second[p];
                            int count = 0;
                            for(const auto& str: epsilonNonTerm){
                                if(str.find(symbol) != string::npos){
                                    count++;
                                }
                            }
                            vector<string> first = firstK[symbol];
                            for(int k = 0; k < first.size(); ++k){
                                follow.push_back(first[k]);
                            }
                        }
                    }
                    if(eps == rules[j].second.size() - at + 1){
                        vector<string> followLeft = followKMap[string(1, rules[j].first)];
                        for(int k = 0; k < followLeft.size(); ++k){
                            follow.push_back(followLeft[k]);
                        }
                    }
                }
                
            }
            vector<string> followSet;
            for(int l = 0; l < follow.size(); ++l){
                followSet.push_back(follow[l]);
                
            }
            vector<string> helper(followSet.begin(), followSet.end());
            followKMap[left_non_terminal] = helper;
        }
        
        return this->followKMap;
    }
	
	vector<string> Epsilons(){
        for(int i = 0; i < rules.size(); i++){
            if(rules[i].second[i] == 'e'){
                epsilonNonTerm.push_back(string(1, rules[i].first));
            }
        }
        for(int i = 0; i < rules.size(); ++i){
            int count = 0;
            vector<char> end = rules[i].second;
            for(int j = 0; j < end.size(); ++j){
                string ch = "";
                ch += end[j];
                for(const auto& str: epsilonNonTerm){
                    if(str.find(ch) != string::npos){
                        count++;
                    }
                }
            }
            if(count == rules[i].second.size()){
                epsilonNonTerm.push_back(string(1, rules[i].first));
            }
        }
        return this->epsilonNonTerm;
    }

	        void Table()
        {
            

            for (auto & rule : rules)
            {
                
                string start = "";
                start += rule.first;
                vector<string> end = {};
                string r0 = "";
                for (char r : rule.second) {
                    end.push_back(r0 += r); r0 = "";}

                // Визначаємо FIRST для end
                vector<string> firstSet;

                for (int i = 0; i < end.size(); ++i)
                {
                    char currentChar = end[i][0];
                    if (IsNonTerminal(currentChar))
                    {
                        vector<string> firstOfNonTerminal = firstKMap[string(1,currentChar)];
                        firstSet.insert(firstSet.end(), firstOfNonTerminal.begin(), firstOfNonTerminal.end());

                        // Якщо epsilon не є в FIRST[nonTerminal], перериваємо цикл
                        if (find(firstOfNonTerminal.begin(), firstOfNonTerminal.end(), "e") == firstOfNonTerminal.end())
                            break;
                    }
                    else
                    {
                        char currentTerminal=currentChar;
                        firstSet.push_back(string(1,currentTerminal));
                        break;
                    }
                }
                int ruleNum;
                // Додаємо FIRST до таблиці
                for (string& terminal : firstSet)
                {
                    if (terminal != "e"){ // Якщо epsilon не є в FIRST, додаємо запис в таблицю
                        for (int j = 0; j < rules.size(); j++) if (string(1, rules[j].first) == start) ruleNum = j;
                    parsingTable[{start, terminal}] = ruleNum;
                }
                    else
                    {
                        // Якщо epsilon є в FIRST, додаємо FOLLOW[start] до таблиці
                        vector<string> followSet = followKMap[start];
                        
                        for (string& followTerminal : followSet)
                        {
                            for (int j = 0; j < rules.size(); j++) if (string(1, rules[j].first) == start) ruleNum = j;
                            parsingTable[{start, followTerminal}] = ruleNum;
                        }
                    }
                }
            }
        
}//table creation

void Analysis(string input){
    stack<string> parseStack;
    parseStack.push("$");  // Bottom of the stack
    parseStack.push("S");  // Starting symbol

    size_t inputIndex = 0;
    while (!parseStack.empty())
    {

        // Get the top of the stack and the current input symbol
        string stackTop = parseStack.top();
        string currentInput = (inputIndex < input.size()) ? string(1, input[inputIndex]) : "";

        // Check if the stack top is a non-terminal
        if (IsNonTerminal(stackTop[0]))
        {
            // Look up the production in the parsing table
            auto it = parsingTable.find({ stackTop, currentInput });
            if (it != parsingTable.end())
            {
                string production = it->second;

                // Pop the non-terminal from the stack
                parseStack.pop();

                // Push the production onto the stack in reverse order
                for (int i = production.size() - 1; i >= 0; --i)
                {
                    if (production[i] != 'e')  // Skip epsilon
                        parseStack.push(string(1, production[i]));
                }

            }
            else
            {
                // Error handling: no production in the parsing table
                cout << "Error: No production for " << stackTop << " and input " << currentInput;
                break;
            }
        }
        else if (stackTop == currentInput)  // Check if the stack top matches the current input symbol
        {
            // Matched terminal, pop from stack and move to the next input symbol
            parseStack.pop();
            ++inputIndex;
        }
        else if (stackTop == "$" && currentInput.length() == 0) {
            cout << "Parsing ended successfully";
            break;
        }
        else
        {
            // Error handling: stack top and input do not match
            cout << "Error: Mismatch between stack top " << stackTop << " and input " << currentInput;
            break;
        }

        cout << endl;
    }

    cout << endl;

    if (inputIndex == input.size() && !parseStack.empty() && parseStack.top() == "$" && parseStack.top().length() == 1)
        cout << "Parsing successful!" << endl;
    else
        cout << "Parsing failed!" << endl;
} //analysis of the input string


};
