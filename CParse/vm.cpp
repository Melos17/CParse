#include "vm.h"
#include "identifier.h"
#include "data.h"
#include "function.h"

#include <stdio.h>

bool VirtualMachine::sShowCommand = false;

PosType VirtualMachine::getMainPosition() {
	PosType address;
	try {
		vector<shared_ptr<Type>> standardParameterTypes;
		shared_ptr<Type> intType = make_shared<Type>(Type::SignedInt);
		shared_ptr<Type> charType = make_shared<Type>(Type::SignedChar);
		shared_ptr<Type> pcType = make_shared<PointerType>(charType);
		shared_ptr<Type> ppcType = make_shared<PointerType>(pcType);
		standardParameterTypes.emplace_back(intType);
		standardParameterTypes.emplace_back(ppcType);
		Function func = FunctionManager::getInstance().getFunction("main", standardParameterTypes);
		address = func.address;
	}
	catch (NoMatchCallException&) {
		vector<shared_ptr<Type>> emptyParameterTypes;
		Function func = FunctionManager::getInstance().getFunction("main", emptyParameterTypes);
		address = func.address;
	}
	return address;
}

BaseValueType VirtualMachine::run(int argc, const char** argv) {
	init();
	BaseValueType a = 0;

	PosType address = getMainPosition();
	mPc = mCode.size();
	stackPush((BaseValueType)argv);
	stackPush(argc);
	mCode.addJSR(address);
	
	// push main return value and call exit
	mCode.addPSH();
	mCode.addIMM(mCode.size() + 3);
	mCode.addCALL();
	mCode.addENT();
	mCode.addEXIT();

	while (1) {
		if (sShowCommand) {
			cout << mPc << " - ";
			printOp();
		}

		BaseValueType val = mCode.getCode(mPc++);
		switch (val) {
		case Command::LEA: {
			int pos = mBp + (int)mCode.getCode(mPc++);
			a = (BaseValueType)&mStack[pos];
			break;
		}
		case Command::IMM: {
			a = (BaseValueType)mCode.getCode(mPc++);
			break;
		}
		case Command::JMP: {
			mPc = (size_t)mCode.getCode(mPc);
			break;
		}
		case Command::CALL: {
			stackPush((BaseValueType)(mPc));
			mPc = (size_t)a;
			break;
		}
		case Command::JSR: {
			stackPush((BaseValueType)(mPc + 1));
			mPc = (size_t)mCode.getCode(mPc);
			break;
		}
		case Command::BZ: {
			mPc = (size_t)(a ? mPc + 1 : mCode.getCode(mPc));
			break;
		}
		case Command::BNZ: {
			mPc = (size_t)(a ? mCode.getCode(mPc) : mPc + 1);
			break;
		}
		case Command::ENT: {
			stackPush(mBp);
			mBp = mSp;
			break;
		}
		case Command::ADJ: {
			mSp = mSp - (int)mCode.getCode(mPc++);
			break;
		}
		case Command::LEV: {
			mSp = mBp;
			mBp = (int)stackPop();
			mPc = (size_t)stackPop();
			break;
		}
		case Command::L64: {
			a = *(int64 *)a;
			break;
		}
		case Command::L32: {
			a = *(int32 *)a;
			break;
		}
		case Command::L16: {
			a = *(int16 *)a;
			break;
		}
		case Command::L8: {
			a = *(int8 *)a;
			break;
		}
		case Command::S64: {
			*(int64 *)stackPop() = a;
			break;
		}
		case Command::S32: {
			a = *(int32 *)stackPop() = (int32)a;
			break;
		}
		case Command::S16: {
			a = *(int16 *)stackPop() = (int16)a;
			break;
		}
		case Command::S8: {
			a = *(int8 *)stackPop() = (int8)a;
			break;
		}
		case Command::T32: {
			a = (int32)a;
			break;
		}
		case Command::T16: {
			a = (int16)a;
			break;
		}
		case Command::T8: {
			a = (int8)a;
			break;
		}
		case Command::TU32: {
			a = (uint32)a;
			break;
		}
		case Command::TU16: {
			a = (uint16)a;
			break;
		}
		case Command::TU8: {
			a = (uint8)a;
			break;
		}
		case Command::PSH: {
			stackPush(a);
			break;
		}
		case Command::OR: {
			a = stackPop() | a;
			break;
		}
		case Command::XOR: {
			a = stackPop() ^ a;
			break;
		}
		case Command::AND: {
			a = stackPop() & a;
			break;
		}
		case Command::EQ: {
			a = stackPop() == a;
			break;
		}
		case Command::NE: {
			a = stackPop() != a;
			break;
		}
		case Command::LT: {
			a = stackPop() < a;
			break;
		}
		case Command::GT: {
			a = stackPop() > a;
			break;
		}
		case Command::LE: {
			a = stackPop() <= a;
			break;
		}
		case Command::GE: {
			a = stackPop() >= a;
			break;
		}
		case Command::LTU: {
			a = (uint64)stackPop() < (uint64)a;
			break;
		}
		case Command::GTU: {
			a = (uint64)stackPop() > (uint64)a;
			break;
		}
		case Command::LEU: {
			a = (uint64)stackPop() <= (uint64)a;
			break;
		}
		case Command::GEU: {
			a = (uint64)stackPop() >= (uint64)a;
			break;
		}
		case Command::SHL: {
			a = stackPop() << a;
			break;
		}
		case Command::SHR: {
			a = stackPop() >> a;
			break;
		}
		case Command::SHRU32: {
			a = (uint32)stackPop() >> a;
			break;
		}
		case Command::SHRU64: {
			a = (uint64)stackPop() >> a;
			break;
		}
		case Command::ADD: {
			a = stackPop() + a;
			break;
		}
		case Command::SUB: {
			a = stackPop() - a;
			break;
		}
		case Command::MUL: {
			a = stackPop() * a;
			break;
		}
		case Command::DIV: {
			a = stackPop() / a;
			break;
		}
		case Command::MOD: {
			a = stackPop() % a;
			break;
		}
		case Command::DIVU32: {
			a = (uint32)stackPop() / a;
			break;
		}
		case Command::MODU32: {
			a = (uint32)stackPop() % a;
			break;
		}
		case Command::DIVU64: {
			a = (uint64)stackPop() / a;
			break;
		}
		case Command::MODU64: {
			a = (uint64)stackPop() % a;
			break;
		}
		case Command::OPEN: {
			a = (BaseValueType)fopen((const char*)stackTop(2), (const char*)stackTop(3));
			break;
		}
		case Command::READ: {
			a = fread((char*)stackTop(3), 1, (size_t)stackTop(4), (FILE*)stackTop(2));
			break;
		}
		case Command::CLOS: {
			a = fclose((FILE*)stackTop(2));
			break;
		}
		case Command::PRTF: {
			printf((const char*)stackTop(0), (int)stackTop(1), (int)stackTop(2), (int)stackTop(3), 0, 0, 0);
				//stackTop(1), stackTop(2), stackTop(3), stackTop(4));
			break;
		}
		case Command::MALC: {
			a = (BaseValueType)malloc((size_t)stackTop(2));
			break;
		}
		case Command::FREE: {
			free((void*)stackTop(2));
			break;
		}
		case Command::MSET: {
			a = (BaseValueType)memset((void*)stackTop(2), (int)stackTop(3), (size_t)stackTop(4));
			break;
		}
		case Command::MCMP: {
			a = memcmp((const void*)stackTop(2), (const void*)stackTop(3), (size_t)stackTop(4));
			break;
		}
		case Command::EXIT: {
			a = stackTop(2);
			return a;
		}
		default: {
			cout << "Unknown cmd:" << val << endl;
			assert(false);
		}
		}
		// cout << a << "--";
		// printStack();
	}
}

void VirtualMachine::init() {
	mStack.resize(spSize);
	mBp = mSp = 0;
}

void VirtualMachine::printOp() {
	const string op[] = {
		"LEA", "IMM", "JMP", "CALL", "JSR", "BZ", "BNZ", "ENT", "ADJ", "LEV", "L32", "L8", "S32", "S8", "PSH",
		"OR", "XOR", "AND", "EQ", "NE", "LT", "GT", "LE", "GE", "SHL", "SHR", "ADD", "SUB", "MUL", "DIV", "MOD",
		"OPEN", "READ", "CLOS", "PRTF", "MALC", "FREE", "MSET", "MCMP", "EXIT",
		"L64", "L16", "S64", "S16", "T8", "T16", "T32", "TU8", "TU16", "TU32",
		"LTU", "GTU", "LEU", "GEU", "SHRU32","DIVU32", "MODU32", "SHRU64","DIVU64", "MODU64",
		"BREAK", "CONTINUE",
	};
	std::cout << op[mCode.getCode(mPc)];
	switch (mCode.getCode(mPc)) {
	case Command::JMP:
	case Command::JSR:
	case Command::BZ:
	case Command::BNZ:
	case Command::LEA:
	case Command::IMM:
	case Command::ADJ: {
		std::cout << " " << mCode.getCode(mPc + 1);
		break;
	}
	}
	std::cout << std::endl;
}

void VirtualMachine::stackPush(BaseValueType val) {
	mStack[mSp++] = val;
}

BaseValueType VirtualMachine::stackPop() {
	assert(mSp > 0);
	BaseValueType ret = mStack[--mSp];
	return ret;
}

BaseValueType VirtualMachine::stackTop(int pos) {
	assert(mSp - pos - 1 >= 0);
	if (mSp - pos - 1 < 0) {
		return 0;
	}
	BaseValueType ret = mStack[mSp - pos - 1];
	return ret;
}

void VirtualMachine::printStack(bool onlyCurrentStack) {
	cout << "       ";
	for (int i = (onlyCurrentStack ? mBp : 0); i < mSp; ++i) {
		cout << mStack[i] << ' ';
	}
	cout << endl;
}
