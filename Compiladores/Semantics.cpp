#include "stdafx.h"
#include "Semantics.h"

Semantics::ObjectNode Semantics::primitives[5] = { Semantics::ObjectNode(-1, Kind::SCALAR_TYPE), Semantics::ObjectNode(-1, Kind::SCALAR_TYPE), Semantics::ObjectNode(-1, Kind::SCALAR_TYPE), Semantics::ObjectNode(-1, Kind::SCALAR_TYPE), Semantics::ObjectNode(-1, Kind::SCALAR_TYPE) };

Semantics::Semantics()
{
	stackSize = 0;
	symbolTable.push_back(std::make_pair(nullptr, nullptr));
}


Semantics::~Semantics()
{
}

int Semantics::NewBlock()
{
	symbolTable.push_back(std::make_pair(nullptr, nullptr));
}

int Semantics::EndBlock()
{
	almostDeleted = std::move(symbolTable.back());
	symbolTable.resize(symbolTable.size() - 1);
}

Semantics::ObjectNode * Semantics::define(int name)
{
	ObjectNode * p = new ObjectNode(name);

	if (symbolTable.back().first) {
		symbolTable.back().second->next = p;
		symbolTable.back().second = p;
	}
	else {
		symbolTable.back().first = symbolTable.back().second = p;
	}
	return nullptr;
}

Semantics::ObjectNode * Semantics::search(int name)
{
	ObjectNode * p = symbolTable.back().first;
	while (p) {
		if (p->name == name)
			return p;
		p = p->next;
	}
	return nullptr;
}

Semantics::ObjectNode * Semantics::find(int name)
{
	ObjectNode * p;
	for (auto it = symbolTable.rbegin(); it != symbolTable.rend(); ++it) {
		p = it->first;
		while (p) {
			if (p->name == name)
				return p;
			p = p->next;
		}
	}
	return nullptr;
}

void Semantics::__IDU_RULE(unsigned int secondaryToken)
{
	SemanticNode IDU;
	ObjectNode * p = find(secondaryToken);

	if (!p) {
		Error(ErrorType::NOT_DECLARED);
		p = define(secondaryToken);
	}

	IDU.nont = NonTerminal::IDU;
	IDU.asID().name = secondaryToken;
	IDU.asID().obj = p;

	pushSemantic(IDU);
}

void Semantics::__IDD_RULE(unsigned int secondaryToken)
{
	SemanticNode IDD;
	IDD.nont = NonTerminal::IDD;

	ObjectNode * p;

	if (p = search(secondaryToken)) {
		Error(ErrorType::REDECLATION);
	}
	else {
		p = define(secondaryToken);
	}

	IDD.asID().name = secondaryToken;
	IDD.asID().obj = p;

	pushSemantic(IDD);
}

void Semantics::__ID_RULE(unsigned int secondaryToken)
{
	SemanticNode ID;
	ID.nont = NonTerminal::ID;
	ID.asID().name = secondaryToken;
	ID.asID().obj = nullptr;

	pushSemantic(ID);
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
	SemanticNode TP;
	TP.nont = NonTerminal::TP;
	TP.asT().type = getPrimitive(INT);

	pushSemantic(TP);
}

void Semantics::__TYPE_CHR()
{
	SemanticNode TP;
	TP.nont = NonTerminal::TP;
	TP.asT().type = getPrimitive(CHAR);

	pushSemantic(TP);
}

void Semantics::__TYPE_STR()
{
	SemanticNode TP;
	TP.nont = NonTerminal::TP;
	TP.asT().type = getPrimitive(STRING);

	pushSemantic(TP);
}

void Semantics::__TYPE_BOL()
{
	SemanticNode TP;
	TP.nont = NonTerminal::TP;
	TP.asT().type = getPrimitive(BOOL);

	pushSemantic(TP);
}

void Semantics::__TYPE_IDU()
{
	SemanticNode T;
	SemanticNode& IDU = getSemantic(0);
	popSemantic(1);
	ObjectNode * p = IDU.asID().obj;
	if (isKindType(p->kind) || p->kind == Kind::UNIVERSAL) {
		T.asT().type = p;
	}
	else {
		T.asT().type = getPrimitive(UNIVERSAL);
		Error(ErrorType::TYPE_EXPECTED);
	}
	pushSemantic(T);
}

void Semantics::__LI()
{
	SemanticNode LI1 = getSemantic(0);
	popSemantic(1);
	SemanticNode LI0;
	LI0.nont = NonTerminal::LI;
	LI0.asL().list = LI1.asL().list;
	pushSemantic(LI0);
}

void Semantics::__LI_TAIL()
{
	SemanticNode IDD = getSemantic(0);
	popSemantic(1);
	SemanticNode LI;
	LI.nont = NonTerminal::LI;
	LI.asL().list = IDD.asID().obj;
	pushSemantic(LI);
}

void Semantics::__DV()
{
	SemanticNode TP = getSemantic(0);
	SemanticNode LI = getSemantic(1);
	popSemantic(2);
	ObjectNode * p = LI.asL().list;
	while (p != nullptr && p->kind == Kind::NO_KIND_DEF) {
		p->kind = Kind::VAR;
		p->asVar().type = TP.asT().type;
		p = p->next;
	}
}

void Semantics::__TRUE()
{
	SemanticNode TRUE;
	TRUE.nont = NonTerminal::TRUE;
	TRUE.asBool().type = getPrimitive(BOOL);
	TRUE.asBool().val = true;
	pushSemantic(TRUE);
}

void Semantics::__FALSE()
{
	SemanticNode FALSE;
	FALSE.nont = NonTerminal::FALSE;
	FALSE.asBool().type = getPrimitive(BOOL);
	FALSE.asBool().val = false;
	pushSemantic(FALSE);
}

void Semantics::__CHAR(unsigned int secondaryToken)
{
	SemanticNode CHR;
	CHR.nont = NonTerminal::CHR;
	CHR.asChr().type = getPrimitive(CHAR);
	CHR.asChr().pos = secondaryToken;
	CHR.asChr().val = DataPool::GetCharConst(secondaryToken);
	pushSemantic(CHR);
}

void Semantics::__STRING(unsigned int secondaryToken)
{
	SemanticNode STR;
	STR.nont = NonTerminal::STR;
	STR.asStr().type = getPrimitive(STRING);
	STR.asStr().pos = secondaryToken;
	STR.asStr().val = &DataPool::GetStringConst(secondaryToken);
	pushSemantic(STR);
}

void Semantics::__NUMERAL(unsigned int secondaryToken)
{
	SemanticNode NUM;
	NUM.nont = NonTerminal::NUM;
	NUM.asNum().type = getPrimitive(INT);
	NUM.asNum().pos = secondaryToken;
	NUM.asNum().val = DataPool::GetIntConst(secondaryToken);
	pushSemantic(NUM);
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

inline void Semantics::pushSemantic(SemanticNode& node)
{
	semanticStack[stackSize++] = node;
}

inline Semantics::SemanticNode& Semantics::getSemantic(size_t rpos)
{
	return semanticStack[stackSize - rpos - 1];
}

inline void Semantics::popSemantic(size_t qtd)
{
	stackSize -= qtd;
}

/*
ObjectList::ObjectNode * ObjectList::Define(int name)
{
	ObjectNode* obj = new ObjectNode(name);

	if (last) {
		obj->prev = last;
		last->next = obj;
		last = obj;
	}
	else {
		first = last = obj;
	}

	return obj;
}

ObjectList::ObjectNode * ObjectList::Search(int name)
{
	ObjectNode * obj = first;

	while (obj) {
		if (obj->name == name)
			break;
		else
			obj = obj->next;
	}

	return obj;
}

ObjectList::ObjectNode * ObjectList::getPrimitive(PrimitiveIndex idx)
{
	return &primitives[idx];
}

ObjectList::ObjectNode * ObjectList::setVar(ObjectNode * node, ObjectNode * type)
{
	if (node->kind != Kind::NO_KIND_DEF)
		return nullptr;

	VPFObjectNode * newNode = new VPFObjectNode(node->name, type, Kind::VAR);

	newNode->prev = node->prev;
	newNode->next = node->next;
	node->next = node->prev = nullptr;

	if (node->prev)
		node->prev->next = newNode;
	else
		first = newNode;

	if (node->next)
		node->next->prev = newNode;
	else
		last = newNode;

	delete node;
	return newNode;
}

ObjectList::ObjectNode * ObjectList::setParam(ObjectNode * node, ObjectNode * type)
{
	return nullptr;
}

ObjectList::ObjectNode * ObjectList::setField(ObjectNode * node, ObjectNode * type)
{
	return nullptr;
}

ObjectList::ObjectNode * ObjectList::setFunction(ObjectNode * node, ObjectNode * ret, ObjectNode * params)
{
	return nullptr;
}

ObjectList::ObjectNode * ObjectList::setArray(ObjectNode * node, ObjectNode * elemType, int numElems)
{
	return nullptr;
}

ObjectList::ObjectNode * ObjectList::setStruct(ObjectNode * node, ObjectNode * fields)
{
	return nullptr;
}

ObjectList::ObjectNode * ObjectList::setAlias(ObjectNode * node, ObjectNode * base)
{
	return nullptr;
}
*/

