#ifndef VARIABLE_H
#define VARIABLE_H

#include <vector>
#include <unordered_map>
#include <memory>

#include "command.h"
#include "type.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::unordered_map;

struct Variable {
	enum StoreType {
		Global,
		Local,
	};
	Variable(StoreType storeType, TypePtr type, string identifier) {
		this->storeType = storeType;
		this->type = type;
		this->identifier = identifier;
	}
	
	StoreType storeType;
	TypePtr type;
	string identifier;
	BaseValueType value;
	
	CommandPtr generateLoadCode();
};

typedef shared_ptr<Variable> VariablePtr;

class VariableManager {
public:
	static VariableManager& getInstance();
	
	VariablePtr addVariable(TypePtr type, const string& identifier, bool isStatic = false);
	VariablePtr addVariable(TypePtr type, const string& identifier, BaseValueType value);
	bool hasVariable(const string& identifier);
	VariablePtr getVariable(const string& identifier);
	
	size_t getStackSizeThisLevel();
	size_t getStackSizeNextLevel();
	
	void advanceLevel();
	void retreatLevel();
	void adjustParameters();
	
	void init() {
		mLevel = 0;
		mStackSize.clear();
		mVariables.resize(1, unordered_map<string, VariablePtr>());
		mVariables[0].clear();
	}
	
private:
	VariableManager() { init(); }

	int mLevel;
	vector<unordered_map<string, VariablePtr>> mVariables;
	unordered_map<int, int> mStackSize;
};

#endif