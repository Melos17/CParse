#ifndef REDUCE_TYPE_H
#define REDUCE_TYPE_H

#include "reduce.h"

using std::move;

// declaration_specifiers : 
// storage_class_specifier
DeclarationSpecifiersPtr generateDeclarationSpecifiers(StorageClassSpecifierPtr unit1) {
	DeclarationSpecifiersPtr declarationSpecifiers = make_shared<DeclarationSpecifiers>();
	declarationSpecifiers->storageClassSpecifier = *unit1;
	return declarationSpecifiers;
}

// storage_class_specifier declaration_specifiers
DeclarationSpecifiersPtr generateDeclarationSpecifiers(StorageClassSpecifierPtr unit1, DeclarationSpecifiersPtr unit2) {
	unit2->storageClassSpecifier.isTypedef |=  unit1->isTypedef;
	unit2->storageClassSpecifier.isExtern |= unit1->isExtern;
	unit2->storageClassSpecifier.isStatic |= unit1->isStatic;
	unit2->storageClassSpecifier.isRegister |= unit1->isRegister;
	return unit2;
}

// type_specifier
// type_qualifier
DeclarationSpecifiersPtr generateDeclarationSpecifiers(TypePtr unit1) {
	DeclarationSpecifiersPtr declarationSpecifiers = make_shared<DeclarationSpecifiers>();
	declarationSpecifiers->type = unit1;
	return declarationSpecifiers;
}

// type_specifier declaration_specifiers
// type_qualifier declaration_specifiers
DeclarationSpecifiersPtr generateDeclarationSpecifiers(TypePtr unit1, DeclarationSpecifiersPtr unit2) {
	TypePtr type2 = make_shared<Type>(unit2->type->baseType);
	switch (unit1->baseType) {
	case Type::UnsignedInt: {
		switch (type2->baseType) {
		case Type::SignedChar:
			type2->baseType = Type::UnsignedChar;
			break;
		case Type::SignedShort:
			type2->baseType = Type::UnsignedShort;
			break;
		case Type::SignedInt:
			type2->baseType = Type::UnsignedInt;
			break;
		case Type::SignedLong:
			type2->baseType = Type::UnsignedLong;
			break;
		case Type::SignedLongLong:
			type2->baseType = Type::UnsignedLongLong;
			break;
		default:
			assert(false);
		}
		break;
	}
	case Type::SignedLong: {
		switch (unit2->type->baseType) {
		case Type::Unknown:
			type2->baseType = Type::SignedLong;
			break;
		case Type::SignedLong:
			type2->baseType = Type::SignedLongLong;
			break;
		default:
			assert(false);
		}
		break;
	}
	case Type::Unknown: {
		type2->baseType = unit2->type->baseType;
		break;
	}
	}
	unit2->type = type2;
	unit2->type->isConst = unit1->isConst;
	unit2->type->isVolatile = unit1->isVolatile;
	return unit2;
}


// storage_class_specifier : 
// ...
StorageClassSpecifierPtr generateStorageClassSpecifier(TerminalUnitPtr unit1) {
	//--still no support
	assert(false);
	StorageClassSpecifierPtr storageClassSpecifier = make_shared<StorageClassSpecifier>();
	switch(unit1->type) {
	case CWord::TYPEDEF:
		storageClassSpecifier->isTypedef = true;
		break;
	case CWord::EXTERN:
		storageClassSpecifier->isExtern = true;
		break;
	case CWord::STATIC:
		storageClassSpecifier->isStatic = true;
		break;
	case CWord::REGISTER:
		storageClassSpecifier->isRegister = true;
		break;
	}
	return storageClassSpecifier;
}


// type_specifier : 
// ...
TypePtr generateTypeSpecifier(TerminalUnitPtr unit1) {
	TypePtr type = nullptr;
	switch(unit1->type) {
	case CWord::VOID:
		type = make_shared<Type>(Type::Void);
		break;
	case CWord::CHAR:
		type = make_shared<Type>(Type::SignedChar);
		break;
	case CWord::SHORT:
		type = make_shared<Type>(Type::SignedShort);
		break;
	case CWord::INT:
		type = make_shared<Type>(Type::SignedInt);
		break;
	case CWord::LONG:
		type = make_shared<Type>(Type::SignedLong);
		break;
	case CWord::FLOAT:
		type = make_shared<Type>(Type::Float);
		break;
	case CWord::DOUBLE:
		type = make_shared<Type>(Type::Double);
		break;
	case CWord::SIGNED:
		type = make_shared<Type>(Type::SignedInt);
		break;
	case CWord::UNSIGNED:
		type = make_shared<Type>(Type::UnsignedInt);
		break;
	case CWord::TYPE_NAME:
		string& name = unit1->word;
		assert(TypesManager::getInstance().hasType(name));
		type = TypesManager::getInstance().getType(name);
		break;
	}
	return type;
}

// struct_or_union_specifier
TypePtr generateTypeSpecifier(StructOrUnionSpecifierPtr unit1) {
	TypePtr type = unit1->type;
	return type;
}

TypePtr generateTypeSpecifier(TypePtr unit1) {
	return unit1;
}

// type_qualifier_list : 
// type_qualifier_list type_qualifier
TypePtr generateTypeQualifier(TypePtr unit1, TypePtr unit2) {
	unit1->isConst |= unit2->isConst;
	unit1->isVolatile |= unit2->isVolatile;
	return unit1;
}


// type_qualifier : 
// ...
TypePtr generateTypeQualifier(TerminalUnitPtr unit1) {
	TypePtr type = make_shared<Type>(Type::Unknown);
	switch(unit1->type) {
	case CWord::CONST:
		type->isConst = true;
		break;
	case CWord::VOLATILE:
		type->isVolatile = true;
		break;
	}
	return type;
}


// declarator : 
// pointer direct_declarator
DeclaratorPtr generateDeclarator(TypePtr unit1, DeclaratorPtr unit2) {
	addTypeToDeepest(unit1, unit2);
	return unit2;
}

// direct_declarator
DeclaratorPtr generateDeclarator(DeclaratorPtr unit1) {
	return unit1;
}


// pointer : 
// '*'
TypePtr generatePointer(TerminalUnitPtr unit1) {
	PointerTypePtr pointer = make_shared<PointerType>();
	return pointer;
}

// '*' type_qualifier_list
// '*' pointer
TypePtr generatePointer(TerminalUnitPtr unit1, TypePtr unit2) {
	PointerTypePtr pointType = make_shared<PointerType>();
	if (unit2->baseType == Type::Unknown) {
		pointType->isConst |= unit2->isConst;
		pointType->isVolatile |= unit2->isVolatile;
	}
	else {
		pointType->pointType = unit2;
	}
	return pointType;
}

// '*' type_qualifier_list pointer
TypePtr generatePointer(TerminalUnitPtr unit1, TypePtr unit2, TypePtr unit3) {
	PointerTypePtr pointType = make_shared<PointerType>();
	pointType->isConst |= unit2->isConst;
	pointType->isVolatile |= unit2->isVolatile;
	pointType->pointType = unit3;
	return pointType;
}


// direct_declarator : 
// IDENTIFIER
DeclaratorPtr generateDirectDeclarator(TerminalUnitPtr unit1) {
	DeclaratorPtr declarator = make_shared<Declarator>();
	declarator->identifier = unit1->word;
	declarator->type = nullptr;
	return declarator;
}

// '(' declarator ')'
DeclaratorPtr generateDirectDeclarator(TerminalUnitPtr unit1, DeclaratorPtr unit2, TerminalUnitPtr unit3) {
	return unit2;
}

// direct_declarator '[' ']'
// direct_declarator '(' ')'
DeclaratorPtr generateDirectDeclarator(DeclaratorPtr unit1, TerminalUnitPtr unit2, TerminalUnitPtr unit3) {
	switch(unit2->type) {
	case '[': {
		ArrayTypePtr type = make_shared<ArrayType>(0);
		addTypeToDeepest(type, unit1);
		break;
	}
	case '(': {
		FunctionTypePtr type = make_shared<FunctionType>();
		addTypeToDeepest(type, unit1);
		break;
	}
	}
	return unit1;
}

// direct_declarator '[' constant_expression ']'
DeclaratorPtr generateDirectDeclarator(DeclaratorPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3,
		TerminalUnitPtr unit4) {
	size_t count = (size_t)unit3->back();
	ArrayTypePtr type = make_shared<ArrayType>(count);
	addTypeToDeepest(type, unit1);
	return unit1;
}

// direct_declarator '(' parameter_type_list ')'
DeclaratorPtr generateDirectDeclarator(DeclaratorPtr unit1, TerminalUnitPtr unit2, ParameterTypeListPtr unit3,
		TerminalUnitPtr unit4) {
	FunctionTypePtr type = make_shared<FunctionType>();
	for(ParameterDeclarationPtr declaration : unit3->parameterList->parameterDeclarations) {
		type->parameterType.emplace_back(declaration->type);
	}
	addTypeToDeepest(type, unit1);
	return unit1;
}


// abstract_declarator : 
// pointer direct_abstract_declarator
TypePtr generateAbstractDeclarator(TypePtr unit1, TypePtr unit2) {
	addTypeToDeepest(unit1, unit2);
	return unit2;
}

// pointer
TypePtr generateAbstractDeclarator(TypePtr unit1) {
	return unit1;
}


// direct_abstract_declarator : 
// '(' abstract_declarator ')'
TypePtr generateDirectAbstractDeclarator(TerminalUnitPtr unit1, TypePtr unit2,
		TerminalUnitPtr unit3) {
	//--undone
	assert(false);
	TypePtr type = make_shared<PointerType>();
	return type;
}

// '[' ']'
// '(' ')'
TypePtr generateDirectAbstractDeclarator(TerminalUnitPtr unit1, TerminalUnitPtr unit2) {
	TypePtr type;
	switch (unit1->type) {
	case '[':
		type = make_shared<ArrayType>(0);
		break;
	case '(':
		type = make_shared<FunctionType>();
		break;
	}
	return type;
}

// '[' constant_expression ']'
TypePtr generateDirectAbstractDeclarator(TerminalUnitPtr unit1, CommandPtr unit2,
		TerminalUnitPtr unit3) {
	size_t size = (size_t)unit2->back();
	TypePtr type = make_shared<ArrayType>(size);
	return type;
}

// direct_abstract_declarator '[' ']'
TypePtr generateDirectAbstractDeclarator(TypePtr unit1, TerminalUnitPtr unit2, TerminalUnitPtr unit3) {
	ArrayTypePtr type = make_shared<ArrayType>(0);
	type->pointType = unit1;
	return unit1;
}

// direct_abstract_declarator '[' constant_expression ']'
TypePtr generateDirectAbstractDeclarator(TypePtr unit1, TerminalUnitPtr unit2,
		CommandPtr unit3, TerminalUnitPtr unit4) {
	size_t size = (size_t)unit3->back();
	ArrayTypePtr type = make_shared<ArrayType>(size);
	type->pointType = unit1;
	return type;
}

// '(' parameter_type_list ')'
TypePtr generateDirectAbstractDeclarator(TerminalUnitPtr unit1, ParameterTypeListPtr unit2,
		TerminalUnitPtr unit3) {
	FunctionTypePtr type = make_shared<FunctionType>();
	return type;
}

// direct_abstract_declarator '(' parameter_type_list ')'
TypePtr generateDirectAbstractDeclarator(TypePtr unit1, TerminalUnitPtr unit2,
		ParameterTypeListPtr unit3, TerminalUnitPtr unit4) {
	//--undone
	assert(false);
	return unit1;
}


// specifier_qualifier_list : 
// type_specifier specifier_qualifier_list
// type_qualifier specifier_qualifier_list
TypePtr generateSpecifierQualifierList(TypePtr unit1, TypePtr unit2) {
	// no strict code
	// only considering :
	// int / unsigned int
	// short / unsigned short
	// long / unsigned long
	// long long / unsigned long long
	TypePtr type = make_shared<Type>(unit1->baseType);
	type->isConst = unit1->isConst;
	type->isVolatile = unit1->isVolatile;
	switch (unit1->baseType) {
	case Type::UnsignedInt: {
		switch (unit2->baseType) {
		case Type::SignedChar:
			type->baseType = Type::UnsignedChar;
			break;
		case Type::SignedShort:
			type->baseType = Type::UnsignedShort;
			break;
		case Type::SignedInt:
			type->baseType = Type::UnsignedInt;
			break;
		case Type::SignedLong:
			type->baseType = Type::UnsignedLong;
			break;
		case Type::SignedLongLong:
			type->baseType = Type::UnsignedLongLong;
			break;
		default:
			assert(false);
		}
		break;
	}
	case Type::SignedLong: {
		switch (unit2->baseType) {
		case Type::Unknown:
			type->baseType = Type::SignedLong;
			break;
		case Type::SignedLong:
			type->baseType = Type::SignedLongLong;
			break;
		default:
			assert(false);
		}
		break;
	}
	case Type::Unknown: {
		type->baseType = unit2->baseType;
		break;
	}
	default:
		assert(false);
	}
	type->isConst |= unit2->isConst;
	type->isVolatile |= unit2->isVolatile;
	return type;
}

// type_name : 
// specifier_qualifier_list
TypeNamePtr generateTypeName(TypePtr unit1) {
	TypeNamePtr typeName = make_shared<TypeName>();
	typeName->type = unit1;
	return typeName;
}

// specifier_qualifier_list abstract_declarator
TypeNamePtr generateTypeName(TypePtr unit1, TypePtr unit2) {
	TypeNamePtr typeName = make_shared<TypeName>();
	addTypeToDeepest(unit1, unit2);
	typeName->type = unit2;
	return typeName;
}

#endif