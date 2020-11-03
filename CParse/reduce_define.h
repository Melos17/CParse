#ifndef REDUCE_DEFINE_H
#define REDUCE_DEFINE_H

#include <assert.h>

#include "type.h"
#include "reduce.h"
#include "function.h"

using std::move;

// function_definition : 
// function_definition_head compound_statement
void generateFunctionDefinition(CommandPtr unit2) {
	if (unit2->hasBreakOrContinue()) {
		throw InvalidBreakContinueException();
	}

	if (IdentifierManager::getInstance().getLevel() == 1) {
		IdentifierManager::getInstance().retreatLevel();
	}

	gCode->addENT();
	if (unit2->size() == 0 || unit2->back() != Command::LEV) {
		unit2->addLEV();
	}
	gCode->addCodes(unit2);
}

// function_definition_head :
// declaration_specifiers declarator
void generateFunctionDefinition(DeclarationSpecifiersPtr unit1, DeclaratorPtr unit2) {
	assert(unit2->type->baseType == Type::Function);

	string& funcName = unit2->identifier;
	FunctionTypePtr type = dynamic_pointer_cast<FunctionType>(unit2->type);
	addTypeToDeepest(unit1->type, type);

	PosType pos = gCode->size();
	shared_ptr<FunctionType> type_(type);
	FunctionManager::getInstance().addFunction(funcName, type_, pos);
	FunctionManager::getInstance().setFunctionPosition(pos, funcName);
}


// declaration_list : 
// declaration
DeclarationListPtr generateDeclarationList(DeclarationPtr unit1) {
	DeclarationListPtr declarationList = make_shared<DeclarationList>();
	declarationList->initializer = unit1->initializer;
	return declarationList;
}

// declaration_list declaration
DeclarationListPtr generateDeclarationList(DeclarationListPtr unit1, DeclarationPtr unit2) {
	if (unit2->initializer) unit1->initializer->addCodes(unit2->initializer);
	return unit1;
}


// declaration : 
// declaration_specifiers ';'
DeclarationPtr generateDeclaration(DeclarationSpecifiersPtr unit1, TerminalUnitPtr unit2) {
	DeclarationPtr declaration = make_shared<Declaration>();
	declaration->declarationSpecifiers = unit1;
	declaration->initializer = make_shared<Command>();
	return declaration;
}

// declaration_init_declarator_list ';'
DeclarationPtr generateDeclaration(DeclarationPtr unit1, TerminalUnitPtr unit2) {
	return unit1;
}


// declaration_init_declarator_list : 
// declaration_specifiers init_declarator
DeclarationPtr generateDeclaration(DeclarationSpecifiersPtr unit1, InitDeclaratorPtr unit2) {
	DeclarationPtr declaration = make_shared<Declaration>();
	declaration->declarationSpecifiers = unit1;
	declaration->initDeclarators.emplace_back(unit2);
	declaration->initializer = unit2->initializer;

	addDeclaration(unit1, unit2->declarator);
	shared_ptr<Variable> var = VariableManager::getInstance().getVariable(unit2->declarator->identifier);

	if (declaration->initializer->size() > 0) {
		CommandPtr code = make_shared<Command>();
		if (var->storeType == Variable::Global) {
			code->addIMM(var->value);
		}
		else {
			code->addLEA(var->value);
		}
		code->addPSH();
		code->addCodes(declaration->initializer);
		switch (var->type->size()) {
		case 1:
			code->addS8();
			break;
		case 2:
			code->addS16();
			break;
		case 4:
			code->addS32();
			break;
		case 8:
			code->addS64();
			break;
		default:
			assert(false);
		}
		declaration->initializer = code;
	}
	return declaration;
}

// declaration_init_declarator_list ',' init_declarator
DeclarationPtr generateDeclaration(DeclarationPtr unit1, TerminalUnitPtr unit2, InitDeclaratorPtr unit3) {
	unit1->initDeclarators.emplace_back(unit3);
	unit1->initializer = unit3->initializer;

	bool isArray = unit3->declarator->type && unit3->declarator->type->baseType == Type::Array;
	addDeclaration(unit1->declarationSpecifiers, unit3->declarator);
	shared_ptr<Variable> var = VariableManager::getInstance().getVariable(unit3->declarator->identifier);

	if (unit1->initializer->size() > 0) {
		CommandPtr code = make_shared<Command>();
		if (var->storeType == Variable::Global) {
			code->addIMM(var->value);
		}
		else {
			code->addLEA(var->value);
		}
		code->addPSH();
		code->addCodes(unit1->initializer);
		switch (var->type->size()) {
		case 1:
			code->addS8();
			break;
		case 2:
			code->addS16();
			break;
		case 4:
			code->addS32();
			break;
		case 8:
			code->addS64();
			break;
		default:
			assert(false);
		}
		unit1->initializer = code;
	}
	else if (isArray) {
		CommandPtr code = make_shared<Command>();
		if (var->storeType == Variable::Global) {
			code->addIMM(var->value - 1);
		}
		else {
			code->addLEA(var->value - 1);
		}
		code->addPSH();
		code->addS64();
		unit1->initializer = code;
	}
	return unit1;
}


// init_declarator : 
// declarator
InitDeclaratorPtr generateInitDeclarator(DeclaratorPtr unit1) {
	InitDeclaratorPtr initDeclarator = make_shared<InitDeclarator>();
	initDeclarator->declarator = unit1;
	initDeclarator->initializer = make_shared<Command>();
	return initDeclarator;
}

// declarator '=' initializer
InitDeclaratorPtr generateInitDeclarator(DeclaratorPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	InitDeclaratorPtr initDeclarator = make_shared<InitDeclarator>();
	initDeclarator->declarator = unit1;
	initDeclarator->initializer = unit3;
	return initDeclarator;
}


// initializer : 
// assignment_expression
CommandPtr generateInitializer(CommandPtr code) {
	return code;
}

// '{' initializer_list '}'
CommandPtr generateInitializer(TerminalUnitPtr unit1, InitializerListPtr unit2, TerminalUnitPtr unit3) {
	CommandPtr code = make_shared<Command>();
	assert(false); // no support now
	return code;
}

// '{' initializer_list ',' '}'
CommandPtr generateInitializer(TerminalUnitPtr unit1, InitializerListPtr unit2, TerminalUnitPtr unit3, TerminalUnitPtr unit4) {
	CommandPtr initializer = generateInitializer(unit1, unit2, unit3);
	return initializer;
}


// initializer_list : 
// initializer
InitializerListPtr generateInitializerList(CommandPtr unit1) {
	InitializerListPtr initializerList = make_shared<InitializerList>();
	initializerList->initializers.emplace_back(unit1);
	return initializerList;
}

// initializer_list ',' initializer
InitializerListPtr generateInitializerList(InitializerListPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	unit1->initializers.emplace_back(unit3);
	return unit1;
}


// parameter_type_list : 
// parameter_list
ParameterTypeListPtr generateParameterTypeList(ParameterListPtr unit1) {
	ParameterTypeListPtr parameterTypeList = make_shared<ParameterTypeList>();
	parameterTypeList->parameterList = unit1;
	
	VariableManager::getInstance().adjustParameters();
	return parameterTypeList;
}

// parameter_list ',' ELLIPSIS
ParameterTypeListPtr generateParameterTypeList(ParameterListPtr unit1, TerminalUnitPtr unit2, TerminalUnitPtr unit3) {
	assert(false); // No support now
	ParameterTypeListPtr parameterTypeList = make_shared<ParameterTypeList>();
	parameterTypeList->parameterList = unit1;
	parameterTypeList->hasEllipsis = true;
	
	VariableManager::getInstance().adjustParameters();
	return parameterTypeList;
}


// parameter_list : 
// parameter_declaration
ParameterListPtr generateParameterList(ParameterDeclarationPtr unit1) {
	ParameterListPtr parameterList = make_shared<ParameterList>();
	parameterList->parameterDeclarations.emplace_back(unit1);
	return parameterList;
}

// parameter_list ',' parameter_declaration
ParameterListPtr generateParameterList(ParameterListPtr unit1, TerminalUnitPtr unit2, ParameterDeclarationPtr unit3) {
	unit1->parameterDeclarations.emplace_back(unit3);
	return unit1;
}

// parameter_declaration : 
// declaration_specifiers declarator
ParameterDeclarationPtr generateParameterDeclaration(DeclarationSpecifiersPtr unit1, DeclaratorPtr unit2) {
	ParameterDeclarationPtr parameterDeclaration = make_shared<ParameterDeclaration>();
	parameterDeclaration->identifier = unit2->identifier;
	
	if (IdentifierManager::getInstance().getLevel() == 0) {
		IdentifierManager::getInstance().advanceLevel();
	}

	TypePtr type = unit2->type;
	// parameter array transfer to pointer because its size is always sizeof(T*)
	if (type && type->baseType == Type::Array) {
		ArrayTypePtr arrayType = dynamic_pointer_cast<ArrayType>(type);
		type = make_shared<PointerType>(arrayType->pointType);
	}
	if (type == nullptr) {
		type = unit1->type;
	}
	else {
		addTypeToDeepest(unit1->type, type);
	}
	VariableManager& varMgr = VariableManager::getInstance();
	varMgr.addVariable(type, unit2->identifier);
	
	parameterDeclaration->type = type;
	parameterDeclaration->storageClassSpecifier = unit1->storageClassSpecifier;
	return parameterDeclaration;
}

// declaration_specifiers abstract_declarator
ParameterDeclarationPtr generateParameterDeclaration(DeclarationSpecifiersPtr unit1, TypePtr unit2) {
	ParameterDeclarationPtr parameterDeclaration = make_shared<ParameterDeclaration>();
	
	TypePtr type = unit2;
	// parameter array transfer to pointer because its size is always sizeof(T*)
	if (type->baseType == Type::Array) {
		ArrayTypePtr arrayType = dynamic_pointer_cast<ArrayType>(type);
		type = make_shared<PointerType>(arrayType->pointType);
	}
	addTypeToDeepest(unit1->type, type);
	VariableManager& varMgr = VariableManager::getInstance();
	varMgr.addVariable(type, ""); // for extending size
	
	parameterDeclaration->type = type;
	parameterDeclaration->storageClassSpecifier = unit1->storageClassSpecifier;
	return parameterDeclaration;
}

// declaration_specifiers
ParameterDeclarationPtr generateParameterDeclaration(DeclarationSpecifiersPtr unit1) {
	ParameterDeclarationPtr parameterDeclaration = make_shared<ParameterDeclaration>();
	
	TypePtr type = unit1->type;
	VariableManager& varMgr = VariableManager::getInstance();
	varMgr.addVariable(type, "");
	
	parameterDeclaration->type = unit1->type;
	parameterDeclaration->storageClassSpecifier = unit1->storageClassSpecifier;
	return parameterDeclaration;
}


// enum_specifier : 
// ENUM '{' enumerator_list '}'
TypePtr generateEnumSpecifier(TerminalUnitPtr unit1, TerminalUnitPtr unit2, EnumeratorListPtr unit3,
		TerminalUnitPtr unit4) {
	EnumTypePtr type = make_shared<EnumType>();
	TypePtr type_ = type;
	vector<EnumMember>& members = type->members;
	int value = -1;
	for (EnumeratorPtr& enumerator : unit3->enumerators) {
		if (enumerator->hasValue) {
			value = enumerator->value;
		} else {
			++value;
		}
		VariableManager::getInstance().addVariable(type, enumerator->identifier, (BaseValueType)value);
		members.emplace_back(enumerator->identifier, value);
	}
	
	return type;
}

// ENUM IDENTIFIER '{' enumerator_list '}'
TypePtr generateEnumSpecifier(TerminalUnitPtr unit1, TerminalUnitPtr unit2, TerminalUnitPtr unit3,
		EnumeratorListPtr unit4, TerminalUnitPtr unit5) {
	string& identifier = unit2->word;
	TypePtr type = generateEnumSpecifier(unit1, unit3, unit4, unit5);
	TypesManager::getInstance().addType(identifier, type);
	return type;
}

// ENUM IDENTIFIER
TypePtr generateEnumSpecifier(TerminalUnitPtr unit1, TerminalUnitPtr unit2) {
	string& identifier = unit2->word;
	if (!TypesManager::getInstance().hasType(identifier)) {
		throw UnknownIdentifierException();
	}
	TypePtr type = TypesManager::getInstance().getType(identifier);
	return type;
}


// enumerator_list : 
// enumerator
EnumeratorListPtr generateEnumeratorList(EnumeratorPtr unit1) {
	EnumeratorListPtr enumeratorList = make_shared<EnumeratorList>();
	enumeratorList->enumerators.emplace_back(unit1);
	return enumeratorList;
}

// enumerator_list ',' enumerator
EnumeratorListPtr generateEnumeratorList(EnumeratorListPtr unit1, TerminalUnitPtr unit2, EnumeratorPtr unit3) {
	unit1->enumerators.emplace_back(unit3);
	return unit1;
}


// enumerator : 
// IDENTIFIER
EnumeratorPtr generateEnumerator(TerminalUnitPtr unit1) {
	EnumeratorPtr enumerator = make_shared<Enumerator>();
	enumerator->identifier = unit1->word;
	return enumerator;
}

// IDENTIFIER '=' constant_expression
EnumeratorPtr generateEnumerator(TerminalUnitPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	EnumeratorPtr enumerator = make_shared<Enumerator>();
	enumerator->identifier = unit1->word;
	enumerator->hasValue = true;
	enumerator->value = (int) unit3->back();
	return enumerator;
}


StructOrUnionSpecifierPtr generateStructOrUnionSpecifier(int unitType, const string& identifier, StructDeclarationListPtr declarationList) {
	StructOrUnionSpecifierPtr specifier = make_shared<StructOrUnionSpecifier>();
	bool isStruct = unitType == CWord::STRUCT;
	vector<StructUnionMember> members;
	
	for (StructDeclarationPtr structDeclaration : declarationList->structDeclarations) {
		for (StructDeclaratorPtr structDeclarator : structDeclaration->structDeclarators) {
			DeclaratorPtr declarator = structDeclarator->declarator;
			string& identifier = declarator->identifier;
			TypePtr type;
			if (declarator->type) {
				type = declarator->type;
				addTypeToDeepest(structDeclaration->type, type);
			}
			else {
				type = structDeclaration->type;
			}
			shared_ptr<Type> type_(type);
			members.emplace_back(type_, identifier);
		}
	}
	shared_ptr<StructUnionType> type_ = make_shared<StructUnionType>(members, isStruct);
	if (TypesManager::getInstance().hasType(identifier)) {
		throw RedefineException();
	}
	TypesManager::getInstance().addType(identifier, type_);
	
	specifier->identifier = identifier;
	specifier->type = type_;
	return specifier;
}


// struct_or_union_specifier : 
// struct_or_union IDENTIFIER '{' struct_declaration_list '}'
StructOrUnionSpecifierPtr generateStructOrUnionSpecifier(TerminalUnitPtr unit1, TerminalUnitPtr unit2,
		TerminalUnitPtr unit3, StructDeclarationListPtr unit4, TerminalUnitPtr unit5) {
	StructOrUnionSpecifierPtr specifier = generateStructOrUnionSpecifier(unit1->type, unit2->word, unit4);
	return specifier;
}

// struct_or_union '{' struct_declaration_list '}'
StructOrUnionSpecifierPtr generateStructOrUnionSpecifier(TerminalUnitPtr unit1, TerminalUnitPtr unit2,
		StructDeclarationListPtr unit3, TerminalUnitPtr unit4) {
	StructOrUnionSpecifierPtr specifier = generateStructOrUnionSpecifier(unit1->type, "", unit3);
	return specifier;
}

// struct_or_union IDENTIFIER
StructOrUnionSpecifierPtr generateStructOrUnionSpecifier(TerminalUnitPtr unit1, TerminalUnitPtr unit2) {
	StructOrUnionSpecifierPtr specifier = make_shared<StructOrUnionSpecifier>();
	string& identifier = unit2->word;
	if (TypesManager::getInstance().hasType(identifier)) {
		throw UnknownIdentifierException();
	}
	specifier->type = TypesManager::getInstance().getType(identifier);
	return specifier;
}


// struct_declaration_list : 
// struct_declaration
StructDeclarationListPtr generateStructDeclarationList(StructDeclarationPtr unit1) {
	StructDeclarationListPtr structDeclarationList = make_shared<StructDeclarationList>();
	structDeclarationList->structDeclarations.emplace_back(unit1);
	return structDeclarationList;
}

// struct_declaration_list struct_declaration
StructDeclarationListPtr generateStructDeclarationList(StructDeclarationListPtr unit1, StructDeclarationPtr unit2) {
	unit1->structDeclarations.emplace_back(unit2);
	return unit1;
}


// struct_declaration : 
// specifier_qualifier_list struct_declarator_list ';'
StructDeclarationPtr generateStructDeclaration(TypePtr unit1, StructDeclaratorListPtr unit2, TerminalUnitPtr unit3) {
	StructDeclarationPtr structDeclaration = make_shared<StructDeclaration>();
	structDeclaration->type = unit1;
	structDeclaration->structDeclarators = move(unit2->structDeclarators);
	return structDeclaration;
}

// struct_declarator_list : 
// struct_declarator
StructDeclaratorListPtr generateStructDeclaratorList(StructDeclaratorPtr unit1) {
	StructDeclaratorListPtr structDeclaratorList = make_shared<StructDeclaratorList>();
	structDeclaratorList->structDeclarators.emplace_back(unit1);
	return structDeclaratorList;
}

// struct_declarator_list ',' struct_declarator
StructDeclaratorListPtr generateStructDeclaratorList(StructDeclaratorListPtr unit1, TerminalUnitPtr unit2, StructDeclaratorPtr unit3) {
	unit1->structDeclarators.emplace_back(unit3);
	return unit1;
}


// struct_declarator : 
// declarator
StructDeclaratorPtr generateStructDeclarator(DeclaratorPtr unit1) {
	StructDeclaratorPtr structDeclarator = make_shared<StructDeclarator>();
	structDeclarator->declarator = unit1;
	return structDeclarator;
}

// ':' constant_expression
StructDeclaratorPtr generateStructDeclarator(TerminalUnitPtr unit1, CommandPtr unit2) {
	assert(false); // No support now
	StructDeclaratorPtr structDeclarator = make_shared<StructDeclarator>();
	structDeclarator->constant = unit2->back();
	return structDeclarator;
}

// declarator ':' constant_expression
StructDeclaratorPtr generateStructDeclarator(DeclaratorPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	assert(false); // No support now
	StructDeclaratorPtr structDeclarator = make_shared<StructDeclarator>();
	structDeclarator->declarator = unit1;
	structDeclarator->constant = unit3->back();
	return structDeclarator;
}

#endif