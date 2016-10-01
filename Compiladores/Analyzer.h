#include <unordered_map>
#include <iostream>
#include <vector>

#include "Defines.h"
#include "DataPool.h"
#include "LexicalAnalyzer.h"
#include "Semantics.h"

#pragma once

#define ACTION_TABLE_SIZE 1587
#define RULE_TABLE_SIZE 

#define FINAL_ACTION 1

class Analyzer
{
	//Typedefs
	typedef std::pair<unsigned int, unsigned int> ActionIndex;
	typedef std::pair<unsigned int, NonTerminal> RuleRow;

	//Atributes
	LexicalAnalyzer& lexical;
	Semantics& semantics;

	std::map<ActionIndex, int> actionTable;
	std::vector<RuleRow> ruleTable;
	std::vector<unsigned int> syntaxStack;

	const unsigned int finalAction;

	//Private members
	inline void AddAction(unsigned int state, Token nextToken, int action) { AddAction(state, TokenIdx(nextToken), action); }
	inline void AddAction(unsigned int state, NonTerminal nextToken, int action) { AddAction(state, TokenIdx(nextToken), action); }
	inline void AddAction(unsigned int state, Token nextToken, Rules reduce) { AddAction(state, TokenIdx(nextToken), RuleNo(reduce)); }
	inline void AddAction(unsigned int state, NonTerminal nextToken, Rules reduce) { AddAction(state, TokenIdx(nextToken), RuleNo(reduce)); }
	inline void AddAction(unsigned int state, unsigned int nextToken, int action) { actionTable[std::make_pair(state, nextToken)] = action; }


	void InitializeActionTable();

	inline int NextAction(unsigned int state, unsigned int token) { return actionTable.at(std::make_pair(state, token)); }
	inline int NextAction(unsigned int state, Token token) { return NextAction(state, TokenIdx(token)); }
	inline int NextAction(unsigned int state, NonTerminal token) { return NextAction(state, TokenIdx(token)); }

	inline bool IsShift(int p) { return p > 0; }
	inline bool IsReduction(int p) { return p <= 0; }
	inline int Rule(int p) { return -p; }

	void InitializeRuleTable();

	inline void SyntaxError(size_t currLine) { std::cout << "An Syntax Error ocurred in line: " << currLine << std::endl; }

public:
	explicit Analyzer(LexicalAnalyzer& analizer, Semantics& semantics);
	~Analyzer();

	void Parse(const std::string& codeFile);
};

