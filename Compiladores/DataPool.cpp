#include "stdafx.h"
#include "DataPool.h"
#include <algorithm>	
#include <stdexcept>

/*const DataPool::keyword_type DataPool::keywords {
	{"array", Token::ARRAY}, {"boolean", Token::BOOLEAN}, {"break", Token::BREAK}, {"char", Token::CHAR}, {"continue", Token::CONTINUE}, {"do", Token::DO},
	{"else", Token::ELSE}, {"false", Token::FALSE}, {"function", Token::FUNCTION}, {"if", Token::IF}, {"integer", Token::INTEGER}, {"of", Token::OF},
	{"string", Token::STRING}, {"struct", Token::STRUCT}, {"true", Token::TRUE}, {"type", Token::TYPE}, {"var", Token::VAR}, {"while", Token::WHILE}
};*/

DataPool DataPool::instance(MAX_CONSTANTS, MAX_IDS);



DataPool::DataPool(size_t maxConstants, size_t maxIds)
{
	InitializeKeywords();
	InitializeTokens();

	constantsList.reserve(maxConstants);
	integerConstants.reserve(maxConstants / 3);
	charConstants.reserve(maxConstants / 3);
	stringConstants.reserve(maxConstants / 3);

	names.reserve(maxIds);
}


DataPool::~DataPool()
{
}

Token DataPool::SearchKeyword(const std::string & name)
{
	auto it = instance.keywords.find(name);
	if (it == instance.keywords.end())
		return Token::ID;
	return it->second;
}

size_t DataPool::SearchName(std::string && name)
{
	auto it = std::find(instance.names.begin(), instance.names.end(), name);
	if (it == instance.names.end()) {
		instance.names.insert(it, std::move(name));
		return instance.names.size() - 1;
	}
	return it - instance.names.begin();
}

size_t DataPool::AddCharConstant(char ch)
{
	auto it = std::find(instance.charConstants.begin(), instance.charConstants.end(), ch);
	if (it == instance.charConstants.end()) {
		instance.charConstants.insert(it, ch);
		instance.constantsList.emplace_back(ConstType::CHAR, instance.charConstants.size() - 1);
		return instance.constantsList.size() - 1;
	}
	else {
		size_t pos = it - instance.charConstants.begin();
		return std::find(instance.constantsList.begin(), instance.constantsList.end(), std::make_pair( ConstType::CHAR, pos )) - instance.constantsList.begin();
	}
}

size_t DataPool::AddIntConstant(int num)
{
	auto it = std::find(instance.integerConstants.begin(), instance.integerConstants.end(), num);
	if (it == instance.integerConstants.end()) {
		instance.integerConstants.insert(it, num);
		instance.constantsList.emplace_back(ConstType::INTEGER, instance.constantsList.size() - 1);
		return instance.constantsList.size() - 1;
	}
	else {
		size_t pos = it - instance.integerConstants.begin();
		return std::find(instance.constantsList.begin(), instance.constantsList.end(), std::make_pair(ConstType::INTEGER, pos)) - instance.constantsList.begin();
	}
}

size_t DataPool::AddStringConst(std::string && str)
{
	auto it = std::find(instance.stringConstants.begin(), instance.stringConstants.end(), str);
	if (it == instance.stringConstants.end()) {
		instance.stringConstants.insert(it, std::move(str));
		instance.constantsList.emplace_back(ConstType::STRING, instance.integerConstants.size() - 1);
		return instance.constantsList.size() - 1;
	}
	else {
		size_t pos = it - instance.stringConstants.begin();
		return std::find(instance.constantsList.begin(), instance.constantsList.end(), std::make_pair(ConstType::STRING, pos)) - instance.constantsList.begin();
	}
}

char DataPool::GetCharConst(size_t pos)
{
	auto index = instance.constantsList[pos];
	if (index.first != ConstType::CHAR)
		throw std::logic_error("Constant in this position is not a character.");
	return instance.charConstants[index.second];
}

int DataPool::GetIntConst(size_t pos)
{
	auto index = instance.constantsList[pos];
	if (index.first != ConstType::INTEGER)
		throw std::logic_error("Constant in this position is not a integer.");
	return instance.integerConstants[index.second];
}

const std::string & DataPool::GetStringConst(size_t pos)
{
	auto index = instance.constantsList[pos];
	if (index.first != ConstType::STRING)
		throw std::logic_error("Constant in this position is not a string.");
	return instance.stringConstants[index.second];
}

void DataPool::InitializeKeywords()
{
	keywords.emplace("array", Token::ARRAY);
	keywords.emplace("boolean", Token::BOOLEAN);
	keywords.emplace("break", Token::BREAK);
	keywords.emplace("char", Token::CHAR);
	keywords.emplace("continue", Token::CONTINUE);
	keywords.emplace("do", Token::DO);
	keywords.emplace("else", Token::ELSE);
	keywords.emplace("false", Token::FALSE);
	keywords.emplace("function", Token::FUNCTION);
	keywords.emplace("if", Token::IF);
	keywords.emplace("integer", Token::INTEGER);
	keywords.emplace("of", Token::OF);
	keywords.emplace("string", Token::STRING);
	keywords.emplace("struct", Token::STRUCT);
	keywords.emplace("true", Token::TRUE);
	keywords.emplace("type", Token::TYPE);
	keywords.emplace("var", Token::VAR);
	keywords.emplace("while", Token::WHILE);
}

void DataPool::InitializeTokens()
{
	singleCharToken.emplace(':', Token::COLON);
	singleCharToken.emplace(';', Token::SEMICOLON);
	singleCharToken.emplace(',', Token::COMMA);
	singleCharToken.emplace('[', Token::LEFT_SQUARE);
	singleCharToken.emplace(']', Token::RIGHT_SQUARE);
	singleCharToken.emplace('{', Token::LEFT_BRACES);
	singleCharToken.emplace('}', Token::RIGHT_BRACES);
	singleCharToken.emplace('(', Token::LEFT_PARENTHESIS);
	singleCharToken.emplace(')', Token::RIGHT_PARENTHESIS);
	singleCharToken.emplace('*', Token::TIMES);
	singleCharToken.emplace('/', Token::DIVIDE);
	singleCharToken.emplace('.', Token::DOT);
}