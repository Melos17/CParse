#include "base.h"
#include "var.h"
#include "data.h"
#include "identifier.h"

using std::dynamic_pointer_cast;

CommandPtr Variable::generateLoadCode() {
	CommandPtr command = make_shared<Command>();
	if (storeType == Global) {
		command->addIMM(value);
	}
	else {
		command->addLEA(value);
	}
	
	switch(type->baseType) {
	case Type::SignedChar:
	case Type::UnsignedChar:
		command->addL8();
		break;
	case Type::SignedShort:
	case Type::UnsignedShort:
		command->addL16();
		break;
	case Type::SignedInt:
	case Type::UnsignedInt:
	case Type::Float:
		command->addL32();
		break;
	case Type::SignedLong:
	case Type::UnsignedLong:
	case Type::SignedLongLong:
	case Type::UnsignedLongLong:
	case Type::Pointer:
	case Type::Function:
	case Type::Struct:
	case Type::Union:
		command->addL64();
		break;
	case Type::Enum:
	case Type::Array:
		break;
	}
	return command;
}


VariableManager& VariableManager::getInstance() {
	static VariableManager sInstance;
	return sInstance;
}

// for non-const value (decided after executing)
// global value is distribute to a unique memory, local value's position is dependent on stack
VariablePtr VariableManager::addVariable(TypePtr type, const string& identifier, bool isStatic) {
	if (!identifier.empty() && IdentifierManager::getInstance().hasThisLevelIdentifier(identifier)) {
		throw RedefineException();
	}
	assert(type->size() > 0);
	IdentifierManager::getInstance().addVariable(identifier);
	
	Variable::StoreType storeType = isStatic ? Variable::Global : (mLevel == 0 ? Variable::Global : Variable::Local);
	VariablePtr var(new Variable(storeType, type, identifier));
	mVariables[mLevel][identifier] = var;

	if (var->storeType == Variable::Global) {
		size_t size = var->type->size();
		PosType pos = DataManager::getInstance().applyMemory(size);
		var->value = pos;
	}
	else {
		if (mLevel == 1) {
			var->value = -mStackSize[1] - 3;
			if (var->type->baseType == Type::Array) {
				shared_ptr<ArrayType> arrayType = dynamic_pointer_cast<ArrayType>(var->type);
				shared_ptr<PointerType> pointerType = make_shared<PointerType>(arrayType->pointType);
				var->type = pointerType;
			}
		}
		else {
			// Local variable without parameter
			int pos = 0;
			for (int i = 2; i <= mLevel; ++i) {
				pos += mStackSize[i];
			}
			var->value = pos;
		}
		// memory alignment
		size_t size = var->type->size();
		size = (size + (MemorySize - 1)) / MemorySize;
		mStackSize[mLevel] += size;
	}
	return var;
}

// for const value (decided before executing) : function position, enum
VariablePtr VariableManager::addVariable(TypePtr type, const string& identifier, BaseValueType value) {
	assert(!identifier.empty() && !IdentifierManager::getInstance().hasThisLevelIdentifier(identifier));
	IdentifierManager::getInstance().addVariable(identifier);
	
	Variable::StoreType storeType = Variable::Global;
	VariablePtr var(new Variable(storeType, type, identifier));
	var->value = value;
	mVariables[mLevel][identifier] = var;
	return var;
}

bool VariableManager::hasVariable(const string& identifier) {
	if(identifier.empty()) return false;
	for(int i = mLevel; i >= 0; --i) {
		auto& map = mVariables[i];
		if(map.find(identifier) != map.end()) {
			return true;
		}
	}
	return false;
}

// plz check hasType before getType
VariablePtr VariableManager::getVariable(const string& identifier) {
	for(int i = mLevel; i >= 0; --i) {
		auto& map = mVariables[i];
		auto var = map.find(identifier);
		if(var != map.end()) {
			return var->second;
		}
	}
	assert(false);
	return VariablePtr();
}

size_t VariableManager::getStackSizeThisLevel() {
	// Global value is distributed by DataManager, mStackSize[0] is meanless
	assert(mLevel > 0);
	return mStackSize[mLevel];
}

//
size_t VariableManager::getStackSizeNextLevel() {
	return mStackSize[mLevel + 1];
}

void VariableManager::advanceLevel() {
	++mLevel;
	mStackSize[mLevel] = 0;
	mVariables.resize(mLevel + 1, unordered_map<string, VariablePtr>());
}

void VariableManager::retreatLevel() {
	assert(mLevel > 0);
	mVariables[mLevel].clear();
	--mLevel;
}

void VariableManager::adjustParameters() {
	assert(mVariables.size() >= 2);
	BaseValueType capacity = mStackSize[1] / MemorySize;
	for (auto idAndVar : mVariables[1]) {
		idAndVar.second->value -= capacity;
	}
}
