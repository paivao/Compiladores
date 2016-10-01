#include <map>
#include <vector>
#include "Defines.h"

#pragma once

class DataPool
{
	enum class ConstType : uint_fast8_t { CHAR, INTEGER, STRING };

	static DataPool instance;

	std::map<std::string, Token> keywords;
	std::map<char, Token> singleCharToken;

	std::vector<std::string> names;

	std::vector<std::pair<ConstType, size_t>> constantsList;

	std::vector<std::string> stringConstants;
	std::vector<int> integerConstants;
	std::vector<char> charConstants;

	void InitializeKeywords();
	void InitializeTokens();

public:
	explicit DataPool(size_t maxConstants, size_t maxIds);
	~DataPool();

	static Token SearchKeyword(const std::string& name);

	static size_t SearchName(std::string&& name);

	static size_t AddCharConstant(char ch);
	static size_t AddIntConstant(int num);
	static size_t AddStringConst(std::string&& str);

	static char GetCharConst(size_t pos);
	static int GetIntConst(size_t pos);
	static const std::string& GetStringConst(size_t pos);

	static inline Token GetSingleCharToken(char ch) { try { return instance.singleCharToken.at(ch); } catch (std::out_of_range) { return Token::UNKNOWN; } }
};
