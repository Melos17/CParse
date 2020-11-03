#include "command.h"
#include "function.h"

CommandPtr gCode;

void Command::addCodes(shared_ptr<Command> commandPtr) {
	if (!commandPtr) return;
	addCodes(*commandPtr);
}

void Command::addCodes(Command& command) {
	// adjust pos
	size_t n = size();
	for (size_t i = 0; i < command.mCommands.size(); ++i) {
		switch (command.mCommands[i]) {
		case JMP:
		case JSR:
		case BZ:
		case BNZ:
			command.mCommands[++i] += n;
			break;
		case LEA:
		case IMM:
		case ADJ:
		case BREAK:
		case CONTINUE:
			++i;
			break;
		}
	}
	mCommands.insert(mCommands.end(), command.mCommands.begin(), command.mCommands.end());
}

void Command::addCodesToBegin(shared_ptr<Command> commandPtr) {
	if (!commandPtr) return;
	addCodesToBegin(*commandPtr);
}

void Command::addCodesToBegin(Command command) {
	// adjust pos
	size_t n = command.mCommands.size();
	for (size_t i = 0; i < mCommands.size(); ++i) {
		switch (mCommands[i]) {
		case JMP:
		case JSR:
		case BZ:
		case BNZ:
			mCommands[++i] += n;
			break;
		case LEA:
		case IMM:
		case ADJ:
		case BREAK:
		case CONTINUE:
			++i;
			break;
		}
	}
	mCommands.insert(mCommands.begin(), command.mCommands.begin(), command.mCommands.end());
}

BaseValueType Command::popCommand() {
	assert(mCommands.size() > 0);
	BaseValueType ret = mCommands.back();
	mCommands.pop_back();
	return ret;
}

BaseValueType Command::back(size_t pos) const {
	assert(mCommands.size() >= pos + 1);
	BaseValueType ret = mCommands[mCommands.size() - pos - 1];
	return ret;
}

string Command::toString(bool showLine) const {
	string ret;
	const string op[] = {
			"LEA", "IMM", "JMP", "CALL", "JSR", "BZ", "BNZ", "ENT", "ADJ", "LEV", "L32", "L8", "S32", "S8", "PSH",
			"OR", "XOR", "AND", "EQ", "NE", "LT", "GT", "LE", "GE", "SHL", "SHR", "ADD", "SUB", "MUL", "DIV", "MOD",
			"OPEN", "READ", "CLOS", "PRTF", "MALC", "FREE", "MSET", "MCMP", "EXIT",
			"L64", "L16", "S64", "S16", "T8", "T16", "T32", "TU8", "TU16", "TU32",
			"LTU", "GTU", "LEU", "GEU", "SHRU32","DIVU32", "MODU32", "SHRU64","DIVU64", "MODU64",
			"BREAK", "CONTINUE",
			"IMM_PC", "IMM_DATA",
	};
	size_t s = size();
	for (size_t i = 0; i < s; ++i) {
#ifdef _DEBUG
		string funcName = FunctionManager::getInstance().getFunctionIfAtPos(i);
		if (!funcName.empty()) ret += "--" + funcName + ":\n";
#endif
		ret += to_string(i) + ": ";
		ret += op[mCommands[i]];
		switch (mCommands[i]) {
		case JMP:
		case JSR:
		case BZ:
		case BNZ:
		case LEA:
		case IMM:
		case ADJ:
		case BREAK:
		case CONTINUE:
			ret += ' ';
			ret += to_string(mCommands[++i]);
			break;
		}
		ret += '\n';
	}
	return ret;
}

void Command::setBreakPosition(PosType pos) {
	size_t s = size();
	for (size_t i = 0; i < s; ++i) {
		switch (mCommands[i]) {
		case JMP:
		case JSR:
		case BZ:
		case BNZ:
		case LEA:
		case IMM:
		case ADJ:
		case CONTINUE:
			++i;
			break;
		case BREAK:
			mCommands[i] = Command::JMP;
			mCommands[++i] = pos;
			break;
		}
	}
}

void Command::setContinuePosition(PosType pos) {
	size_t s = size();
	for (size_t i = 0; i < s; ++i) {
		switch (mCommands[i]) {
		case JMP:
		case JSR:
		case BZ:
		case BNZ:
		case LEA:
		case IMM:
		case ADJ:
		case BREAK:
			++i;
			break;
		case CONTINUE:
			mCommands[i] = Command::JMP;
			mCommands[++i] = pos;
			break;
		}
	}
}

bool Command::hasBreakOrContinue() const {
	size_t s = size();
	for (size_t i = 0; i < s; ++i) {
		switch (mCommands[i]) {
		case JMP:
		case JSR:
		case BZ:
		case BNZ:
		case LEA:
		case IMM:
		case ADJ:
			++i;
			break;
		case BREAK:
		case CONTINUE:
			return true;
		}
	}
	return false;
}
	