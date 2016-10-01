// Compiladores.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "Defines.h"
#include "LexicalAnalyzer.h"
#include "Analyzer.h"
#include "Semantics.h"


int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cout << "Not enough arguments.";
		return -1;
	}
	std::string source(argv[1]);

	LexicalAnalyzer lexical;
	Semantics semantics;
	Analyzer parser(lexical, semantics);

	parser.Parse(source);

	return 0;
}

