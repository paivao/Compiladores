#include "stdafx.h"
#include "LexicalAnalyzer.h"
#include <cctype>
#include <string>


LexicalAnalyzer::LexicalAnalyzer()
{
	nextChar = ' ';
	currentLine = 1;
}


LexicalAnalyzer::~LexicalAnalyzer()
{
	if (sourceCode.is_open())
		sourceCode.close();
}

bool LexicalAnalyzer::OpenSourceFile(const std::string & filename)
{
	if (sourceCode.is_open()) {
		if (sourceCode.eof()) {
			sourceCode.close();
		}
		else {
			sourceCode.close();
			throw std::logic_error("Source code was not completely tokenized.");
		}
	}
	currentLine = 1;
	sourceCode.open(filename);
}

Token LexicalAnalyzer::NextToken()
{
	Token token;
	while (std::isspace(nextChar)) {
		if (nextChar == '\n')
			currentLine++;
		nextChar = sourceCode.get();
	}
	if (std::isalpha(nextChar)) {
		std::string text;
		text.reserve(MAX_ID_LENGHT);

		do {
			text += nextChar;
			nextChar = sourceCode.get();
		} while (std::isalnum(nextChar) || nextChar == '_');
		
		token = DataPool::SearchKeyword(text);

		if (token == Token::ID)
			secondaryToken = DataPool::SearchName(std::move(text));
	}
	else if (std::isdigit(nextChar)) {
		std::string numeral;
		numeral.reserve(MAX_NUM_LENGTH);

		do {
			numeral += nextChar;
			nextChar = sourceCode.get();
		} while (std::isdigit(nextChar));

		secondaryToken = DataPool::AddIntConstant(std::stoi(numeral));
		token = Token::NUMERAL;
	}
	else if (nextChar == '"') {
		std::string str;
		str.reserve(MAX_STR_LENGTH);

		do {
			str += nextChar;
			nextChar = sourceCode.get();
		} while (nextChar != '"');
		str += '"';

		secondaryToken = DataPool::AddStringConst(std::move(str));
		token = Token::STRINGVAL;
	}
	else {
		switch (nextChar) {
		case '\'':
			nextChar = sourceCode.get();
			secondaryToken = DataPool::AddCharConstant(nextChar);
			sourceCode.get();
			nextChar = sourceCode.get();
			token = Token::CHARACTER;
			break;
		case '+':
			nextChar = sourceCode.get();
			if (nextChar == '+') {
				nextChar = sourceCode.get();
				token = Token::PLUS_PLUS;
			}
			else {
				token = Token::PLUS;
			}
			break;
		case '-':
			nextChar = sourceCode.get();
			if (nextChar == '-') {
				nextChar = sourceCode.get();
				token = Token::MINUS_MINUS;
			}
			else {
				token = Token::MINUS;
			}
			break;
		case '=':
			nextChar = sourceCode.get();
			if (nextChar == '=') {
				nextChar = sourceCode.get();
				token = Token::EQUAL_EQUAL;
			}
			else {
				token = Token::EQUALS;
			}
			break;
		case '!':
			nextChar = sourceCode.get();
			if (nextChar == '=') {
				nextChar = sourceCode.get();
				token = Token::NOT_EQUAL;
			}
			else {
				token = Token::NOT;
			}
			break;
		case '<':
			nextChar = sourceCode.get();
			if (nextChar == '=') {
				nextChar = sourceCode.get();
				token = Token::LESS_OR_EQUAL;
			}
			else {
				token = Token::LESS_THAN;
			}
			break;
		case '>':
			nextChar = sourceCode.get();
			if (nextChar == '=') {
				nextChar = sourceCode.get();
				token = Token::GREATER_OR_EQUAL;
			}
			else {
				token = Token::GREATER_THAN;
			}
			break;
		case '&':
			nextChar = sourceCode.get();
			if (nextChar == '&') {
				nextChar = sourceCode.get();
				token = Token::AND;
			}
			else {
				token = Token::UNKNOWN;
			}
			break;
		case '|':
			nextChar = sourceCode.get();
			if (nextChar == '|') {
				nextChar = sourceCode.get();
				token = Token::OR;
			}
			else {
				token = Token::UNKNOWN;
			}
			break;
		case std::char_traits<char>::eof():
			token = Token::END_OF_CODE;
			break;
		default:
			token = DataPool::GetSingleCharToken(nextChar);
			nextChar = sourceCode.get();
		}
	}

	return token;
}
