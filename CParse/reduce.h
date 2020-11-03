#ifndef REDUCE_H
#define REDUCE_H

#include <assert.h>
#include <memory>
#include "base.h"
#include "Word/word.h"
#include "Parse/parse.h"
#include "identifier.h"
#include "type.h"
#include "command.h"
#include "var.h"
#include "data.h"
#include "exception.h"

using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::make_shared;

struct ArgumentExpressionList : Polymorphism {
	vector<CommandPtr> parameters;
};
typedef shared_ptr<ArgumentExpressionList> ArgumentExpressionListPtr;

struct StorageClassSpecifier : Polymorphism {
	bool isTypedef;
	bool isExtern;
	bool isStatic;
	bool isRegister;
};
typedef shared_ptr<StorageClassSpecifier> StorageClassSpecifierPtr;

struct DeclarationSpecifiers : Polymorphism {
	TypePtr type;
	StorageClassSpecifier storageClassSpecifier;
};
typedef shared_ptr<DeclarationSpecifiers> DeclarationSpecifiersPtr;

struct InitDeclarator;
typedef shared_ptr<InitDeclarator> InitDeclaratorPtr;
struct Declaration : Polymorphism {
	DeclarationSpecifiersPtr declarationSpecifiers;
	vector<InitDeclaratorPtr> initDeclarators;
	CommandPtr initializer;
};
typedef shared_ptr<Declaration> DeclarationPtr;

struct Declarator : Polymorphism {
	string identifier;
	TypePtr type;
	CommandPtr initializer;
};
typedef shared_ptr<Declarator> DeclaratorPtr;

struct InitDeclarator : Polymorphism {
	DeclaratorPtr declarator;
	CommandPtr initializer;
};

struct StructDeclarator : Polymorphism {
	DeclaratorPtr declarator;
	BaseValueType constant;
};
typedef shared_ptr<StructDeclarator> StructDeclaratorPtr;

struct StructDeclaratorList : Polymorphism {
	vector<StructDeclaratorPtr> structDeclarators;
};
typedef shared_ptr<StructDeclaratorList> StructDeclaratorListPtr;

struct StructDeclaration : Polymorphism {
	TypePtr type;
	vector<StructDeclaratorPtr> structDeclarators;
};
typedef shared_ptr<StructDeclaration> StructDeclarationPtr;

struct StructDeclarationList : Polymorphism {
	vector<StructDeclarationPtr> structDeclarations;
};
typedef shared_ptr<StructDeclarationList> StructDeclarationListPtr;

struct StructOrUnionSpecifier : Polymorphism {
	string identifier;
	shared_ptr<Type> type;
};
typedef shared_ptr<StructOrUnionSpecifier> StructOrUnionSpecifierPtr;

struct Enumerator : Polymorphism {
	string identifier;
	bool hasValue;
	int value;
};
typedef shared_ptr<Enumerator> EnumeratorPtr;

struct EnumeratorList : Polymorphism {
	vector<EnumeratorPtr> enumerators;
};
typedef shared_ptr<EnumeratorList> EnumeratorListPtr;

struct DeclarationList : Polymorphism {
	CommandPtr initializer;
};
typedef shared_ptr<DeclarationList> DeclarationListPtr;

struct ParameterDeclaration : Polymorphism {
	StorageClassSpecifier storageClassSpecifier;
	TypePtr type;
	string identifier;
};
typedef shared_ptr<ParameterDeclaration> ParameterDeclarationPtr;

struct ParameterList : Polymorphism {
	vector<ParameterDeclarationPtr> parameterDeclarations;
};
typedef shared_ptr<ParameterList> ParameterListPtr;

struct ParameterTypeList : Polymorphism {
	ParameterListPtr parameterList;
	bool hasEllipsis;
};
typedef shared_ptr<ParameterTypeList> ParameterTypeListPtr;

struct InitializerList : Polymorphism {
	vector<CommandPtr> initializers;
};
typedef shared_ptr<InitializerList> InitializerListPtr;

struct TypeName : Polymorphism {
	TypePtr type;
};
typedef shared_ptr<TypeName> TypeNamePtr;

struct PostfixExpression : Polymorphism {
	CommandPtr command;
	string functionIdentifier;
};
typedef shared_ptr<PostfixExpression> PostfixExpressionPtr;

struct PrimaryExpression : Polymorphism {
	CommandPtr command;
	string functionIdentifier;
};
typedef shared_ptr<PrimaryExpression> PrimaryExpressionPtr;


void addTypeToDeepest(TypePtr source, TypePtr target) {
	assert(target);
	shared_ptr<Type> deeper = nullptr;
	switch (target->baseType) {
	case Type::Array: {
		ArrayType& type = (ArrayType&)*target;
		if (type.pointType) {
			addTypeToDeepest(source, type.pointType);
		}
		else {
			type.pointType = source;
		}
		break;
	}
	case Type::Pointer: {
		PointerType& type = (PointerType&)*target;
		if (type.pointType) {
			addTypeToDeepest(source, type.pointType);
		}
		else {
			type.pointType = source;
		}
		break;
	}
	case Type::Function: {
		FunctionType& type = (FunctionType&)*target;
		if (type.returnType) {
			addTypeToDeepest(source, type.returnType);
		}
		else {
			type.returnType = source;
		}
		break;
	}
	default:
		assert(false);
		break;
	}
}

void addTypeToDeepest(TypePtr type, DeclaratorPtr declarator) {
	if (declarator->type) {
		addTypeToDeepest(type, declarator->type);
	}
	else {
		declarator->type = type;
	}
}

void addDeclaration(DeclarationSpecifiersPtr declarationSpecifiers, DeclaratorPtr declarator) {
	TypePtr type;
	if (declarator->type) {
		type = declarator->type;
		addTypeToDeepest(declarationSpecifiers->type, type);
	}
	else {
		type = declarationSpecifiers->type;
	}
	shared_ptr<Type> type_(type);
	VariableManager& varMgr = VariableManager::getInstance();
	varMgr.addVariable(type_, declarator->identifier, declarationSpecifiers->storageClassSpecifier.isStatic);
}


void converseType(CommandPtr cmd, shared_ptr<Type>& type) {
	shared_ptr<Type> originType = cmd->getType();
	if (originType->baseType == Type::Array && type->baseType == Type::Pointer) {
		cmd->setType(type);
		return;
	}

	if (originType->baseType == Type::Pointer && type->baseType == Type::Pointer) {
		return;
	}

	// struct, union can not converse
	assert(originType->baseType != Type::Struct && originType->baseType != Type::Union && type->baseType != Type::Struct && type->baseType != Type::Union);
	size_t originSize = originType->size();
	// extend sign bit
	if (originSize > type->size() && !type->isUnsigned()) {
		switch (type->size()) {
		case 1:
			cmd->addT8();
			break;
		case 2:
			cmd->addT16();
			break;
		case 4:
			cmd->addT32();
			break;
		}
	}
	// clear over bit
	else if (originSize < type->size() && originType->isUnsigned()) {
		switch (originSize) {
		case 1:
			cmd->addTU8();
			break;
		case 2:
			cmd->addTU16();
			break;
		case 4:
			cmd->addTU32();
			break;
		}
	}
	cmd->setType(type);
}

void implicitConverseTypeTo(CommandPtr from, CommandPtr to) {
	shared_ptr<Type> fromType = from->getType();
	shared_ptr<Type> toType = to->getType();
	if (fromType == toType) return;
	
	if (!fromType->isBaseType() || !toType->isBaseType() || fromType->size() > toType->size()) {
		throw InvalidTypeConversionException();
	}
	
	if (fromType->size() < toType->size()) {
		converseType(from, toType);
	}
	else {
		if (!fromType->isUnsigned() && toType->isUnsigned()) {
			from->setType(fromType->getUnsignedType());
		}
	}
}

// implicit conversion for binary operate
shared_ptr<Type> implicitConverseType(CommandPtr cmd1, CommandPtr cmd2) {
	shared_ptr<Type> type1 = cmd1->getType();
	shared_ptr<Type> type2 = cmd2->getType();
	if (type1 == type2) return type1;

	// any type operate with pointer need to converse to signed int (32bits) or signed long long (64bits)
	if ((type1->baseType == Type::Pointer && type2->baseType != Type::Pointer) ||
		(type1->baseType == Type::Array && type2->baseType != Type::Array)) {
		converseType(cmd2, SignedLongLongType);
		return type1;
	}
	else if ((type1->baseType != Type::Pointer && type2->baseType == Type::Pointer) ||
		(type1->baseType != Type::Array && type2->baseType == Type::Array)) {
		converseType(cmd1, SignedLongLongType);
		return type2;
	}

	if (!type1->isBaseType() || !type2->isBaseType()) {
		throw InvalidTypeConversionException();
	}

	if (type1->size() > type2->size()) {
		converseType(cmd2, type1);
		return type1;
	}
	else if (type1->size() < type2->size()) {
		converseType(cmd1, type2);
		return type2;
	}
	else {
		if (type1->isUnsigned() && !type2->isUnsigned()) {
			cmd2->setType(type2->getUnsignedType());
		}
		else if (!type1->isUnsigned() && type2->isUnsigned()) {
			cmd1->setType(type1->getUnsignedType());
		}
		return type1;
	}
}

#endif