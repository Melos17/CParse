#ifndef TYPE_H 
#define TYPE_H

#include <iostream>
#include <assert.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "Parse/parse.h"
#include "base.h"

using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;
using std::shared_ptr;
using std::weak_ptr;

struct Type : Polymorphism {
	enum BaseType {
		Unknown,
		Void,
		SignedChar, UnsignedChar,
		SignedShort, UnsignedShort,
		SignedInt, UnsignedInt,
		Enum,
		SignedLong, UnsignedLong,
		SignedLongLong, UnsignedLongLong,
		Float, Double,
		Array,
		Pointer,
		Function,
		Struct,
		Union,
	};

	Type(BaseType baseType) {
		this->baseType = baseType;
		isConst = isVolatile = false;
	}

	virtual bool operator==(Type& type) {
		return baseType == type.baseType /* && isConst == type.isConst && isVolatile == type.isVolatile */;
	}

	virtual size_t size();
	virtual size_t alignSize() { return size(); }
	
	bool canImplicitConverseTo(Type& type);
	
	shared_ptr<Type> getUnsignedType();
	
	bool isUnsigned();
	bool isBaseType();
	bool isSmallThanInt();
	bool isLongLong();

	BaseType baseType;
	bool isConst;
	bool isVolatile;
};
typedef shared_ptr<Type> TypePtr;

bool operator==(const TypePtr& type1, const TypePtr& type2);

struct ArrayType : Type {
	ArrayType(size_t count) : Type(Array) {
		this->count = count;
	}
	
	virtual bool operator==(Type& type) {
		if (!Type::operator==(type)) {
			return false;
		}
		ArrayType& arrayType = (ArrayType&)type;
		return count == arrayType.count &&
			((!pointType && !arrayType.pointType) ||
			(pointType && pointType->operator==(*arrayType.pointType)));
	}
	
	virtual size_t size() {
		return count * pointType->size();
	}

	virtual size_t alignSize() {
		return pointType->alignSize();
	}
	
	shared_ptr<Type> pointType;
	size_t count;
};
typedef shared_ptr<ArrayType> ArrayTypePtr;


struct PointerType : Type {
	PointerType() : Type(Pointer) {
	}
	
	PointerType(shared_ptr<Type>& pointType) : Type(Pointer) {
		this->pointType = pointType;
	}

	virtual bool operator==(Type& type) {
		if (!Type::operator==(type)) {
			return false;
		}
		PointerType& pointType = (PointerType&)type;
		return /* isConstPtr == pointType.isConstPtr && isVolatilePtr == pointType.isVolatilePtr &&*/
			((!this->pointType && !pointType.pointType) ||
			(this->pointType && this->pointType == pointType.pointType));
	}
	
	virtual size_t size() {
		return MemorySize;
	}
	
	shared_ptr<Type> pointType;
	bool isConstPtr;
	bool isVolatilePtr;
};
typedef shared_ptr<PointerType> PointerTypePtr;


struct FunctionType : Type {
	FunctionType() : Type(Function) {
	}

	FunctionType(vector<shared_ptr<Type>>& parameterType) : Type(Function) {
		this->parameterType = parameterType;
	}
	
	FunctionType(shared_ptr<Type>& returnType, vector<shared_ptr<Type>>& parameterType) : Type(Function) {
		this->returnType = returnType;
		this->parameterType = parameterType;
	}

	virtual bool operator==(Type& type) {
		if (!Type::operator==(type)) {
			return false;
		}
		FunctionType& functionType = (FunctionType&)type;
		return returnType == functionType.returnType &&
			parameterType == functionType.parameterType;
	}
	
	// never call
	virtual size_t size() {
		assert(false);
		return MemorySize;
	}
	
	shared_ptr<Type> returnType;
	vector<shared_ptr<Type>> parameterType;
};
typedef shared_ptr<FunctionType> FunctionTypePtr;


struct EnumMember {
	string identifier;
	int value;
	EnumMember(const string& identifier, int value) {
		this->identifier = identifier;
		this->value = value;
	}
};

struct EnumType : Type {
	EnumType() : Type(Enum) {
	}

	EnumType(vector<EnumMember>& members) : Type(Enum) {
		this->members = members;
	}

	virtual bool operator==(Type& type) {
		return false;
	}
	
	virtual size_t size() {
		return 4;
	}

	vector<EnumMember> members;
};
typedef shared_ptr<EnumType> EnumTypePtr;


struct StructUnionMember {
	shared_ptr<Type> type;
	string identifier;
	int address;
	
	StructUnionMember(const shared_ptr<Type>& type, const string& identifier) {
		this->type = type;
		this->identifier = identifier;
		address = 0; // decided by StructUnionType::init
	}
};

struct StructUnionType : Type {
	StructUnionType(vector<StructUnionMember>& members, bool isStruct) : Type(isStruct ? Struct : Union) {
		init(members, isStruct);
	}

	virtual bool operator==(Type& type) {
		return false;
	}
	
	virtual size_t size() {
		return size_;
	}

	virtual size_t alignSize() {
		return alignSize_;
	}

	vector<StructUnionMember> members;
private:
	void init(vector<StructUnionMember>& members, bool isStruct);
	size_t size_;
	size_t alignSize_;
};
typedef shared_ptr<StructUnionType> StructUnionTypePtr;

class TypesManager {
public:
	static TypesManager& getInstance();

	void addType(const string& identifier, shared_ptr<Type> type);
	bool hasType(const string& identifier);

	shared_ptr<Type> getType(const string& identifier);
	
	void advanceLevel();
	void retreatLevel();
	
	void init();

private:
	TypesManager() { init(); }
	
	int mLevel;
	vector<unordered_map<string, shared_ptr<Type>>> mTypes;
};

extern TypePtr SignedIntType;
extern TypePtr UnsignedIntType;
extern TypePtr SignedLongLongType;
extern TypePtr SignedCharType;
extern TypePtr SignedCharPointType;

#endif