#include <stdio.h>
#include <iostream>
#include "LexAnalyser.h"
using namespace std;

int main()
{
    //тут буде взаємодія з користувачем
    string path, input;
	printf("Enter file name (+ .txt) to read grammar from: ");
	scanf(" %s", path.c_str());LexAnalyser grammar(path);
	printf("Enter input string to analyse by given grammar: ");
	scanf(" %s", input.c_str());grammar.Analysis(input);
	system("pause");
	return 0;
}


