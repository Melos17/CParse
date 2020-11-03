#include "identifier.h"
#include "var.h"
#include "type.h"

IdentifierManager& IdentifierManager::getInstance() {
	static IdentifierManager sInstance;
	return sInstance;
}

// function allows overload
void IdentifierManager::addFunction(const string& identifier) {
	IdentifierType type = getType(identifier);
	if (type != NonExist && type != Function) throw RedefineException();
	mIdentifiers[mLevel][identifier] = Function;
}

void IdentifierManager::addVariable(const string& identifier) {
	if (hasThisLevelIdentifier(identifier)) throw RedefineException();
	mIdentifiers[mLevel][identifier] = Var;
}

void IdentifierManager::addTypename(const string& identifier) {
	if (hasThisLevelIdentifier(identifier)) throw RedefineException();
	mIdentifiers[mLevel][identifier] = Typename;
}

IdentifierManager::IdentifierType IdentifierManager::getType(const string& identifier) {
	assert(!identifier.empty());
	for (int level = mLevel; level >= 0; --level) {
		auto it = mIdentifiers[level].find(identifier);
		if (it != mIdentifiers[level].end()) {
			return it->second;
		}
	}
	return NonExist;
}

bool IdentifierManager::hasThisLevelIdentifier(const string& identifier) {
	if (identifier.empty()) return false;
	if (mIdentifiers[mLevel].find(identifier) != mIdentifiers[mLevel].end()) {
		return true;
	}
	return mLevel == 2 && mIdentifiers[mLevel - 1].find(identifier) != mIdentifiers[mLevel - 1].end();
}

bool IdentifierManager::hasIdentifier(const string& identifier) {
	if (identifier.empty()) return false;
	for (int i = mLevel; i >= 0; --i) {
		auto& map = mIdentifiers[i];
		if (map.find(identifier) != map.end()) {
			return true;
		}
	}
	return false;
}

void IdentifierManager::advanceLevel() {
	++mLevel;
	mIdentifiers.resize(mLevel + 1, unordered_map<string, IdentifierType>());
	VariableManager::getInstance().advanceLevel();
	TypesManager::getInstance().advanceLevel();
}

void IdentifierManager::retreatLevel() {
	assert(mLevel > 0);
	--mLevel;
	mIdentifiers[mLevel].clear();
	VariableManager::getInstance().retreatLevel();
	TypesManager::getInstance().retreatLevel();
}

int IdentifierManager::getLevel() {
	return mLevel;
}

void IdentifierManager::init() {
	mLevel = 0;
	mIdentifiers.resize(1, unordered_map<string, IdentifierType>());
	mIdentifiers[0].clear();
}