#pragma once

#define MAX_CONSTANTS 2048
#define MAX_IDS 2048
#define MAX_ID_LENGHT 128
#define MAX_NUM_LENGTH 32
#define MAX_STR_LENGTH 128
#define SYNTAX_STACK_SIZE 128
#define SEMANTIC_STACK_SIZE 128

enum class Token : unsigned int {
	//RESERVED WORDS
	//keyword array
	ARRAY = 1 << 15,
	//keyword boolean
	BOOLEAN,
	//keyword beak
	BREAK,
	//keyword char
	CHAR,
	//keyword continue
	CONTINUE,
	//keyword do
	DO,
	//keyword else
	ELSE,
	//keyword false
	FALSE,
	//keyword function
	FUNCTION,
	//keyword if
	IF,
	//keyword integer
	INTEGER,
	//keyword of
	OF,
	//keyword string
	STRING,
	//keyword struct
	STRUCT,
	//keyword true
	TRUE,
	//keyword type
	TYPE,
	//keyword var
	VAR,
	//keyword while
	WHILE,

	//SYMBOLS
	COLON,
	SEMICOLON,
	COMMA,
	EQUALS,
	LEFT_SQUARE,
	RIGHT_SQUARE,
	LEFT_BRACES,
	RIGHT_BRACES,
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	AND,
	OR,
	LESS_THAN,
	GREATER_THAN,
	LESS_OR_EQUAL,
	GREATER_OR_EQUAL,
	NOT_EQUAL,
	EQUAL_EQUAL,
	PLUS,
	PLUS_PLUS,
	MINUS,
	MINUS_MINUS,
	TIMES,
	DIVIDE,
	DOT,
	NOT,

	//REGULAR TOKENS
	//constant char
	CHARACTER,
	//constant numeral
	NUMERAL,
	//constant string
	STRINGVAL,
	//Id
	ID,

	//EOF
	END_OF_CODE,

	//UNKNOWN
	UNKNOWN
};

enum class NonTerminal : unsigned int {
	CODE,
	P,
	LDE,
	DE,
	DF,
	DT,
	IDD,
	NUM,
	TP,
	NB,
	DC,
	IDU,
	LI,
	LP,
	B,
	LDV,
	DV,
	LS,
	S,
	E,
	LV,
	L,
	R,
	TM,
	F,
	TRUE,
	FALSE,
	CHR,
	STR,
	LE,
	ID,
};

enum class Rules : int {
	//P -> LDE
	P = 1,
	//LDE -> LDE DE
	LDE,
	//LDE -> DE
	LDE_TAIL,
	//DE -> DF
	DE_FUNC,
	//DE -> DT
	DE_TYPE,
	//DT -> type IDD = array [ NT_NUM ] of TP
	DT_ARRAY,
	//DT -> type IDD = struct NB { DC }
	DT_STRUCT,
	//DT -> type IDD = TP
	DT_SIMP,
	//TP -> integer
	TYPE_INT,
	//TP -> char
	TYPE_CHR,
	//TP->boolean
	TYPE_BOL,
	//TP -> string
	TYPE_STR,
	//TP -> IDU
	TYPE_IDU,
	//DC -> DC ; LI : TP
	DC,
	//DC -> LI : TP
	DC_TAIL,
	//DF -> function IDD NB ( LP ) : TP B
	DF,
	//LP -> LP , IDD : TP
	LP,
	//LP -> IDD : TP
	LP_TAIL,
	//B -> { LDV LS }
	B,
	//LDV -> LDV DV
	LDV,
	//LDV -> DV
	LDV_TAIL,
	//LS -> LS S
	LS,
	//LS -> S
	LS_TAIL,
	//DV -> var LI : TP ;
	DV,
	//LI -> LI , IDD
	LI,
	//LI -> IDD
	LI_TAIL,
	//S -> if ( E ) S
	IF,
	//S -> if ( E ) S else S
	IF_ELSE,
	//S -> while ( E ) S
	WHILE,
	//S -> do S while ( E ) ;
	DO_WHILE,
	//S -> B
	S_B,
	//S -> LV = E ;
	S_LV,
	//S -> break ;
	BREAK,
	//S -> continue ;
	CONTINUE,
	//E -> E and L
	AND,
	//E -> E or L
	OR,
	//E -> L
	L,
	//L -> L lt R
	COMP_LT,
	//L -> L gt R
	COMP_GT,
	//L -> L le R
	COMP_LE,
	//L -> L ge R
	COMP_GE,
	//L -> L eq R
	COMP_EQ,
	//L -> L ne R
	COMP_NEQ,
	//L -> R
	R,
	//R -> R + TM
	PLUS,
	//R -> R - TM
	MINUS,
	//R -> TM
	TM,
	//TM -> TM * F
	TIMES,
	//TM -> TM / F
	DIVIDE,
	//TM -> F
	F,
	//F -> LV
	LV,
	//F -> inc LV
	INC_PRE,
	//F -> dec LV
	DEC_PRE,
	//F -> LV inc
	INC_POS,
	//F -> LV dec
	DEC_POS,
	//F -> ( E )
	PARENTH,
	//F -> IDU ( LE )
	IDU_PARENTH,
	//F -> - F
	NEG,
	//F -> not F
	NOT,
	//F -> NT_TRUE
	NT_TRUE,
	//F -> NT_FALSE
	NT_FALSE,
	//F -> NT_CHR
	NT_CHR,
	//F -> NT_STR
	NT_STR,
	//F -> NT_NUM
	NT_NUM,
	//LE -> LE , E
	LE,
	//LE -> E
	LE_TAIL,
	//LV -> LV dot ID
	DOT,
	//LV -> LV [ E ]
	LV_E,
	//LV -> IDU
	IDU,
	//IDD -> id
	IDD_ID,
	//IDU -> id
	IDU_ID,
	//ID -> id
	ID,
	//NT_TRUE -> true
	TRUE,
	//NT_FALSE -> false
	FALSE,
	//NT_CHR -> charval
	CHAR,
	//NT_STR -> stringval
	STRING,
	//NT_NUM -> numeral
	NUMERAL,
	//NB leva a nada!
	NB,
};

enum class Kind {
	NO_KIND_DEF = -1,
	VAR,
	PARAM,
	FUNCTION,
	FIELD,
	ARRAY_TYPE,
	STRUCT_TYPE,
	ALIAS_TYPE,
	SCALAR_TYPE,
	UNIVERSAL,
};

inline bool isKindType(Kind k) {
	return k == Kind::ARRAY_TYPE || k == Kind::STRUCT_TYPE || k == Kind::ALIAS_TYPE || k == Kind::SCALAR_TYPE;
}

enum class ErrorType {
	REDECLATION,
	NOT_DECLARED,
	INVALID_RULE,
	TYPE_EXPECTED,
};

inline unsigned int TokenIdx(Token k) {
	return static_cast<unsigned int>(k);
}

inline unsigned int TokenIdx(NonTerminal k) {
	return static_cast<unsigned int>(k);
}

inline int RuleNo(Rules rule) {
	return -static_cast<int>(rule);
}

inline bool IsTerminal(Token k) { return true; }
inline bool IsTerminal(NonTerminal k) { return false; }
inline bool IsTerminal(unsigned int k) { return k >= (1 << 15); }