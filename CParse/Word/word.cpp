#include "word.h"

// <user definition
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
	if (IdentifierManager::getInstance().getType(identifier) == IdentifierManager::getInstance().Typename) {
		return(CWord::TYPE_NAME);
	}
	return(CWord::IDENTIFIER);
}
// user definition>
// user definition>

int CWord::runRelativeCode(int type) {
	switch( type ) {
		case 0:
			{ count(); return(BREAK); }
			break;
		case 1:
			{ count(); return(CASE); }
			break;
		case 2:
			{ count(); return(CHAR); }
			break;
		case 3:
			{ count(); return(CONST); }
			break;
		case 4:
			{ count(); return(CONTINUE); }
			break;
		case 5:
			{ count(); return(DEFAULT); }
			break;
		case 6:
			{ count(); return(DO); }
			break;
		case 7:
			{ count(); return(DOUBLE); }
			break;
		case 8:
			{ count(); return(ELSE); }
			break;
		case 9:
			{ count(); return(ENUM); }
			break;
		case 10:
			{ count(); return(EXTERN); }
			break;
		case 11:
			{ count(); return(FLOAT); }
			break;
		case 12:
			{ count(); return(FOR); }
			break;
		case 13:
			{ count(); return(GOTO); }
			break;
		case 14:
			{ count(); return(IF); }
			break;
		case 15:
			{ count(); return(INT); }
			break;
		case 16:
			{ count(); return(LONG); }
			break;
		case 17:
			{ count(); return(REGISTER); }
			break;
		case 18:
			{ count(); return(RETURN); }
			break;
		case 19:
			{ count(); return(SHORT); }
			break;
		case 20:
			{ count(); return(SIGNED); }
			break;
		case 21:
			{ count(); return(SIZEOF); }
			break;
		case 22:
			{ count(); return(STATIC); }
			break;
		case 23:
			{ count(); return(STRUCT); }
			break;
		case 24:
			{ count(); return(SWITCH); }
			break;
		case 25:
			{ count(); return(TYPEDEF); }
			break;
		case 26:
			{ count(); return(UNION); }
			break;
		case 27:
			{ count(); return(UNSIGNED); }
			break;
		case 28:
			{ count(); return(VOID); }
			break;
		case 29:
			{ count(); return(VOLATILE); }
			break;
		case 30:
			{ count(); return(WHILE); }
			break;
		case 31:
			{ count(); return(check_type(getCurWord())); }
			break;
		case 32:
			{ count(); return(CONSTANT); }
			break;
		case 33:
			{ count(); return(CONSTANT); }
			break;
		case 34:
			{ count(); return(CONSTANT); }
			break;
		case 35:
			{ count(); return(CONSTANT); }
			break;
		case 36:
			{ count(); return(CONSTANT); }
			break;
		case 37:
			{ count(); return(CONSTANT); }
			break;
		case 38:
			{ count(); return(CONSTANT); }
			break;
		case 39:
			{ count(); return(STRING_LITERAL); }
			break;
		case 40:
			{ count(); return(ELLIPSIS); }
			break;
		case 41:
			{ count(); return(RIGHT_ASSIGN); }
			break;
		case 42:
			{ count(); return(LEFT_ASSIGN); }
			break;
		case 43:
			{ count(); return(ADD_ASSIGN); }
			break;
		case 44:
			{ count(); return(SUB_ASSIGN); }
			break;
		case 45:
			{ count(); return(MUL_ASSIGN); }
			break;
		case 46:
			{ count(); return(DIV_ASSIGN); }
			break;
		case 47:
			{ count(); return(MOD_ASSIGN); }
			break;
		case 48:
			{ count(); return(AND_ASSIGN); }
			break;
		case 49:
			{ count(); return(XOR_ASSIGN); }
			break;
		case 50:
			{ count(); return(OR_ASSIGN); }
			break;
		case 51:
			{ count(); return(RIGHT_OP); }
			break;
		case 52:
			{ count(); return(LEFT_OP); }
			break;
		case 53:
			{ count(); return(INC_OP); }
			break;
		case 54:
			{ count(); return(DEC_OP); }
			break;
		case 55:
			{ count(); return(PTR_OP); }
			break;
		case 56:
			{ count(); return(AND_OP); }
			break;
		case 57:
			{ count(); return(OR_OP); }
			break;
		case 58:
			{ count(); return(LE_OP); }
			break;
		case 59:
			{ count(); return(GE_OP); }
			break;
		case 60:
			{ count(); return(EQ_OP); }
			break;
		case 61:
			{ count(); return(NE_OP); }
			break;
		case 62:
			{ count(); return(';'); }
			break;
		case 63:
			{ count(); return('{'); }
			break;
		case 64:
			{ count(); return('}'); }
			break;
		case 65:
			{ count(); return(','); }
			break;
		case 66:
			{ count(); return(':'); }
			break;
		case 67:
			{ count(); return('='); }
			break;
		case 68:
			{ count(); return('('); }
			break;
		case 69:
			{ count(); return(')'); }
			break;
		case 70:
			{ count(); return('['); }
			break;
		case 71:
			{ count(); return(']'); }
			break;
		case 72:
			{ count(); return('.'); }
			break;
		case 73:
			{ count(); return('&'); }
			break;
		case 74:
			{ count(); return('!'); }
			break;
		case 75:
			{ count(); return('~'); }
			break;
		case 76:
			{ count(); return('-'); }
			break;
		case 77:
			{ count(); return('+'); }
			break;
		case 78:
			{ count(); return('*'); }
			break;
		case 79:
			{ count(); return('/'); }
			break;
		case 80:
			{ count(); return('%'); }
			break;
		case 81:
			{ count(); return('<'); }
			break;
		case 82:
			{ count(); return('>'); }
			break;
		case 83:
			{ count(); return('^'); }
			break;
		case 84:
			{ count(); return('|'); }
			break;
		case 85:
			{ count(); return('?'); }
			break;
		case 86:
			{ count(); /*return(Comment);*/ break; }
			break;
		case 87:
			{ count(); /*return(Comment);*/ break; }
			break;
		case 88:
			{ count(); break; }
			break;
		case 89:
			{ count(); return(INCLUDE); }
			break;
	}
	return -1;
}
