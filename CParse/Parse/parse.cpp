#include "parse.h"

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

void CParse::reduce(size_t reduceId) {
	switch(reduceId) {
	case 5: {
		// function_definition -> function_definition_head compound_statement 
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		generateFunctionDefinition(unit2);
		break;
	}
	case 6: {
		// declaration -> declaration_specifiers ; 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<DeclarationSpecifiers> unit1 = dynamic_pointer_cast<DeclarationSpecifiers>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Declaration> parent = generateDeclaration(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 7: {
		// declaration -> declaration_init_declarator_list ; 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Declaration> unit1 = dynamic_pointer_cast<Declaration>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Declaration> parent = generateDeclaration(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 8: {
		// function_definition_head -> declaration_specifiers declarator 
		shared_ptr<Declarator> unit2 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<DeclarationSpecifiers> unit1 = dynamic_pointer_cast<DeclarationSpecifiers>(mUnitStack.top());
		mUnitStack.pop();
		
		generateFunctionDefinition(unit1, unit2);
		break;
	}
	case 9: {
		// compound_statement -> left_brace right_brace 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateCompoundStatement(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 10: {
		// compound_statement -> left_brace statement_list right_brace 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateCompoundStatement(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 11: {
		// compound_statement -> left_brace declaration_list right_brace 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<DeclarationList> unit2 = dynamic_pointer_cast<DeclarationList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateCompoundStatement(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 12: {
		// compound_statement -> left_brace declaration_list statement_list right_brace 
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<DeclarationList> unit2 = dynamic_pointer_cast<DeclarationList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateCompoundStatement(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 13: {
		// declaration_specifiers -> storage_class_specifier 
		shared_ptr<StorageClassSpecifier> unit1 = dynamic_pointer_cast<StorageClassSpecifier>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<DeclarationSpecifiers> parent = generateDeclarationSpecifiers(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 14: {
		// declaration_specifiers -> storage_class_specifier declaration_specifiers 
		shared_ptr<DeclarationSpecifiers> unit2 = dynamic_pointer_cast<DeclarationSpecifiers>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<StorageClassSpecifier> unit1 = dynamic_pointer_cast<StorageClassSpecifier>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<DeclarationSpecifiers> parent = generateDeclarationSpecifiers(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 15: {
		// declaration_specifiers -> type_specifier 
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<DeclarationSpecifiers> parent = generateDeclarationSpecifiers(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 16: {
		// declaration_specifiers -> type_specifier declaration_specifiers 
		shared_ptr<DeclarationSpecifiers> unit2 = dynamic_pointer_cast<DeclarationSpecifiers>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<DeclarationSpecifiers> parent = generateDeclarationSpecifiers(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 17: {
		// declaration_specifiers -> type_qualifier 
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<DeclarationSpecifiers> parent = generateDeclarationSpecifiers(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 18: {
		// declaration_specifiers -> type_qualifier declaration_specifiers 
		shared_ptr<DeclarationSpecifiers> unit2 = dynamic_pointer_cast<DeclarationSpecifiers>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<DeclarationSpecifiers> parent = generateDeclarationSpecifiers(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 19: {
		// declarator -> pointer direct_declarator 
		shared_ptr<Declarator> unit2 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Declarator> parent = generateDeclarator(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 20: {
		// declarator -> direct_declarator 
		shared_ptr<Declarator> unit1 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Declarator> parent = generateDeclarator(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 21: {
		// declaration_list -> declaration 
		shared_ptr<Declaration> unit1 = dynamic_pointer_cast<Declaration>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<DeclarationList> parent = generateDeclarationList(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 22: {
		// declaration_list -> declaration_list declaration 
		shared_ptr<Declaration> unit2 = dynamic_pointer_cast<Declaration>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<DeclarationList> unit1 = dynamic_pointer_cast<DeclarationList>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<DeclarationList> parent = generateDeclarationList(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 23: {
		// declaration_init_declarator_list -> declaration_specifiers init_declarator 
		shared_ptr<InitDeclarator> unit2 = dynamic_pointer_cast<InitDeclarator>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<DeclarationSpecifiers> unit1 = dynamic_pointer_cast<DeclarationSpecifiers>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Declaration> parent = generateDeclaration(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 24: {
		// declaration_init_declarator_list -> declaration_init_declarator_list , init_declarator 
		shared_ptr<InitDeclarator> unit3 = dynamic_pointer_cast<InitDeclarator>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Declaration> unit1 = dynamic_pointer_cast<Declaration>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Declaration> parent = generateDeclaration(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 25: {
		// init_declarator -> declarator 
		shared_ptr<Declarator> unit1 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<InitDeclarator> parent = generateInitDeclarator(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 26: {
		// init_declarator -> declarator = initializer 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Declarator> unit1 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<InitDeclarator> parent = generateInitDeclarator(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 27: {
		// initializer -> assignment_expression 
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateInitializer(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 28: {
		// initializer -> { initializer_list } 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<InitializerList> unit2 = dynamic_pointer_cast<InitializerList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateInitializer(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 29: {
		// initializer -> { initializer_list , } 
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<InitializerList> unit2 = dynamic_pointer_cast<InitializerList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateInitializer(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 31: {
		// assignment_expression -> unary_expression assignment_operator assignment_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 32: {
		// initializer_list -> initializer 
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<InitializerList> parent = generateInitializerList(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 33: {
		// initializer_list -> initializer_list , initializer 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<InitializerList> unit1 = dynamic_pointer_cast<InitializerList>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<InitializerList> parent = generateInitializerList(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 34: {
		// parameter_type_list -> parameter_list 
		shared_ptr<ParameterList> unit1 = dynamic_pointer_cast<ParameterList>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<ParameterTypeList> parent = generateParameterTypeList(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 35: {
		// parameter_type_list -> parameter_list , ELLIPSIS 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<ParameterList> unit1 = dynamic_pointer_cast<ParameterList>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<ParameterTypeList> parent = generateParameterTypeList(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 36: {
		// parameter_list -> parameter_declaration 
		shared_ptr<ParameterDeclaration> unit1 = dynamic_pointer_cast<ParameterDeclaration>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<ParameterList> parent = generateParameterList(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 37: {
		// parameter_list -> parameter_list , parameter_declaration 
		shared_ptr<ParameterDeclaration> unit3 = dynamic_pointer_cast<ParameterDeclaration>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<ParameterList> unit1 = dynamic_pointer_cast<ParameterList>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<ParameterList> parent = generateParameterList(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 38: {
		// parameter_declaration -> declaration_specifiers declarator 
		shared_ptr<Declarator> unit2 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<DeclarationSpecifiers> unit1 = dynamic_pointer_cast<DeclarationSpecifiers>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<ParameterDeclaration> parent = generateParameterDeclaration(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 39: {
		// parameter_declaration -> declaration_specifiers abstract_declarator 
		shared_ptr<Type> unit2 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<DeclarationSpecifiers> unit1 = dynamic_pointer_cast<DeclarationSpecifiers>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<ParameterDeclaration> parent = generateParameterDeclaration(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 40: {
		// parameter_declaration -> declaration_specifiers 
		shared_ptr<DeclarationSpecifiers> unit1 = dynamic_pointer_cast<DeclarationSpecifiers>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<ParameterDeclaration> parent = generateParameterDeclaration(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 43: {
		// abstract_declarator -> pointer direct_abstract_declarator 
		shared_ptr<Type> unit2 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateAbstractDeclarator(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 44: {
		// enum_specifier -> ENUM { enumerator_list } 
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<EnumeratorList> unit3 = dynamic_pointer_cast<EnumeratorList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateEnumSpecifier(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 45: {
		// enum_specifier -> ENUM IDENTIFIER { enumerator_list } 
		shared_ptr<TerminalUnit> unit5 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<EnumeratorList> unit4 = dynamic_pointer_cast<EnumeratorList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateEnumSpecifier(unit1, unit2, unit3, unit4, unit5);
		mUnitStack.push(parent);
		break;
	}
	case 46: {
		// enum_specifier -> ENUM IDENTIFIER 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateEnumSpecifier(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 47: {
		// enumerator_list -> enumerator 
		shared_ptr<Enumerator> unit1 = dynamic_pointer_cast<Enumerator>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<EnumeratorList> parent = generateEnumeratorList(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 48: {
		// enumerator_list -> enumerator_list , enumerator 
		shared_ptr<Enumerator> unit3 = dynamic_pointer_cast<Enumerator>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<EnumeratorList> unit1 = dynamic_pointer_cast<EnumeratorList>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<EnumeratorList> parent = generateEnumeratorList(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 49: {
		// enumerator -> IDENTIFIER 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Enumerator> parent = generateEnumerator(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 50: {
		// enumerator -> IDENTIFIER = constant_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Enumerator> parent = generateEnumerator(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 51: {
		// constant_expression -> CONSTANT 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateConstantExpression(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 52: {
		// struct_or_union_specifier -> struct_or_union IDENTIFIER { struct_declaration_list } 
		shared_ptr<TerminalUnit> unit5 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<StructDeclarationList> unit4 = dynamic_pointer_cast<StructDeclarationList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StructOrUnionSpecifier> parent = generateStructOrUnionSpecifier(unit1, unit2, unit3, unit4, unit5);
		mUnitStack.push(parent);
		break;
	}
	case 53: {
		// struct_or_union_specifier -> struct_or_union { struct_declaration_list } 
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<StructDeclarationList> unit3 = dynamic_pointer_cast<StructDeclarationList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StructOrUnionSpecifier> parent = generateStructOrUnionSpecifier(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 54: {
		// struct_or_union_specifier -> struct_or_union IDENTIFIER 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StructOrUnionSpecifier> parent = generateStructOrUnionSpecifier(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 57: {
		// struct_declaration_list -> struct_declaration 
		shared_ptr<StructDeclaration> unit1 = dynamic_pointer_cast<StructDeclaration>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StructDeclarationList> parent = generateStructDeclarationList(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 58: {
		// struct_declaration_list -> struct_declaration_list struct_declaration 
		shared_ptr<StructDeclaration> unit2 = dynamic_pointer_cast<StructDeclaration>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<StructDeclarationList> unit1 = dynamic_pointer_cast<StructDeclarationList>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StructDeclarationList> parent = generateStructDeclarationList(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 59: {
		// struct_declaration -> specifier_qualifier_list struct_declarator_list ; 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<StructDeclaratorList> unit2 = dynamic_pointer_cast<StructDeclaratorList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StructDeclaration> parent = generateStructDeclaration(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 60: {
		// specifier_qualifier_list -> type_specifier specifier_qualifier_list 
		shared_ptr<Type> unit2 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateSpecifierQualifierList(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 62: {
		// specifier_qualifier_list -> type_qualifier specifier_qualifier_list 
		shared_ptr<Type> unit2 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateSpecifierQualifierList(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 64: {
		// struct_declarator_list -> struct_declarator 
		shared_ptr<StructDeclarator> unit1 = dynamic_pointer_cast<StructDeclarator>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StructDeclaratorList> parent = generateStructDeclaratorList(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 65: {
		// struct_declarator_list -> struct_declarator_list , struct_declarator 
		shared_ptr<StructDeclarator> unit3 = dynamic_pointer_cast<StructDeclarator>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<StructDeclaratorList> unit1 = dynamic_pointer_cast<StructDeclaratorList>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StructDeclaratorList> parent = generateStructDeclaratorList(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 66: {
		// struct_declarator -> declarator 
		shared_ptr<Declarator> unit1 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StructDeclarator> parent = generateStructDeclarator(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 67: {
		// struct_declarator -> : constant_expression 
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StructDeclarator> parent = generateStructDeclarator(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 68: {
		// struct_declarator -> declarator : constant_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Declarator> unit1 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StructDeclarator> parent = generateStructDeclarator(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 69: {
		// storage_class_specifier -> TYPEDEF 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StorageClassSpecifier> parent = generateStorageClassSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 70: {
		// storage_class_specifier -> EXTERN 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StorageClassSpecifier> parent = generateStorageClassSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 71: {
		// storage_class_specifier -> STATIC 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StorageClassSpecifier> parent = generateStorageClassSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 72: {
		// storage_class_specifier -> REGISTER 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<StorageClassSpecifier> parent = generateStorageClassSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 73: {
		// type_specifier -> VOID 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 74: {
		// type_specifier -> CHAR 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 75: {
		// type_specifier -> SHORT 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 76: {
		// type_specifier -> INT 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 77: {
		// type_specifier -> LONG 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 78: {
		// type_specifier -> FLOAT 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 79: {
		// type_specifier -> DOUBLE 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 80: {
		// type_specifier -> SIGNED 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 81: {
		// type_specifier -> UNSIGNED 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 82: {
		// type_specifier -> struct_or_union_specifier 
		shared_ptr<StructOrUnionSpecifier> unit1 = dynamic_pointer_cast<StructOrUnionSpecifier>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 83: {
		// type_specifier -> enum_specifier 
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 84: {
		// type_specifier -> TYPE_NAME 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeSpecifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 85: {
		// type_qualifier -> CONST 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeQualifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 86: {
		// type_qualifier -> VOLATILE 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeQualifier(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 88: {
		// type_qualifier_list -> type_qualifier_list type_qualifier 
		shared_ptr<Type> unit2 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateTypeQualifier(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 89: {
		// pointer -> * 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generatePointer(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 90: {
		// pointer -> * type_qualifier_list 
		shared_ptr<Type> unit2 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generatePointer(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 91: {
		// pointer -> * pointer 
		shared_ptr<Type> unit2 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generatePointer(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 92: {
		// pointer -> * type_qualifier_list pointer 
		shared_ptr<Type> unit3 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit2 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generatePointer(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 93: {
		// direct_declarator -> IDENTIFIER 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Declarator> parent = generateDirectDeclarator(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 94: {
		// direct_declarator -> ( declarator ) 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Declarator> unit2 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Declarator> parent = generateDirectDeclarator(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 95: {
		// direct_declarator -> direct_declarator [ constant_expression ] 
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Declarator> unit1 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Declarator> parent = generateDirectDeclarator(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 96: {
		// direct_declarator -> direct_declarator [ ] 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Declarator> unit1 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Declarator> parent = generateDirectDeclarator(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 97: {
		// direct_declarator -> direct_declarator ( parameter_type_list ) 
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<ParameterTypeList> unit3 = dynamic_pointer_cast<ParameterTypeList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Declarator> unit1 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Declarator> parent = generateDirectDeclarator(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 98: {
		// direct_declarator -> direct_declarator ( ) 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Declarator> unit1 = dynamic_pointer_cast<Declarator>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Declarator> parent = generateDirectDeclarator(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 99: {
		// direct_abstract_declarator -> ( abstract_declarator ) 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit2 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateDirectAbstractDeclarator(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 100: {
		// direct_abstract_declarator -> [ ] 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateDirectAbstractDeclarator(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 101: {
		// direct_abstract_declarator -> [ constant_expression ] 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateDirectAbstractDeclarator(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 102: {
		// direct_abstract_declarator -> direct_abstract_declarator [ ] 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateDirectAbstractDeclarator(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 103: {
		// direct_abstract_declarator -> direct_abstract_declarator [ constant_expression ] 
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateDirectAbstractDeclarator(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 104: {
		// direct_abstract_declarator -> ( ) 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateDirectAbstractDeclarator(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 105: {
		// direct_abstract_declarator -> ( parameter_type_list ) 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<ParameterTypeList> unit2 = dynamic_pointer_cast<ParameterTypeList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateDirectAbstractDeclarator(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 106: {
		// direct_abstract_declarator -> direct_abstract_declarator ( ) 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateDirectAbstractDeclarator(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 107: {
		// direct_abstract_declarator -> direct_abstract_declarator ( parameter_type_list ) 
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<ParameterTypeList> unit3 = dynamic_pointer_cast<ParameterTypeList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Type> parent = generateDirectAbstractDeclarator(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 108: {
		// type_name -> specifier_qualifier_list 
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<TypeName> parent = generateTypeName(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 109: {
		// type_name -> specifier_qualifier_list abstract_declarator 
		shared_ptr<Type> unit2 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Type> unit1 = dynamic_pointer_cast<Type>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<TypeName> parent = generateTypeName(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 111: {
		// statement_list -> statement_list statement 
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateStatementList(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 118: {
		// labeled_statement -> CASE constant_expression : statement 
		shared_ptr<Command> unit4 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateLabeledStatement(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 119: {
		// labeled_statement -> DEFAULT : statement 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateLabeledStatement(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 120: {
		// labeled_statement -> IDENTIFIER : statement 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateLabeledStatement(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 121: {
		// expression_statement -> ; 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpressionStatement(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 122: {
		// expression_statement -> expression ; 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpressionStatement(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 123: {
		// selection_statement -> IF ( expression ) statement 
		shared_ptr<Command> unit5 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateSelectionStatement(unit1, unit2, unit3, unit4, unit5);
		mUnitStack.push(parent);
		break;
	}
	case 124: {
		// selection_statement -> IF ( expression ) statement ELSE statement 
		shared_ptr<Command> unit7 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit6 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit5 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateSelectionStatement(unit1, unit2, unit3, unit4, unit5, unit6, unit7);
		mUnitStack.push(parent);
		break;
	}
	case 125: {
		// selection_statement -> SWITCH ( expression ) statement 
		shared_ptr<Command> unit5 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateSelectionStatement(unit1, unit2, unit3, unit4, unit5);
		mUnitStack.push(parent);
		break;
	}
	case 126: {
		// iteration_statement -> WHILE ( expression ) statement 
		shared_ptr<Command> unit5 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateIterationStatement(unit1, unit2, unit3, unit4, unit5);
		mUnitStack.push(parent);
		break;
	}
	case 127: {
		// iteration_statement -> DO statement WHILE ( expression ) ; 
		shared_ptr<TerminalUnit> unit7 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit6 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit5 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateIterationStatement(unit1, unit2, unit3, unit4, unit5, unit6, unit7);
		mUnitStack.push(parent);
		break;
	}
	case 128: {
		// iteration_statement -> FOR ( expression_statement expression_statement ) statement 
		shared_ptr<Command> unit6 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit5 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit4 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateIterationStatement(unit1, unit2, unit3, unit4, unit5, unit6);
		mUnitStack.push(parent);
		break;
	}
	case 129: {
		// iteration_statement -> FOR ( expression_statement expression_statement expression ) statement 
		shared_ptr<Command> unit7 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit6 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit5 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit4 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateIterationStatement(unit1, unit2, unit3, unit4, unit5, unit6, unit7);
		mUnitStack.push(parent);
		break;
	}
	case 130: {
		// jump_statement -> GOTO IDENTIFIER ; 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateJumpStatement(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 131: {
		// jump_statement -> CONTINUE ; 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateJumpStatement(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 132: {
		// jump_statement -> BREAK ; 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateJumpStatement(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 133: {
		// jump_statement -> RETURN ; 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateJumpStatement(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 134: {
		// jump_statement -> RETURN expression ; 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateJumpStatement(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 135: {
		// left_brace -> { 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<TerminalUnit> parent = advanceLevel(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 136: {
		// right_brace -> } 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<TerminalUnit> parent = retreatLevel(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 138: {
		// expression -> expression , assignment_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 151: {
		// conditional_expression -> logical_or_expression ? expression : conditional_expression 
		shared_ptr<Command> unit5 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateConditionalExpression(unit1, unit2, unit3, unit4, unit5);
		mUnitStack.push(parent);
		break;
	}
	case 152: {
		// unary_expression -> postfix_expression 
		shared_ptr<PostfixExpression> unit1 = dynamic_pointer_cast<PostfixExpression>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateUnaryExpression(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 153: {
		// unary_expression -> INC_OP unary_expression 
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateUnaryExpression(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 154: {
		// unary_expression -> DEC_OP unary_expression 
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateUnaryExpression(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 155: {
		// unary_expression -> unary_operator cast_expression 
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateUnaryExpression(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 156: {
		// unary_expression -> SIZEOF ( type_name ) 
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TypeName> unit3 = dynamic_pointer_cast<TypeName>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateUnaryExpression(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 157: {
		// unary_expression -> SIZEOF unary_expression 
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateUnaryExpression(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 159: {
		// logical_or_expression -> logical_or_expression OR_OP logical_and_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 161: {
		// logical_and_expression -> logical_and_expression AND_OP inclusive_or_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 163: {
		// inclusive_or_expression -> inclusive_or_expression | exclusive_or_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 165: {
		// exclusive_or_expression -> exclusive_or_expression ^ and_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 167: {
		// and_expression -> and_expression & equality_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 169: {
		// equality_expression -> equality_expression EQ_OP relational_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 170: {
		// equality_expression -> equality_expression NE_OP relational_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 172: {
		// relational_expression -> relational_expression < shift_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 173: {
		// relational_expression -> relational_expression > shift_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 174: {
		// relational_expression -> relational_expression LE_OP shift_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 175: {
		// relational_expression -> relational_expression GE_OP shift_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 177: {
		// shift_expression -> shift_expression LEFT_OP additive_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 178: {
		// shift_expression -> shift_expression RIGHT_OP additive_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 180: {
		// additive_expression -> additive_expression + multiplicative_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 181: {
		// additive_expression -> additive_expression - multiplicative_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 183: {
		// multiplicative_expression -> multiplicative_expression * cast_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 184: {
		// multiplicative_expression -> multiplicative_expression / cast_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 185: {
		// multiplicative_expression -> multiplicative_expression % cast_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 187: {
		// cast_expression -> ( type_name ) cast_expression 
		shared_ptr<Command> unit4 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TypeName> unit2 = dynamic_pointer_cast<TypeName>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<Command> parent = generateCastExpression(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 194: {
		// postfix_expression -> primary_expression 
		shared_ptr<PrimaryExpression> unit1 = dynamic_pointer_cast<PrimaryExpression>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<PostfixExpression> parent = generatePostfixExpression(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 195: {
		// postfix_expression -> postfix_expression [ expression ] 
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<PostfixExpression> unit1 = dynamic_pointer_cast<PostfixExpression>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<PostfixExpression> parent = generatePostfixExpression(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 196: {
		// postfix_expression -> postfix_expression ( ) 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<PostfixExpression> unit1 = dynamic_pointer_cast<PostfixExpression>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<PostfixExpression> parent = generatePostfixExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 197: {
		// postfix_expression -> postfix_expression ( argument_expression_list ) 
		shared_ptr<TerminalUnit> unit4 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<ArgumentExpressionList> unit3 = dynamic_pointer_cast<ArgumentExpressionList>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<PostfixExpression> unit1 = dynamic_pointer_cast<PostfixExpression>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<PostfixExpression> parent = generatePostfixExpression(unit1, unit2, unit3, unit4);
		mUnitStack.push(parent);
		break;
	}
	case 198: {
		// postfix_expression -> postfix_expression . IDENTIFIER 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<PostfixExpression> unit1 = dynamic_pointer_cast<PostfixExpression>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<PostfixExpression> parent = generatePostfixExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 199: {
		// postfix_expression -> postfix_expression PTR_OP IDENTIFIER 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<PostfixExpression> unit1 = dynamic_pointer_cast<PostfixExpression>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<PostfixExpression> parent = generatePostfixExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 200: {
		// postfix_expression -> postfix_expression INC_OP 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<PostfixExpression> unit1 = dynamic_pointer_cast<PostfixExpression>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<PostfixExpression> parent = generatePostfixExpression(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 201: {
		// postfix_expression -> postfix_expression DEC_OP 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<PostfixExpression> unit1 = dynamic_pointer_cast<PostfixExpression>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<PostfixExpression> parent = generatePostfixExpression(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	case 202: {
		// primary_expression -> IDENTIFIER 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<PrimaryExpression> parent = generatePrimaryExpression(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 203: {
		// primary_expression -> CONSTANT 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<PrimaryExpression> parent = generatePrimaryExpression(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 204: {
		// primary_expression -> string_literal 
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<PrimaryExpression> parent = generatePrimaryExpression(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 205: {
		// primary_expression -> ( expression ) 
		shared_ptr<TerminalUnit> unit3 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<Command> unit2 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<PrimaryExpression> parent = generatePrimaryExpression(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 206: {
		// argument_expression_list -> assignment_expression 
		shared_ptr<Command> unit1 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<ArgumentExpressionList> parent = generateArgumentExpressionList(unit1);
		mUnitStack.push(parent);
		break;
	}
	case 207: {
		// argument_expression_list -> argument_expression_list , assignment_expression 
		shared_ptr<Command> unit3 = dynamic_pointer_cast<Command>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<ArgumentExpressionList> unit1 = dynamic_pointer_cast<ArgumentExpressionList>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<ArgumentExpressionList> parent = generateArgumentExpressionList(unit1, unit2, unit3);
		mUnitStack.push(parent);
		break;
	}
	case 209: {
		// string_literal -> string_literal STRING_LITERAL 
		shared_ptr<TerminalUnit> unit2 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		shared_ptr<TerminalUnit> unit1 = dynamic_pointer_cast<TerminalUnit>(mUnitStack.top());
		mUnitStack.pop();
		
		shared_ptr<TerminalUnit> parent = generateStringLiteral(unit1, unit2);
		mUnitStack.push(parent);
		break;
	}
	default:
		break;
	}
}
