#include "type.h"
#include "identifier.h"

using std::make_shared;

TypePtr SignedIntType = make_shared<Type>(Type::SignedInt);
TypePtr UnsignedIntType = make_shared<Type>(Type::UnsignedInt);
TypePtr SignedLongLongType = make_shared<Type>(Type::SignedLongLong);
TypePtr SignedCharType = make_shared<Type>(Type::SignedChar);
TypePtr SignedCharPointType = make_shared<PointerType>(SignedCharType);

size_t Type::size() {
	switch(baseType) {
	case Void: // considering pointer void*
	case SignedChar:
	case UnsignedChar:
		return 1;
	case SignedShort:
	case UnsignedShort:
		return 2;
	case SignedInt:
	case UnsignedInt:
	case Float:
		return 4;
	case SignedLong:
	case UnsignedLong:
	case SignedLongLong:
	case UnsignedLongLong:
	case Double:
		return 8;
	}
	return 0;
	assert(false);
}

shared_ptr<Type> Type::getUnsignedType() {
	BaseType newBaseType = Unknown;
	switch (baseType) {
	case SignedChar:
		newBaseType = UnsignedChar;
		break;
	case SignedShort:
		newBaseType = UnsignedShort;
		break;
	case SignedInt:
		newBaseType = UnsignedInt;
		break;
	case SignedLong:
		newBaseType = UnsignedLong;
		break;
	case SignedLongLong:
		newBaseType = UnsignedLongLong;
		break;
	default:
		assert(false);
	}
	return make_shared<Type>(newBaseType);
}

bool Type::isUnsigned() {
	switch(baseType) {
	case UnsignedChar:
	case UnsignedShort:
	case UnsignedInt:
	case UnsignedLong:
	case UnsignedLongLong:
		return true;
	}
	return false;
}

bool Type::isBaseType() {
	switch (baseType) {
	case SignedChar:
	case SignedShort:
	case SignedInt:
	case SignedLong:
	case SignedLongLong:
	case UnsignedChar:
	case UnsignedShort:
	case UnsignedInt:
	case UnsignedLong:
	case UnsignedLongLong:
	case Float:
	case Double:
	case Enum:
		return true;
	}
	return false; // struct, union
}

bool Type::isSmallThanInt() {
	switch (baseType) {
	case SignedChar:
	case SignedShort:
	case UnsignedChar:
	case UnsignedShort:
		return true;
	}
	return false;
}

bool Type::isLongLong() {
	switch (baseType) {
	case SignedLongLong:
	case UnsignedLongLong:
		return true;
	}
	return false;
}


void StructUnionType::init(vector<StructUnionMember>& members, bool isStruct) {
	size_ = alignSize_ = 0;
	if (isStruct) {
		for (StructUnionMember& member : members) {
			size_t s = member.type->size();
			size_t s2 = member.type->alignSize();
			size_ = ((size_ + (s2 - 1)) / s2) * s2;
			member.address = size_;
			size_ += s;
			if(alignSize_ < s2) alignSize_ = s2;
		}
		size_t s = alignSize();
		size_ = ((size_ + (s - 1)) / s) * s;
	}
	else {
		for (StructUnionMember& member : members) {
			size_t s = member.type->size();
			size_t s2 = member.type->alignSize();
			if(size_ < s) size_ = s;
			if(alignSize_ < s2) alignSize_ = s2;
			member.address = 0;
		}
	}
	this->members = members;
}

bool Type::canImplicitConverseTo(Type& type) {
	switch (baseType) {
	case SignedChar:
	case UnsignedChar:
		return type.baseType >= SignedChar && type.baseType <= UnsignedLongLong;
	case SignedShort:
	case UnsignedShort:
		return type.baseType >= SignedShort && type.baseType <= UnsignedLongLong;
	case SignedInt:
	case UnsignedInt:
	case SignedLong:
	case UnsignedLong:
	case Enum:
		return type.baseType >= SignedInt && type.baseType <= UnsignedLongLong;
	case SignedLongLong:
	case UnsignedLongLong:
		return type.baseType >= SignedLongLong && type.baseType <= UnsignedLongLong;
	case Float:
		return type.baseType >= Float && type.baseType <= Double;
	case Double:
		return type.baseType == Double;
	case Pointer:
		return (type.baseType == Pointer && (((PointerType*)this)->pointType->baseType == Void || ((PointerType&)type).pointType->baseType == Void)) || operator==(type);
	case Array: {
		if (type.baseType == Pointer) {
			PointerType& pointer = (PointerType&)type;
			ArrayType& arr = (ArrayType&)*this;
			if (pointer.pointType->baseType == Void || pointer.pointType == arr.pointType) {
				return true;
			}
		}
		break;
	}
	case Function:
	case Struct:
	case Union:
	case Unknown:
	case Void:
	default:
		assert(false);
	}
	return false;
}

TypesManager& TypesManager::getInstance() {
	static TypesManager sInstance;
	return sInstance;
}

void TypesManager::addType(const string& identifier, shared_ptr<Type> type) {
	assert(!IdentifierManager::getInstance().hasThisLevelIdentifier(identifier));
	IdentifierManager::getInstance().addTypename(identifier);
	
	mTypes[mLevel][identifier] = type;
}

bool TypesManager::hasType(const string& identifier) {
	for(int i = mLevel; i >= 0; --i) {
		auto& map = mTypes[i];
		if(map.find(identifier) != map.end()) {
			return true;
		}
	}
	return false;
}

shared_ptr<Type> TypesManager::getType(const string& identifier) {
	for(int i = mLevel; i >= 0; --i) {
		auto& map = mTypes[i];
		auto type = map.find(identifier);
		if(type != map.end()) {
			return type->second;
		}
	}
	// check hasType before getType
	assert(false);
	return shared_ptr<Type>();
}

void TypesManager::advanceLevel() {
	++mLevel;
	mTypes.resize(mLevel + 1, unordered_map<string, shared_ptr<Type>>());
}

void TypesManager::retreatLevel() {
	assert(mLevel > 0);
	--mLevel;
	mTypes[mLevel].clear();
}

void TypesManager::init() {
	mLevel = 0;
	mTypes.resize(1, unordered_map<string, shared_ptr<Type>>());
	mTypes[0].clear();
}

bool operator==(const TypePtr& type1, const TypePtr& type2) {
	if (!type1 || !type2) return false;
	return *type1 == *type2;
}
