#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class LexAnalyser {
private:
	vector<string> terminals = {}; /*Під час зчитування граматики з файла перше, що зчитується - алфавіти, потім правила*/
	vector<string> nonterminals = {};
	vector<string, vector<string>> rules = {}; /*Правила граматики за моїм концептом зберігаються у вигляді пар з нетермінала
	в лівій частині та вектора лексем правої частини*/

	vector<string>epsilonNonTerm = {};
	vector<string, vector<string>> firstKVec = {};
	vector<string, vector<string>> followKVec = {}; /*ці множини зберігаються в такому самому вигляді, що й правила*/
	
	//тут мають бути атрибути під таблицю
	
	void ReadGrammar(string path){} //зчитування граматики
public:
	LexAnalyser(string path){} //конструктор приймає шлях до файлу з граматикою та викликає метод під зчитування

	void FirstK(){}
	void FollowK(){}
	
	void Epsilons(){}//потенційно необхідна функція під знаходження епсилон-нетерміналів

	void Table(){}//побудова таблиці

	void Analysis(string input){} //аналіз введеної послідовності


};