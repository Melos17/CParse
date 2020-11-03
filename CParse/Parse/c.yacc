
output:"F:/Programme/YaccTest/YaccTest/Parse/parse"
version:"1.1.1"
parsename:"CParse"
wordname:"CWord"
wordheader:"..\Word\word.h"
start:"translation_unit"


{Grammar

/* base */

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;

external_declaration
	: function_definition
	| declaration
	;

/* define */

function_definition { : generateFunctionDefinition}
	: function_definition_head compound_statement
	;

function_definition_head { : generateFunctionDefinition}
	: declaration_specifiers declarator
	;

declaration_list {DeclarationList : generateDeclarationList}
	: declaration
	| declaration_list declaration
	;

declaration {Declaration : generateDeclaration}
	: declaration_specifiers ';'
	| declaration_init_declarator_list ';'
	;

declaration_init_declarator_list {Declaration : generateDeclaration}
	: declaration_specifiers init_declarator
	| declaration_init_declarator_list ',' init_declarator
	;


init_declarator {InitDeclarator : generateInitDeclarator}
	: declarator
	| declarator '=' initializer
	;

initializer {Command : generateInitializer}
	: assignment_expression
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list {InitializerList : generateInitializerList}
	: initializer
	| initializer_list ',' initializer
	;


parameter_type_list {ParameterTypeList : generateParameterTypeList}
	: parameter_list
	| parameter_list ',' ELLIPSIS
	;

parameter_list {ParameterList : generateParameterList}
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration {ParameterDeclaration : generateParameterDeclaration}
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;


enum_specifier {Type : generateEnumSpecifier}
	: ENUM '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER
	;

enumerator_list {EnumeratorList : generateEnumeratorList}
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator {Enumerator : generateEnumerator}
	: IDENTIFIER
	| IDENTIFIER '=' constant_expression
	;


struct_or_union_specifier {StructOrUnionSpecifier : generateStructOrUnionSpecifier}
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	;

struct_or_union {TerminalUnit}
	: STRUCT
	| UNION
	;

struct_declaration_list {StructDeclarationList : generateStructDeclarationList}
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration {StructDeclaration : generateStructDeclaration}
	: specifier_qualifier_list struct_declarator_list ';'
	;

struct_declarator_list {StructDeclaratorList : generateStructDeclaratorList}
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator {StructDeclarator : generateStructDeclarator}
	: declarator
	| ':' constant_expression
	| declarator ':' constant_expression
	;


/* type */

declaration_specifiers {DeclarationSpecifiers : generateDeclarationSpecifiers}
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier
	| type_specifier declaration_specifiers
	| type_qualifier
	| type_qualifier declaration_specifiers
	;

storage_class_specifier {StorageClassSpecifier : generateStorageClassSpecifier}
	: TYPEDEF
	| EXTERN
	| STATIC
	| REGISTER
	;

type_specifier {Type : generateTypeSpecifier}
	: VOID
	| CHAR
	| SHORT
	| INT
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME
	;

type_qualifier_list {Type}
	: type_qualifier
	| type_qualifier_list type_qualifier {generateTypeQualifier}
	;

type_qualifier {Type : generateTypeQualifier}
	: CONST
	| VOLATILE
	;


declarator {Declarator : generateDeclarator}
	: pointer direct_declarator
	| direct_declarator
	;

pointer {Type : generatePointer}
	: '*'
	| '*' type_qualifier_list
	| '*' pointer
	| '*' type_qualifier_list pointer
	;

direct_declarator {Declarator : generateDirectDeclarator}
	: IDENTIFIER
	| '(' declarator ')'
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' ')'
	;


abstract_declarator {Type}
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator {generateAbstractDeclarator}
	;

direct_abstract_declarator {Type : generateDirectAbstractDeclarator}
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;


specifier_qualifier_list {Type}
	: type_specifier specifier_qualifier_list {generateSpecifierQualifierList}
	| type_specifier
	| type_qualifier specifier_qualifier_list {generateSpecifierQualifierList}
	| type_qualifier
	;

type_name {TypeName : generateTypeName}
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

/* statement */

statement_list {Command}
	: statement
	| statement_list statement {generateStatementList}
	;

statement {Command}
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement {Command : generateLabeledStatement}
	: CASE constant_expression ':' statement
	| DEFAULT ':' statement
	| IDENTIFIER ':' statement
	;

compound_statement {Command : generateCompoundStatement}
	: left_brace right_brace
	| left_brace statement_list right_brace
	| left_brace declaration_list right_brace
	| left_brace declaration_list statement_list right_brace
	;

left_brace {TerminalUnit : advanceLevel}
	: '{'
	;

right_brace {TerminalUnit : retreatLevel}
	: '}'
	;

expression_statement {Command : generateExpressionStatement}
	: ';'
	| expression ';'
	;

selection_statement {Command : generateSelectionStatement}
	: IF '(' expression ')' statement
	| IF '(' expression ')' statement ELSE statement
	| SWITCH '(' expression ')' statement
	;

iteration_statement {Command : generateIterationStatement}
	: WHILE '(' expression ')' statement
	| DO statement WHILE '(' expression ')' ';'
	| FOR '(' expression_statement expression_statement ')' statement
	| FOR '(' expression_statement expression_statement expression ')' statement
	;

jump_statement {Command : generateJumpStatement}
	: GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN expression ';'
	;


/* expression */

assignment_operator {TerminalUnit}
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;


expression {Command}
	: assignment_expression
	| expression ',' assignment_expression {generateExpression}
	;

assignment_expression {Command}
	: conditional_expression
	| unary_expression assignment_operator assignment_expression {generateExpression}
	;

constant_expression {Command : generateConstantExpression}
	: CONSTANT
	/* | conditional_expression -- do not support constant expression now */
	;

conditional_expression {Command}
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression {generateConditionalExpression}
	;

logical_or_expression {Command}
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression {generateExpression}
	;

logical_and_expression {Command}
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression {generateExpression}
	;

inclusive_or_expression {Command}
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression {generateExpression}
	;

exclusive_or_expression {Command}
	: and_expression
	| exclusive_or_expression '^' and_expression {generateExpression}
	;

and_expression {Command}
	: equality_expression
	| and_expression '&' equality_expression {generateExpression}
	;

equality_expression {Command}
	: relational_expression
	| equality_expression EQ_OP relational_expression {generateExpression}
	| equality_expression NE_OP relational_expression {generateExpression}
	;

relational_expression {Command}
	: shift_expression
	| relational_expression '<' shift_expression {generateExpression}
	| relational_expression '>' shift_expression {generateExpression}
	| relational_expression LE_OP shift_expression {generateExpression}
	| relational_expression GE_OP shift_expression {generateExpression}
	;

shift_expression {Command}
	: additive_expression
	| shift_expression LEFT_OP additive_expression {generateExpression}
	| shift_expression RIGHT_OP additive_expression {generateExpression}
	;

additive_expression {Command}
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression {generateExpression}
	| additive_expression '-' multiplicative_expression {generateExpression}
	;

multiplicative_expression {Command}
	: cast_expression
	| multiplicative_expression '*' cast_expression {generateExpression}
	| multiplicative_expression '/' cast_expression {generateExpression}
	| multiplicative_expression '%' cast_expression {generateExpression}
	;

cast_expression {Command}
	: unary_expression
	| '(' type_name ')' cast_expression {generateCastExpression}
	;

unary_operator {TerminalUnit}
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

unary_expression {Command : generateUnaryExpression}
	: postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| SIZEOF '(' type_name ')'
	| SIZEOF unary_expression
	;

postfix_expression {PostfixExpression : generatePostfixExpression}
	: primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	;

argument_expression_list {ArgumentExpressionList : generateArgumentExpressionList}
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

primary_expression {PrimaryExpression : generatePrimaryExpression}
	: IDENTIFIER
	| CONSTANT
	| string_literal
	| '(' expression ')'
	;

string_literal {TerminalUnit}
	: STRING_LITERAL
	| string_literal STRING_LITERAL {generateStringLiteral}
	;

Grammar}


{Code-H-Outside
// <user definition
class Command;
#include "..\Word\word.h"
// user definition>
Code-H-Outside}


{Code-H-Public
// <user definition
	void init();
	shared_ptr<Command> fetchCommand();
// user definition>
Code-H-Public}


{Code-CPP
// <user definition
#include "..\reduce.h"
#include "..\reduce_define.h"
#include "..\reduce_expression.h"
#include "..\reduce_statement.h"
#include "..\reduce_type.h"
#include "..\base.h"
#include "..\function.h"
#include "..\data.h"
#include "..\identifier.h"
#include "..\var.h"

void CParse::init() {
	gCode = make_shared<Command>();
	FunctionManager::getInstance().init();
	DataManager::getInstance().init();
	IdentifierManager::getInstance().init();
	VariableManager::getInstance().init();
	TypesManager::getInstance().init();
}

shared_ptr<Command> CParse::fetchCommand() {
	return gCode;
}
// user definition>
Code-CPP}
