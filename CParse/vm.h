#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <iostream>
#include <string>

#include "command.h"

using std::cout;
using std::endl;

class VirtualMachine {
public:
	VirtualMachine(Command& code) : mCode(code) {
		if(sShowCommand) {
			cout << mCode.toString() << endl;
		}
	}

	BaseValueType run(int argc = 0, const char** argv = nullptr);
	
	void setShowCommandFlag(bool flag) {
		sShowCommand = flag;
	}

private:
	void init();
	
	PosType getMainPosition();

	void stackPush(BaseValueType val);
	BaseValueType stackPop();
	BaseValueType stackTop(int pos = 0);
	
	void printOp();
	void printStack(bool onlyCurrentStack = true);

	BaseValueType getNextCode() {
		return mCode.getCode(mPc++);
	}

	int mSp;
	int mBp;
	vector<BaseValueType> mStack;

	size_t mPc;
	Command& mCode;
	const size_t spSize = 256 * 1024;
	
	static bool sShowCommand;
};

#endif