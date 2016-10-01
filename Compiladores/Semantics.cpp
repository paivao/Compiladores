#include "stdafx.h"
#include "Semantics.h"

Semantics::Semantics() : primitiveTypes{ {-1, Kind::SCALAR_TYPE, nullptr}, { -1, Kind::SCALAR_TYPE, nullptr }, { -1, Kind::SCALAR_TYPE, nullptr }, { -1, Kind::SCALAR_TYPE, nullptr }, { -1, Kind::SCALAR_TYPE, nullptr } }
{
	currentLevel = 0;
	symbolTable.push_back(nullptr);
	symbolTableLast.push_back(nullptr);
}


Semantics::~Semantics()
{
	for (auto a : symbolTable)
		delete a;
}

int Semantics::NewBlock()
{
	currentLevel++;
	symbolTable.push_back(nullptr);
	symbolTableLast.push_back(nullptr);
	return currentLevel;
}

int Semantics::EndBlock()
{
	return --currentLevel;
}

ObjectNode * Semantics::Define(int name)
{
	ObjectNode* obj = new ObjectNode(name);

	if (symbolTable[currentLevel]) {
		symbolTableLast[currentLevel]->next = obj;
		symbolTableLast[currentLevel] = obj;
	}
	else {
		symbolTable[currentLevel] = symbolTableLast[currentLevel] = obj;
	}
	return obj;
}

ObjectNode * Semantics::Search(int name)
{
	ObjectNode* obj = symbolTable[currentLevel];
	while (obj) {
		if (obj->name == name)
			break;
		else
			obj = obj->next;
	}
	return obj;
}

ObjectNode * Semantics::Find(int name)
{
	ObjectNode* obj = nullptr;

	for (auto a : symbolTable) {
		obj = a;
		while (obj) {
			if (obj->name = name)
				break;
			else
				obj = obj->next;
		}
		if (obj)
			break;
	}
	return obj;
}

void Semantics::__IDU_RULE(unsigned int secondaryToken)
{
	SemanticNode IDU;
	IDU.nont = NonTerminal::IDU;
	IDU._.ID.name = secondaryToken;

	if (!(IDU._.ID.obj = Find(secondaryToken))) {
		Error(ErrorType::NOT_DECLARED);
		IDU._.ID.obj = Define(secondaryToken);
	}

	semanticStack.push_back(IDU);
}

void Semantics::__IDD_RULE(unsigned int secondaryToken)
{
	SemanticNode IDD;
	IDD.nont = NonTerminal::IDD;
	IDD._.ID.name = secondaryToken;

	if (IDD._.ID.obj = Search(secondaryToken)) {
		Error(ErrorType::REDECLATION);
	}
	else {
		IDD._.ID.obj = Define(secondaryToken);
	}

	semanticStack.push_back(IDD);
}

void Semantics::__ID_RULE(unsigned int secondaryToken)
{
	SemanticNode ID;
	ID.nont = NonTerminal::ID;
	ID._.ID.name = secondaryToken;
	ID._.ID.obj = nullptr;

	semanticStack.push_back(ID);
}

void Semantics::__NB_RULE()
{
	NewBlock();
}

void Semantics::__DT_RULE()
{
	EndBlock();
}

void Semantics::__DF_RULE()
{
	EndBlock();
}

void Semantics::__TYPE_INT()
{
	SemanticNode type;
	type.nont = NonTerminal::TP;
	type._.T.type = &primitiveTypes[INT];
	semanticStack.push_back(type);
}

void Semantics::__TYPE_CHR()
{
	SemanticNode type;
	type.nont = NonTerminal::TP;
	type._.T.type = &primitiveTypes[CHAR];
	semanticStack.push_back(type);
}

void Semantics::__TYPE_STR()
{
	SemanticNode type;
	type.nont = NonTerminal::TP;
	type._.T.type = &primitiveTypes[STRING];
	semanticStack.push_back(type);
}

void Semantics::__TYPE_BOL()
{
	SemanticNode type;
	type.nont = NonTerminal::TP;
	type._.T.type = &primitiveTypes[BOOL];
	semanticStack.push_back(type);
}

void Semantics::__TYPE_IDU()
{
	SemanticNode T;
	SemanticNode IDU = semanticStack.back();
	semanticStack.pop_back();
	ObjectNode * p = IDU._.ID.obj;
	if (isKindType(p->kind) || p->kind == Kind::UNIVERSAL) {
		T._.T.type = p;
	}
	else {
		T._.T.type = &primitiveTypes[UNIVERSAL];
		Error(ErrorType::TYPE_EXPECTED);
	}
	semanticStack.push_back(T);
}

void Semantics::__LI()
{
	SemanticNode LI1 = semanticStack.back();
	semanticStack.pop_back();
	SemanticNode LI0;
	LI0._.LI.list = LI1._.LI.list;
	semanticStack.push_back(LI0);
}

void Semantics::__LI_TAIL()
{
	SemanticNode IDD = semanticStack.back();
	semanticStack.pop_back();
	SemanticNode LI;
	LI._.LI.list = IDD._.ID.obj;
	semanticStack.push_back(LI);
}

void Semantics::__DV()
{
	SemanticNode TP, LI;
	TP = semanticStack.back();
	LI = semanticStack.at(semanticStack.size() - 2);
	semanticStack.resize(semanticStack.size() - 2);
	ObjectNode * p;
	while (p != nullptr && p->kind == Kind::NO_KIND_DEF) {
		p->kind = Kind::VAR;
		
	}
}

void Semantics::Error(ErrorType code)
{
	switch (code) {
	case ErrorType::NOT_DECLARED:
		std::cout << "Erro! Variavel nao declarada." << std::endl;
		break;
	case ErrorType::REDECLATION:
		std::cout << "Erro! Redeclaracao de variavel." << std::endl;
		break;
	default:
		std::cout << "Erro semantico desconhecido!" << std::endl;
	}
}

void Semantics::operator()(Rules rule, unsigned int secondaryToken)
{
	switch (rule) {
	case Rules::DF:
		std::cout << "DF Rule called." << std::endl;
		__DF_RULE();
		break;
	case Rules::DT_SIMP:
	case Rules::DT_ARRAY:
	case Rules::DT_STRUCT:
		std::cout << "DT Rule called." << std::endl;
		__DT_RULE();
		break;
	case Rules::IDD_ID:
		std::cout << "IDD Rule called." << std::endl;
		__IDD_RULE(secondaryToken);
		break;
	case Rules::IDU_ID:
		std::cout << "IDU Rule called." << std::endl;
		__IDU_RULE(secondaryToken);
		break;
	case Rules::ID:
		std::cout << "ID Rule called." << std::endl;
		__ID_RULE(secondaryToken);
		break;
	case Rules::NB:
		std::cout << "NB Rule called." << std::endl;
		__NB_RULE();
		break;
	//default:
		//Error(ErrorType::INVALID_RULE);
	}
}
