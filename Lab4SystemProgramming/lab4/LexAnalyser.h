#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class LexAnalyser {
private:
	vector<string> terminals = {}; /*ϳ� ��� ���������� ��������� � ����� �����, �� ��������� - �������, ���� �������*/
	vector<string> nonterminals = {};
	vector<string, vector<string>> rules = {}; /*������� ��������� �� ��� ��������� ����������� � ������ ��� � ����������
	� ��� ������ �� ������� ������ ����� �������*/

	vector<string>epsilonNonTerm = {};
	vector<string, vector<string>> firstKVec = {};
	vector<string, vector<string>> followKVec = {}; /*�� ������� ����������� � ������ ������ ������, �� � �������*/
	
	//��� ����� ���� �������� �� �������
	
	void ReadGrammar(string path){} //���������� ���������
public:
	LexAnalyser(string path){} //����������� ������ ���� �� ����� � ���������� �� ������� ����� �� ����������

	void FirstK(){}
	void FollowK(){}
	
	void Epsilons(){}//���������� ��������� ������� �� ����������� �������-����������

	void Table(){}//�������� �������

	void Analysis(string input){} //����� ������� �����������


};