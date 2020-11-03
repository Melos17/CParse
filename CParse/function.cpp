#include "function.h"

FunctionManager& FunctionManager::getInstance() {
	static FunctionManager sFunctionManager;
	return sFunctionManager;
}


void FunctionManager::addFunction(const string& identifier, FunctionTypePtr& type, PosType pos) {
	IdentifierManager::getInstance().addFunction(identifier);

	vector<Function>& functions = mFunction[identifier];
	for (Function& existedFunc : functions) {
		if (type->parameterType == existedFunc.type->parameterType) {
			throw RedefineException();
		}
	}
	functions.emplace_back(type, pos);
}

bool FunctionManager::hasFunction(const string& identifier) {
	return mFunction.find(identifier) != mFunction.end();
}

// allow implicit match
bool FunctionManager::checkParameterMatch(FunctionTypePtr funcType, vector<TypePtr>& types) {
	for (size_t i = 0; i < types.size(); ++i) {
		if (!types[i]->canImplicitConverseTo(*funcType->parameterType[i])) {
			return false;
		}
	}
	return true;
}

Function FunctionManager::getFunction(const string& identifier, vector<TypePtr>& types) {
	size_t size = types.size();
	if (mFunction.find(identifier) == mFunction.end()) {
		throw NoMatchCallException();
	}

	vector<Function>& functions = mFunction[identifier];
	vector<Function> implicitMatch;
	for (Function& func : functions) {
		if (size != func.type->parameterType.size()) {
			continue;
		}
		// complete match
		if (types == func.type->parameterType) {
			return func;
		}

		if (checkParameterMatch(func.type, types)) {
			implicitMatch.emplace_back(func);
		}
	}

	if (implicitMatch.size() == 0) {
		throw NoMatchCallException();
	}
	else if (implicitMatch.size() > 1) {
		throw AmbiguousCallException();
	}
	return implicitMatch[0];
}

#ifdef _DEBUG
void FunctionManager::setFunctionPosition(PosType pos, const string& funcName) {
	mPositionAndFunction[pos] = funcName;
}
string FunctionManager::getFunctionIfAtPos(PosType pos) const {
	auto it = mPositionAndFunction.find(pos);
	if (it != mPositionAndFunction.end()) {
		return it->second;
	}
	else {
		return string();
	}
}
#endif

void FunctionManager::init() {
#ifdef _DEBUG
	mPositionAndFunction.clear();
#endif
	mFunction.clear();
	addSystemFunction();
}

void FunctionManager::addSystemFunction() {
	Command command;

	TypePtr constCharType = make_shared<Type>(Type::SignedChar);
	constCharType->isConst = true;
	TypePtr charType = make_shared<Type>(Type::SignedChar);
	TypePtr constCharPtrType = make_shared<PointerType>(constCharType);
	TypePtr charPtrType = make_shared<PointerType>(charType);
	TypePtr constVoidType = make_shared<Type>(Type::Void);
	constVoidType->isConst = true;
	TypePtr constVoidPtrType = make_shared<PointerType>(constVoidType);
	TypePtr voidType = make_shared<Type>(Type::Void);
	TypePtr voidPtrType = make_shared<PointerType>(voidType);
	TypePtr returnType;
	vector<TypePtr> parameterType;
	FunctionTypePtr funcType;

	// int open(const char *pathname, const char * flags);
	command.addENT();
	command.addOPEN();
	command.addLEV();
	returnType = SignedIntType;
	parameterType.emplace_back(constCharPtrType);
	parameterType.emplace_back(constCharPtrType);
	funcType = make_shared<FunctionType>(returnType, parameterType);
	Function funcOpen(funcType, 0);
	mFunction["open"].emplace_back(funcOpen);

	// unsigned int read(int fd, void *buf, unsigned int count);
	command.addENT();
	command.addREAD();
	command.addLEV();
	returnType = UnsignedIntType;
	parameterType.clear();
	parameterType.emplace_back(SignedIntType);
	parameterType.emplace_back(voidPtrType);
	parameterType.emplace_back(UnsignedIntType);
	funcType = make_shared<FunctionType>(returnType, parameterType);
	Function funcRead(funcType, 3);
	mFunction["read"].emplace_back(funcRead);

	// int close(int fd);
	command.addENT();
	command.addCLOS();
	command.addLEV();
	returnType = SignedIntType;
	parameterType.clear();
	parameterType.emplace_back(SignedIntType);
	funcType = make_shared<FunctionType>(returnType, parameterType);
	Function funcClose(funcType, 6);
	mFunction["close"].emplace_back(funcClose);

	// void *malloc(unsigned int size);
	command.addENT();
	command.addMALC();
	command.addLEV();
	returnType = voidPtrType;
	parameterType.clear();
	parameterType.emplace_back(UnsignedIntType);
	funcType = make_shared<FunctionType>(returnType, parameterType);
	Function funcMalloc(funcType, 9);
	mFunction["malloc"].emplace_back(funcMalloc);

	// void free(void *ptr);
	command.addENT();
	command.addFREE();
	command.addLEV();
	returnType = voidType;
	parameterType.clear();
	parameterType.emplace_back(voidPtrType);
	funcType = make_shared<FunctionType>(returnType, parameterType);
	Function funcFree(funcType, 12);
	mFunction["free"].emplace_back(funcFree);

	// void *memset(void *str, int c, unsigned int n);
	command.addENT();
	command.addMSET();
	command.addLEV();
	returnType = voidPtrType;
	parameterType.clear();
	parameterType.emplace_back(voidPtrType);
	parameterType.emplace_back(SignedIntType);
	parameterType.emplace_back(UnsignedIntType);
	funcType = make_shared<FunctionType>(returnType, parameterType);
	Function funcMemset(funcType, 15);
	mFunction["memset"].emplace_back(funcMemset);

	// int memcmp(const void *str1, const void *str2, size_t n);
	command.addENT();
	command.addMCMP();
	command.addLEV();
	returnType = SignedIntType;
	parameterType.clear();
	parameterType.emplace_back(constVoidPtrType);
	parameterType.emplace_back(constVoidPtrType);
	parameterType.emplace_back(UnsignedIntType);
	funcType = make_shared<FunctionType>(returnType, parameterType);
	Function funcMemcmp(funcType, 18);
	mFunction["memcmp"].emplace_back(funcMemcmp);

	// 	void exit(int status);
	command.addENT();
	command.addEXIT();
	command.addLEV();
	returnType = voidType;
	parameterType.clear();
	parameterType.emplace_back(SignedIntType);
	funcType = make_shared<FunctionType>(returnType, parameterType);
	Function funcExit(funcType, 21);
	mFunction["exit"].emplace_back(funcExit);
	gCode->addCodes(command);

	// handle in generatePostfixExpression specially
	mFunction["printf"];
}
