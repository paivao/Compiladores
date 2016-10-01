#include <forward_list>
#include <vector>
#include <iostream>

#include "Defines.h"

#pragma once

struct ObjectNode {
	int name;
	ObjectNode* next;
	Kind kind;

	ObjectNode(int name, Kind kind = Kind::NO_KIND_DEF, ObjectNode* next = nullptr) : name(name), kind(kind), next(next) {}
	virtual ~ObjectNode() { if (next) delete next; }
};

struct VarObjectNode : public ObjectNode {
	ObjectNode * type;
};

struct FunctionObjectNode : public ObjectNode {
	ObjectNode * returnType;
	ObjectNode * params;
};

struct ArrayObjectNode : public ObjectNode {
	ObjectNode * elemType;
	size_t numElems;
};

struct StructObjectNode : public ObjectNode {
	ObjectNode * fields;
};

struct AliasObjectNode : public ObjectNode {
	ObjectNode * baseType;
};

class Semantics
{

	struct SemanticNode {
		NonTerminal nont;
		union {
			struct {
				int name;
				ObjectNode* obj;
			} ID;
			struct {
				ObjectNode * type;
			} T;
			struct {
				ObjectNode * list;
			} LI;
		} _;
	};

	std::vector<ObjectNode*> symbolTable;
	std::vector<ObjectNode*> symbolTableLast;
	ObjectNode primitiveTypes[5];

	enum PrimitiveIndex {
		INT = 0,
		CHAR,
		BOOL,
		STRING,
		UNIVERSAL,
	};

	std::vector<SemanticNode> semanticStack;

	int currentLevel;

	int NewBlock();
	int EndBlock();

	ObjectNode* Define(int name);
	ObjectNode* Search(int name);
	ObjectNode* Find(int name);

	//RULE METHODS
	void __IDU_RULE(unsigned int secondaryToken);
	void __IDD_RULE(unsigned int secondaryToken);
	void __ID_RULE(unsigned int secondaryToken);
	void __NB_RULE();
	void __DT_RULE();
	void __DF_RULE();
	void __TYPE_INT();
	void __TYPE_CHR();
	void __TYPE_STR();
	void __TYPE_BOL();
	void __TYPE_IDU();
	void __LI();
	void __LI_TAIL();
	void __DV();

	void Error(ErrorType code);
	
public:
	Semantics();
	~Semantics();

	void operator()(Rules rule, unsigned int secondaryToken);
};
