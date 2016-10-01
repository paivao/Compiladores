#include <fstream>
#include "Defines.h"
#include "DataPool.h"

#pragma once

class LexicalAnalyzer
{
	std::ifstream sourceCode;
	char nextChar;
	size_t secondaryToken;

	size_t currentLine;

public:
	LexicalAnalyzer();
	~LexicalAnalyzer();

	bool OpenSourceFile(const std::string& filename);

	Token NextToken();
	inline size_t GetSecondaryToken() const { return secondaryToken; }
	inline size_t GetCurrentLine() const { return currentLine; }
};

