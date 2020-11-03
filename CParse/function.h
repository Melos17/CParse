#ifndef FUNCTION_H
#define FUNCTION_H

#include <unordered_map>

#include "type.h"
#include "identifier.h"
#include "command.h"

using std::unordered_map;

struct Function {
	FunctionTypePtr type;
	PosType address;
	Function(FunctionTypePtr& type, PosType address) {
		this->type = type;
		this->address = address;
	}
};

class FunctionManager {
public:
	static FunctionManager& getInstance();
	
	void addFunction(const string& identifier, FunctionTypePtr& type, PosType pos);
	bool hasFunction(const string& identifier);
	Function getFunction(const string& identifier, vector<TypePtr>& types);

	bool checkParameterMatch(FunctionTypePtr funcType, vector<TypePtr>& types);
	
#ifdef _DEBUG
	void setFunctionPosition(PosType pos, const string& funcName);
	string getFunctionIfAtPos(PosType pos) const;
#endif
	
	void init();
	
private:
	FunctionManager() { }
	
	void addSystemFunction();
	
#ifdef _DEBUG
	unordered_map<PosType, string> mPositionAndFunction;
#endif
	unordered_map<string, vector<Function>> mFunction;
};

#endif
