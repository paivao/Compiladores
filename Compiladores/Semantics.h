#include <forward_list>
#include <vector>
#include <list>
#include <iostream>
#include <memory>

#include "Defines.h"
#include "DataPool.h"

#pragma once

/*
class ObjectList {
public:
	struct ObjectNode {
		int name;
		Kind kind;

		ObjectNode * prev, * next;

		ObjectNode(int name, Kind kind = Kind::NO_KIND_DEF) : name(name), kind(kind), prev(nullptr), next(nullptr) {}
		virtual ~ObjectNode() { if (prev) delete prev; if (next) delete next; }

	};

	struct VPFObjectNode : public ObjectNode {
		VPFObjectNode(int name, ObjectNode * type, Kind kind) : ObjectNode(name, kind), type(type) {}
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

	enum PrimitiveIndex {
		INT = 0,
		CHAR,
		BOOL,
		STRING,
		UNIVERSAL,
	};

private:
	ObjectNode * first, * last;
	

public:
	ObjectList() : first(nullptr), last(nullptr) {}
	ObjectList(ObjectList && temp) { first = temp.first; last = temp.last; temp.first = temp.last = nullptr; };
	~ObjectList() { if (first) delete first; }

	ObjectList& operator=(ObjectList &&temp) { first = temp.first; last = temp.last; temp.first = temp.last = nullptr; return *this; }

	ObjectNode * Define(int name);
	ObjectNode * Search(int name);

	static ObjectNode* getPrimitive(PrimitiveIndex idx);

	ObjectNode * setVar(ObjectNode * node, ObjectNode * type);
	ObjectNode * setParam(ObjectNode * node, ObjectNode * type);
	ObjectNode * setField(ObjectNode * node, ObjectNode * type);
	ObjectNode * setFunction(ObjectNode * node, ObjectNode * ret, ObjectNode * params);
	ObjectNode * setArray(ObjectNode * node, ObjectNode * elemType, int numElems);
	ObjectNode * setStruct(ObjectNode * node, ObjectNode * fields);
	ObjectNode * setAlias(ObjectNode * node, ObjectNode * base);
};
*/

class Semantics
{

	struct ObjectNode {
		struct VPFObject {
			ObjectNode* type;
		};
		struct FunctionObject {
			ObjectNode* ret;
			ObjectNode* params;
		};
		struct StructObject {
			ObjectNode* fields;
		};
		struct ArrayObject {
			ObjectNode * type;
			int numElms;
		};
		struct AliasObject {
			ObjectNode * baseType;
		};

		int name;
		Kind kind;
		ObjectNode * next;

		ObjectNode(int name, Kind kind = Kind::NO_KIND_DEF) : name(name), kind(kind), next(nullptr) {}

		union {
			VPFObject var, param, field;
			FunctionObject function;
			ArrayObject arr;
			StructObject str;
			AliasObject alias;
		} __;
		
		inline VPFObject& asVar() { return __.var; }
		inline VPFObject& asParam() { return __.param; }
		inline VPFObject& asField() { return __.field; }
		inline FunctionObject& asFunction() { return __.function; }
		inline ArrayObject& asArray() { return __.arr; }
		inline StructObject& asStruct() { return __.str; }
		inline AliasObject& asAlias() { return __.alias; }
	};

	struct SemanticNode {
		struct SemanticID {
			int name;
			ObjectNode* obj;
		};
		struct SemanticT {
			ObjectNode* type;
		};
		struct SemanticL {
			ObjectNode* list;
		};
		struct SemanticBOOL {
			ObjectNode * type;
			bool val;
		};
		struct SemanticSTR {
			ObjectNode * type;
			int pos;
			const std::string * val;
		};
		struct SemanticCHR {
			ObjectNode * type;
			int pos;
			char val;
		};
		struct SemanticNUM {
			ObjectNode * type;
			int pos;
			int val;
		};

		NonTerminal nont;
		union {
			SemanticID id;
			SemanticT t;
			SemanticL l;
			SemanticBOOL bool_val;
			SemanticCHR chr;
			SemanticSTR str;
			SemanticNUM num;
		} __;

		inline SemanticID& asID() { return __.id; }
		inline SemanticT& asT() { return __.t; }
		inline SemanticL& asL() { return __.l; }
		inline SemanticBOOL& asBool() { return __.bool_val; }
		inline SemanticCHR& asChr() { return __.chr; }
		inline SemanticNUM& asNum() { return __.num; }
		inline SemanticSTR& asStr() { return __.str; }
	};

	enum PrimitiveIndex {
		INT = 0,
		CHAR,
		BOOL,
		STRING,
		UNIVERSAL,
	};

	static ObjectNode primitives[5];
	static inline ObjectNode * getPrimitive(PrimitiveIndex idx) { return &primitives[idx]; }
	typedef std::pair<ObjectNode *, ObjectNode *> BoundPointers;

	std::vector<BoundPointers> symbolTable;

	SemanticNode semanticStack[SEMANTIC_STACK_SIZE];
	size_t stackSize;

	int NewBlock();
	int EndBlock();

	BoundPointers almostDeleted;

	ObjectNode * define(int name);
	ObjectNode * search(int name);
	ObjectNode * find(int name);

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
	void __TRUE();
	void __FALSE();
	void __CHAR(unsigned int secondaryToken);
	void __STRING(unsigned int secondaryToken);
	void __NUMERAL(unsigned int secondaryToken);

	void Error(ErrorType code);

	inline void pushSemantic(SemanticNode & node);
	inline SemanticNode & getSemantic(size_t rpos);
	inline void popSemantic(size_t qtd);
	
public:
	Semantics();
	~Semantics();

	void operator()(Rules rule, unsigned int secondaryToken);
};