#ifndef COMMAND_H
#define COMMAND_H

#include <assert.h>
#include <string>
#include <vector>
#include <memory>

#include "base.h"
#include "type.h"

using std::to_string;
using std::shared_ptr;
using std::vector;
using std::string;
using std::make_shared;

class Command : public Polymorphism {
public:

	enum OpType {
		LEA, IMM, JMP, CALL, JSR, BZ, BNZ, ENT, ADJ, LEV, L32, L8, S32, S8, PSH,
		OR, XOR, AND, EQ, NE, LT, GT, LE, GE, SHL, SHR, ADD, SUB, MUL, DIV, MOD,
		OPEN, READ, CLOS, PRTF, MALC, FREE, MSET, MCMP, EXIT,
		L64, L16, S64, S16, T8, T16, T32, TU8, TU16, TU32,
		LTU, GTU, LEU, GEU, SHRU32, DIVU32, MODU32, SHRU64, DIVU64, MODU64,
		// temp op
		BREAK, CONTINUE,
	};
	
	Command() { mType = make_shared<Type>(Type::Unknown); }
	
	void addCodes(shared_ptr<Command> commandPtr);
	void addCodes(Command& command);
	
	void addCodesToBegin(shared_ptr<Command> commandPtr);
	void addCodesToBegin(Command command);
	
	// load local address
	void addLEA(BaseValueType value) {
		mCommands.emplace_back(LEA);
		mCommands.emplace_back(value);
	}
	
	// load global address or immediate
	void addIMM(BaseValueType value) {
		mCommands.emplace_back(IMM);
		mCommands.emplace_back(value);
	}
	
	void addJMP(PosType pos) {
		mCommands.emplace_back(JMP);
		mCommands.emplace_back(pos);
	}

	void addCALL() {
		mCommands.emplace_back(CALL);
	}
	
	// branch if zero
	void addBZ(PosType pos) {
		mCommands.emplace_back(BZ);
		mCommands.emplace_back(pos);
	}
	
	// branch if not zerobranch if not zero
	void addBNZ(PosType pos) {
		mCommands.emplace_back(BNZ);
		mCommands.emplace_back(pos);
	}

	// save next pc pos to stack, set pc to function pos
	// push(pc+1); pc = *pc
	void addJSR(PosType functionPos) {
		mCommands.emplace_back(JSR);
		mCommands.emplace_back(functionPos);
	}
	
	// save old bp, set sp to new bp
	// push(bp), bp=sp
	void addENT() {
		mCommands.emplace_back(ENT);
	}
	
	// used for adjust stack for paraments
	// sp += n
	void addADJ(BaseValueType paramentCount) {
		mCommands.emplace_back(ADJ);
		mCommands.emplace_back(paramentCount);
	}
	
	// leave function directly: set bp to sp, set origin bp and origin pc
	// sp=bp; bp=sp.pop; pc=sp.pop
	void addLEV() {
		mCommands.emplace_back(LEV);
	}

	void addL8() {
		mCommands.emplace_back(L8);
	}

	void addL16() {
		mCommands.emplace_back(L16);
	}
	
	void addL32() {
		mCommands.emplace_back(L32);
	}
	
	void addL64() {
		mCommands.emplace_back(L64);
	}
	
	void addS8() {
		mCommands.emplace_back(S8);
	}
	
	void addS16() {
		mCommands.emplace_back(S16);
	}

	void addS32() {
		mCommands.emplace_back(S32);
	}
	
	void addS64() {
		mCommands.emplace_back(S64);
	}

	void addT8() {
		mCommands.emplace_back(T8);
	}

	void addT16() {
		mCommands.emplace_back(T16);
	}

	void addT32() {
		mCommands.emplace_back(T32);
	}

	void addTU8() {
		mCommands.emplace_back(TU8);
	}

	void addTU16() {
		mCommands.emplace_back(TU16);
	}

	void addTU32() {
		mCommands.emplace_back(TU32);
	}
	
	void addPSH() {
		mCommands.emplace_back(PSH);
	}
	
	void addOR() {
		mCommands.emplace_back(OR);
	}
	
	void addXOR() {
		mCommands.emplace_back(XOR);
	}
	
	void addAND() {
		mCommands.emplace_back(AND);
	}
	
	void addEQ() {
		mCommands.emplace_back(EQ);
	}
	
	void addNE() {
		mCommands.emplace_back(NE);
	}
	
	void addLT() {
		mCommands.emplace_back(LT);
	}
	
	void addGT() {
		mCommands.emplace_back(GT);
	}
	
	void addLE() {
		mCommands.emplace_back(LE);
	}
	
	void addGE() {
		mCommands.emplace_back(GE);
	}
	
	void addSHL() {
		mCommands.emplace_back(SHL);
	}
	
	void addSHR() {
		mCommands.emplace_back(SHR);
	}
	
	void addADD() {
		mCommands.emplace_back(ADD);
	}
	
	void addSUB() {
		mCommands.emplace_back(SUB);
	}
	
	void addMUL() {
		mCommands.emplace_back(MUL);
	}
	
	void addDIV() {
		mCommands.emplace_back(DIV);
	}
	
	void addMOD() {
		mCommands.emplace_back(MOD);
	}
	
	void addLTU() {
		mCommands.emplace_back(LTU);
	}
	
	void addGTU() {
		mCommands.emplace_back(GTU);
	}
	
	void addLEU() {
		mCommands.emplace_back(LEU);
	}
	
	void addGEU() {
		mCommands.emplace_back(GEU);
	}
	
	void addSHRU32() {
		mCommands.emplace_back(SHRU32);
	}
	
	void addDIVU32() {
		mCommands.emplace_back(DIVU32);
	}
	
	void addMODU32() {
		mCommands.emplace_back(MODU32);
	}

	void addSHRU64() {
		mCommands.emplace_back(SHRU64);
	}

	void addDIVU64() {
		mCommands.emplace_back(DIVU64);
	}

	void addMODU64() {
		mCommands.emplace_back(MODU64);
	}
	
	void addBREAK() {
		mCommands.emplace_back(BREAK);
		mCommands.emplace_back(0);
	}
	
	void addCONTINUE() {
		mCommands.emplace_back(CONTINUE);
		mCommands.emplace_back(0);
	}

	void addOPEN() {
		mCommands.emplace_back(OPEN);
	}

	void addREAD() {
		mCommands.emplace_back(READ);
	}

	void addCLOS() {
		mCommands.emplace_back(CLOS);
	}

	void addPRTF() {
		mCommands.emplace_back(PRTF);
	}

	void addMALC() {
		mCommands.emplace_back(MALC);
	}

	void addFREE() {
		mCommands.emplace_back(FREE);
	}

	void addMSET() {
		mCommands.emplace_back(MSET);
	}

	void addMCMP() {
		mCommands.emplace_back(MCMP);
	}
	
	void addEXIT() {
		mCommands.emplace_back(EXIT);
	}

	void addRawValue(BaseValueType value) {
		mCommands.emplace_back(value);
	}
	
	BaseValueType popCommand();
	
	BaseValueType back(size_t pos = 0) const;
	
	size_t size() const { return mCommands.size(); }

	string toString(bool showLine = false) const;

	BaseValueType getCode(size_t size) {
		return mCommands[size];
	}
	
	void setType(shared_ptr<Type> type) {
		mType = type;
	}
	
	shared_ptr<Type> getType() {
		return mType;
	}
	
	void setBreakPosition(PosType pos);
	
	void setContinuePosition(PosType pos);
	
	bool hasBreakOrContinue() const;
	
private:
	vector<BaseValueType> mCommands;
	shared_ptr<Type> mType;
};

typedef shared_ptr<Command> CommandPtr;
extern CommandPtr gCode;

#endif