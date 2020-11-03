
output:"F:/Programme/YaccTest/YaccTest/Word/word"
classname:"CWord"
version:"1.1.1"

{Constant
D			"[0-9]"
L			"[a-zA-Z_]"
H			"[a-fA-F0-9]"
E			"[Ee]""[+\-]"?{D}+
FS			"(f|F|l|L)"
IS			"(u|U)?(l|L)?(l|L)?"
Constant}

{Target
"break"							{ count(); return(BREAK); }
"case"							{ count(); return(CASE); }
"char"							{ count(); return(CHAR); }
"const"							{ count(); return(CONST); }
"continue"						{ count(); return(CONTINUE); }
"default"						{ count(); return(DEFAULT); }
"do"							{ count(); return(DO); }
"double"						{ count(); return(DOUBLE); }
"else"							{ count(); return(ELSE); }
"enum"							{ count(); return(ENUM); }
"extern"						{ count(); return(EXTERN); }
"float"							{ count(); return(FLOAT); }
"for"							{ count(); return(FOR); }
"goto"							{ count(); return(GOTO); }
"if"							{ count(); return(IF); }
"int"							{ count(); return(INT); }
"long"							{ count(); return(LONG); }
"register"						{ count(); return(REGISTER); }
"return"						{ count(); return(RETURN); }
"short"							{ count(); return(SHORT); }
"signed"						{ count(); return(SIGNED); }
"sizeof"						{ count(); return(SIZEOF); }
"static"						{ count(); return(STATIC); }
"struct"						{ count(); return(STRUCT); }
"switch"						{ count(); return(SWITCH); }
"typedef"						{ count(); return(TYPEDEF); }
"union"							{ count(); return(UNION); }
"unsigned"						{ count(); return(UNSIGNED); }
"void"							{ count(); return(VOID); }
"volatile"						{ count(); return(VOLATILE); }
"while"							{ count(); return(WHILE); }

{L}({L}|{D})*					{ count(); return(check_type(getCurWord())); }
"0[xX]"{H}+{IS}					{ count(); return(CONSTANT); }
"0"{D}+{IS}						{ count(); return(CONSTANT); }
{D}+{IS}						{ count(); return(CONSTANT); }
"L"?"'([^']+|\\\')'"			{ count(); return(CONSTANT); }

{D}+{E}{FS}?					{ count(); return(CONSTANT); }
{D}*"\."{D}+{E}?{FS}?			{ count(); return(CONSTANT); }
{D}+"\."{D}*{E}?{FS}?			{ count(); return(CONSTANT); }
"L"?"\"([^\"]|(\\\"))*\""		{ count(); return(STRING_LITERAL); }

"\.\.\."						{ count(); return(ELLIPSIS); }
">>="							{ count(); return(RIGHT_ASSIGN); }
"<<="							{ count(); return(LEFT_ASSIGN); }
"\+="							{ count(); return(ADD_ASSIGN); }
"-="							{ count(); return(SUB_ASSIGN); }
"\*="							{ count(); return(MUL_ASSIGN); }
"/="							{ count(); return(DIV_ASSIGN); }
"%="							{ count(); return(MOD_ASSIGN); }
"&="							{ count(); return(AND_ASSIGN); }
"^="							{ count(); return(XOR_ASSIGN); }
"\|="							{ count(); return(OR_ASSIGN); }
">>"							{ count(); return(RIGHT_OP); }
"<<"							{ count(); return(LEFT_OP); }
"\+\+"							{ count(); return(INC_OP); }
"--"							{ count(); return(DEC_OP); }
"->"							{ count(); return(PTR_OP); }
"&&"							{ count(); return(AND_OP); }
"\|\|"							{ count(); return(OR_OP); }
"<="							{ count(); return(LE_OP); }
">="							{ count(); return(GE_OP); }
"=="							{ count(); return(EQ_OP); }
"!="							{ count(); return(NE_OP); }
";"								{ count(); return(';'); }
"\{"							{ count(); return('{'); }
"\}"							{ count(); return('}'); }
","								{ count(); return(','); }
":"								{ count(); return(':'); }
"="								{ count(); return('='); }
"\("							{ count(); return('('); }
"\)"							{ count(); return(')'); }
"\["							{ count(); return('['); }
"\]"							{ count(); return(']'); }
"\."							{ count(); return('.'); }
"&"								{ count(); return('&'); }
"!"								{ count(); return('!'); }
"~"								{ count(); return('~'); }
"-"								{ count(); return('-'); }
"\+"							{ count(); return('+'); }
"\*"							{ count(); return('*'); }
"/"								{ count(); return('/'); }
"%"								{ count(); return('%'); }
"<"								{ count(); return('<'); }
">"								{ count(); return('>'); }
"^"								{ count(); return('^'); }
"\|"							{ count(); return('|'); }
"\?"							{ count(); return('?'); }

"//[^\n]*\n"					{ count(); /*return(Comment);*/ break; }
"/\*/?([^/]|[^\*]/)*\*+/"       { count(); /*return(Comment);*/ break; }
"[ \t\v\n\f]"					{ count(); break; }
"#include"						{ count(); return(INCLUDE); }
Target}

{Code-H-Public
enum TOKEN_TYPE {
	IDENTIFIER = 128, TYPE_NAME, BREAK, CASE, CHAR, CONST, CONTINUE, DEFAULT, DO, DOUBLE, ELSE, ENUM, EXTERN, FLOAT, FOR, GOTO, IF, INT, LONG, REGISTER, RETURN, SHORT, SIGNED, SIZEOF, STATIC, STRUCT,
	SWITCH, TYPEDEF, UNION, UNSIGNED, VOID, VOLATILE, WHILE, CONSTANT, STRING_LITERAL, ELLIPSIS,
	RIGHT_ASSIGN, LEFT_ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, AND_ASSIGN, XOR_ASSIGN, OR_ASSIGN,
	RIGHT_OP, LEFT_OP, INC_OP, DEC_OP, PTR_OP, AND_OP, OR_OP, LE_OP, GE_OP, EQ_OP, NE_OP,
	INCLUDE
};
Code-H-Public}

{Code-H-Private
	void count();
	void comment();
	int check_type(const std::string& identifier);
Code-H-Private}

{Code-CPP
// <user definition
#include "../type.h"
#include "../identifier.h"

void CWord::count() {
	
}

void CWord::comment()
{
	char c = getNextChar();
	while (c) {
		while (c == '*') {
			c = getNextChar();
			if (c == '/') return;
		}
		c = getNextChar();
	}
}

int CWord::check_type(const std::string& identifier)
{
	if (IdentifierManager::getInstance().getTypeThisLevel(identifier) == IdentifierManager::getInstance().Typename) {
		return(CWord::TYPE_NAME);
	}
	return(CWord::IDENTIFIER);
}
// user definition>
Code-CPP}
