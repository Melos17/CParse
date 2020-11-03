#ifndef REDUCE_EXPRESSION_H
#define REDUCE_EXPRESSION_H

#include "reduce.h"

// unary_expression : 
// postfix_expression
CommandPtr generateUnaryExpression(PostfixExpressionPtr unit1) {
	return unit1->command;
}

// unary_operator cast_expression
// INC_OP unary_expression
// DEC_OP unary_expression
//	SIZEOF unary_expression 
CommandPtr generateUnaryExpression(TerminalUnitPtr unit1, CommandPtr unit2) {
	switch (unit1->type) {
	case '&': {
		BaseValueType val = unit2->popCommand();
		// check whether l-value
		if (val != Command::L8 && val != Command::L16 && val != Command::L32 && val != Command::L64) {
			throw NoLeftValueException();
		}
		break;
	}
	case '*': {
		shared_ptr<PointerType> type = dynamic_pointer_cast<PointerType>(unit2->getType());
		assert(type);
		switch (type->pointType->size()) {
		case 1:
			unit2->addL8();
			break;
		case 2:
			unit2->addL16();
			break;
		case 4:
			unit2->addL32();
			break;
		case 8:
			unit2->addL64();
			break;
		default:
			assert(false);
		}

		if (type->pointType->baseType == Type::Array) {
			shared_ptr<ArrayType> arrayType = dynamic_pointer_cast<ArrayType>(type->pointType);
			shared_ptr<Type> pointerType = make_shared<PointerType>(arrayType->pointType);
			unit2->setType(pointerType);
		}
		else {
			unit2->setType(type->pointType);
		}
		break;
	}
	case '+':
		break;
	case '-':
		unit2->addPSH();
		unit2->addIMM(-1);
		unit2->addMUL();
		break;
	case '~':
		unit2->addPSH();
		unit2->addIMM(-1);
		unit2->addXOR();
		break;
	case '!':
		unit2->addPSH();
		unit2->addIMM(0);
		unit2->addEQ();
		break;
	case CWord::INC_OP:
	case CWord::DEC_OP: {
		BaseValueType op = unit2->popCommand();
		unit2->addPSH();
		unit2->addRawValue(op);

		unit2->addPSH();
		if (unit2->getType()->baseType == Type::Pointer) {
			shared_ptr<PointerType> pointer = dynamic_pointer_cast<PointerType>(unit2->getType());
			unit2->addIMM(pointer->pointType->size());
		}
		else {
			unit2->addIMM(1);
		}

		if (unit1->type == CWord::INC_OP) {
			unit2->addADD();
		}
		else {
			unit2->addSUB();
		}

		switch (op) {
		case Command::L8:
			unit2->addS8();
			break;
		case Command::L16:
			unit2->addS16();
			break;
		case Command::L32:
			unit2->addS32();
			break;
		case Command::L64:
			unit2->addS64();
			break;
		default:
			throw NoLeftValueException();
		}
		break;
	}
	case CWord::SIZEOF: {
		unit2->addIMM(unit2->getType()->size());
		unit2->setType(SignedIntType);
		break;
	}
	}
	return unit2;
}

// SIZEOF '(' type_name ')'
CommandPtr generateUnaryExpression(TerminalUnitPtr unit1, TerminalUnitPtr  unit2, TypeNamePtr unit3, TerminalUnitPtr unit4) {
	CommandPtr code = make_shared<Command>();
	code->addIMM(unit3->type->size());
	code->setType(SignedIntType);
	return code;
}


// postfix_expression : 
// primary_expression
PostfixExpressionPtr generatePostfixExpression(PrimaryExpressionPtr unit1) {
	PostfixExpressionPtr postfixExpression = make_shared<PostfixExpression>();
	postfixExpression->command = unit1->command;
	postfixExpression->functionIdentifier = unit1->functionIdentifier;
	return postfixExpression;
}

// postfix_expression '[' expression ']'
PostfixExpressionPtr generatePostfixExpression(PostfixExpressionPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3, TerminalUnitPtr unit4) {
	CommandPtr command = unit1->command;
	size_t pointSize;
	implicitConverseType(command, unit3);

	BaseValueType originType = command->getType()->baseType;
	assert(originType == Type::Pointer || originType == Type::Array);
	if (originType == Type::Array) {
		shared_ptr<ArrayType> type = dynamic_pointer_cast<ArrayType>(command->getType());
		command->setType(type->pointType);
		pointSize = type->pointType->size();
	}
	else {
		shared_ptr<PointerType> type = dynamic_pointer_cast<PointerType>(command->getType());
		command->setType(type->pointType);
		pointSize = type->pointType->size();
	}

	if (pointSize > 1) {
		unit3->addPSH();
		unit3->addIMM(pointSize);
		unit3->addMUL();
	}

	command->addPSH();
	command->addCodes(unit3);
	command->addADD();

	if (command->getType()->baseType != Type::Array) {
		switch (command->getType()->size()) {
		case 1:
			command->addL8();
			break;
		case 2:
			command->addL16();
			break;
		case 4:
			command->addL32();
			break;
		case 8:
			command->addL64();
			break;
		default:
			assert(false);
		}
	}

	return unit1;
}

// postfix_expression '(' ')'
// 	postfix_expression '.' IDENTIFIER
// 	postfix_expression PTR_OP IDENTIFIER
PostfixExpressionPtr generatePostfixExpression(PostfixExpressionPtr unit1, TerminalUnitPtr unit2, TerminalUnitPtr unit3) {
	switch (unit2->type) {
	case '(': {
		// need to consider func ptr later
		if (unit1->command->size() > 0) {
			BaseValueType cmd = unit1->command->popCommand();
		}
		else {
			vector<shared_ptr<Type>> emptyTypes;
			Function func = FunctionManager::getInstance().getFunction(unit1->functionIdentifier, emptyTypes);
			unit1->command->addIMM(func.address);
			unit1->command->addCALL();
			unit1->command->setType(func.type->returnType);
		}
		break;
	}
	case CWord::PTR_OP: {
		CommandPtr command = unit1->command;
		assert(command->getType()->baseType == Type::Pointer);
		shared_ptr<PointerType> type = dynamic_pointer_cast<PointerType>(command->getType());
		command->addL64(); // 64bits
		command->setType(type->pointType);
	}
	case '.': {
		string& identifier = unit3->word;
		CommandPtr command = unit1->command;
		assert(command->getType()->baseType == Type::Struct || command->getType()->baseType == Type::Union);
		BaseValueType op = command->popCommand();
		assert(op == Command::L8 || op == Command::L16 || op == Command::L32 || op == Command::L64);
		shared_ptr<StructUnionType> type = dynamic_pointer_cast<StructUnionType>(command->getType());
		bool found = false;
		for (StructUnionMember& member : type->members) {
			if (member.identifier == identifier) {
				found = true;
				command->addPSH();
				command->addIMM(member.address);
				command->addADD();
				command->setType(member.type);
				switch (member.type->size()) {
				case 1:
					command->addL8();
					break;
				case 2:
					command->addL16();
					break;
				case 4:
					command->addL32();
					break;
				case 8:
					command->addL64();
					break;
				default:
					assert(false);
				}
				break;
			}
		}
		if (!found) {
			throw UnknownIdentifierException();
		}
		break;
	}
	}
	return unit1;
}

// postfix_expression INC_OP
// 	postfix_expression DEC_OP
PostfixExpressionPtr generatePostfixExpression(PostfixExpressionPtr unit1, TerminalUnitPtr unit2) {
	CommandPtr command = unit1->command;
	BaseValueType op = command->popCommand();
	command->addPSH();
	switch (op) {
	case Command::L8:
		command->addL8();
		break;
	case Command::L16:
		command->addL16();
		break;
	case Command::L32:
		command->addL32();
		break;
	case Command::L64:
		command->addL64();
		break;
	default:
		throw NoLeftValueException();
	}

	command->addPSH();
	if (command->getType()->baseType == Type::Pointer) {
		shared_ptr<PointerType> pointer = dynamic_pointer_cast<PointerType>(command->getType());
		command->addIMM(pointer->pointType->size());
	}
	else {
		command->addIMM(1);
	}

	if (unit2->type == CWord::INC_OP) {
		command->addADD();
	}
	else {
		command->addSUB();
	}

	switch (op) {
	case Command::L8:
		command->addS8();
		break;
	case Command::L16:
		command->addS16();
		break;
	case Command::L32:
		command->addS32();
		break;
	case Command::L64:
		command->addS64();
		break;
	}

	command->addPSH();
	if (command->getType()->baseType == Type::Pointer) {
		shared_ptr<PointerType> pointer = dynamic_pointer_cast<PointerType>(command->getType());
		command->addIMM(pointer->pointType->size());
	}
	else {
		command->addIMM(1);
	}

	if (unit2->type == CWord::INC_OP) {
		command->addSUB();
	}
	else {
		command->addADD();
	}

	return unit1;
}

// postfix_expression '(' argument_expression_list ')'
PostfixExpressionPtr generatePostfixExpression(PostfixExpressionPtr unit1, TerminalUnitPtr unit2, ArgumentExpressionListPtr unit3, TerminalUnitPtr unit4) {
	CommandPtr command = unit1->command;
	// need to consider func ptr later
	if (command->size() > 0) {
		BaseValueType cmd = command->popCommand();
		assert(false);
	}
	else {
		// no allow ... parameter, so need to handle printf specially now
		if (unit1->functionIdentifier == "printf") {
			CommandPtr parameterPushCode = make_shared<Command>();
			for (int i = unit3->parameters.size() - 1; i >= 0; --i) {
				CommandPtr cmd = unit3->parameters[i];
				cmd->addPSH();
				parameterPushCode->addCodes(cmd);
			}
			command->addCodesToBegin(parameterPushCode);
			command->addPRTF();
			command->addADJ(unit3->parameters.size());
		}
		else {
			vector<shared_ptr<Type>> types;
			for (CommandPtr cmd : unit3->parameters) {
				types.emplace_back(cmd->getType());
			}
			// need to consider func ptr later
			if (command->size() > 0) {
				BaseValueType cmd = command->popCommand();
			}
			else {
				Function func = FunctionManager::getInstance().getFunction(unit1->functionIdentifier, types);
				CommandPtr parameterPushCode = make_shared<Command>();
				for (int i = unit3->parameters.size() - 1; i >= 0; --i) {
					CommandPtr cmd = unit3->parameters[i];
					shared_ptr<Type> type = func.type->parameterType[i];
					converseType(cmd, type);
					cmd->addPSH();
					parameterPushCode->addCodes(cmd);
				}
				command->addIMM(func.address);
				command->addCALL();
				command->addADJ(unit3->parameters.size());
				command->addCodesToBegin(parameterPushCode);
				command->setType(func.type->returnType);
			}
		}
	}
	return unit1;
}

// constant_expression :
// CONSTANT
CommandPtr generateConstantExpression(TerminalUnitPtr unit1) {
	CommandPtr code = make_shared<Command>();
	string& constant = unit1->word;
	if (constant.front() == '\'' && constant.back() == '\'') {
		char c;
		if (constant[1] == '\\') {
			assert(constant.size() == 4);
			switch (constant[2]) {
			case 'n':
				c = '\n';
				break;
			case 't':
				c = '\t';
				break;
			case 'v':
				c = '\v';
				break;
			case 'f':
				c = '\f';
				break;
			case '0':
				c = 0;
				break;
			case '\'':
				c = '\'';
				break;
			case '\\':
				c = '\\';
				break;
			}
		} else {
			assert(constant.size() == 3);
			c = constant[1];
		}
		code->addIMM(c);
		code->setType(SignedCharType);
	}
	else {
		int base = 10;
		if (constant.find("0x") == 0 || constant.find("0X") == 0) {
			base = 16;
		}
		else if (constant.find("0") == 0) {
			base = 8;
		}
		int i = 0;
		try {
			i = std::stoi(constant, nullptr, base);
		}
		catch (...) {
		}
		long long l = std::stoll(constant, nullptr, base);
		if (i != l) {
			code->addIMM(l);
			code->setType(SignedLongLongType);
		}
		else {
			code->addIMM(i);
			code->setType(SignedIntType);
		}
	}
	return code;
}


void replaceStr(string& source, const string& from, const string& to) {
	auto n = source.find(from);
	while (n != string::npos) {
		source.replace(n, from.size(), to);
		n = source.find(from);
	}
}

// primary_expression : 
// IDENTIFIER
// 	CONSTANT
// 	STRING_LITERAL
PrimaryExpressionPtr generatePrimaryExpression(TerminalUnitPtr unit1) {
	PrimaryExpressionPtr primaryExpression = make_shared<PrimaryExpression>();
	switch(unit1->type) {
	case CWord::IDENTIFIER: {
		primaryExpression->command = make_shared<Command>();
		string& identifier = unit1->word;
		VariableManager& varMgr = VariableManager::getInstance();
		if (varMgr.hasVariable(identifier)) {
			shared_ptr<Variable> var = varMgr.getVariable(identifier);
			CommandPtr loadCode = var->generateLoadCode();
			primaryExpression->command->addCodes(loadCode);
			primaryExpression->command->setType(var->type);
		}
		else { // find function
			FunctionManager& funcMgr = FunctionManager::getInstance();
			if (!funcMgr.hasFunction(identifier)) throw UnknownIdentifierException();
			primaryExpression->functionIdentifier = identifier;
		}
		break;
	}
	case CWord::CONSTANT: {
		primaryExpression->command = generateConstantExpression(unit1);
		break;
	}
	case CWord::STRING_LITERAL: {
		primaryExpression->command = make_shared<Command>();
		string& str = unit1->word;
		
		replaceStr(str, "\\n", "\n");
		PosType pos = DataManager::getInstance().addString(str.c_str() + 1, str.size() - 2);
		primaryExpression->command->addIMM(pos);
		primaryExpression->command->setType(SignedCharPointType);
		break;
	}
	}
	return primaryExpression;
}

// string_literal :
// STRING_LITERAL STRING_LITERAL
shared_ptr<TerminalUnit> generateStringLiteral(shared_ptr<TerminalUnit> unit1, shared_ptr<TerminalUnit> unit2) {
	unit1->word.pop_back();
	unit1->word += unit2->word.substr(1, unit2->word.size() - 1);
	return unit1;
}


// '(' expression ')'
PrimaryExpressionPtr generatePrimaryExpression(TerminalUnitPtr unit1, CommandPtr unit2, TerminalUnitPtr unit3) {
	PrimaryExpressionPtr primaryExpression = make_shared<PrimaryExpression>();
	primaryExpression->command = unit2;
	return primaryExpression;
}

// argument_expression_list : 
// assignment_expression
ArgumentExpressionListPtr generateArgumentExpressionList(CommandPtr unit1) {
	ArgumentExpressionListPtr argumentExpressionList = make_shared<ArgumentExpressionList>();
	argumentExpressionList->parameters.emplace_back(unit1);
	return argumentExpressionList;
}

// argument_expression_list ',' assignment_expression
ArgumentExpressionListPtr generateArgumentExpressionList(ArgumentExpressionListPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	unit1->parameters.emplace_back(unit3);
	return unit1;
}


// cast_expression : 
// '(' type_name ')' cast_expression
CommandPtr generateCastExpression(TerminalUnitPtr unit1, TypeNamePtr unit2, TerminalUnitPtr unit3, CommandPtr unit4) {
	shared_ptr<Type> type = unit2->type;
	converseType(unit4, type);
	return unit4;
}

void generateAssignExpression(CommandPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	BaseValueType op = unit1->popCommand();
	unit1->addPSH();
	shared_ptr<Type> type = unit1->getType();
	converseType(unit3, type);
	
	unit1->addCodes(unit3);
	switch (op) {
	case Command::L8:
		unit1->addS8();
		break;
	case Command::L16:
		unit1->addS16();
		break;
	case Command::L32:
		unit1->addS32();
		break;
	case Command::L64:
		unit1->addS64();
		break;
	default:
		throw NoLeftValueException();
	}
}

CommandPtr generateAddSubExpression(CommandPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	if (unit2->type == '+' && (unit1->getType()->baseType == Type::Pointer && unit3->getType()->baseType == Type::Pointer)) {
		throw InvalidExpressionException();
	}

	implicitConverseType(unit1, unit3);

	if ((!unit1->getType()->isBaseType() && unit1->getType()->baseType != Type::Pointer)
		|| (!unit3->getType()->isBaseType() && unit3->getType()->baseType != Type::Pointer)) {
		throw InvalidExpressionException();
	}
	
	if (unit1->getType()->baseType != Type::Pointer && unit3->getType()->baseType == Type::Pointer) {
		assert(unit2->type == '+');
		CommandPtr temp = unit1;
		unit1 = unit3;
		unit3 = temp;
	}
	
	if (unit1->getType()->baseType == Type::Pointer) {
		shared_ptr<PointerType> type = dynamic_pointer_cast<PointerType>(unit1->getType());
		if (type->pointType->size() > 1) {
			unit3->addPSH();
			unit3->addIMM(type->pointType->size());
			unit3->addMUL();
		}
	}
	
	unit1->addPSH();
	unit1->addCodes(unit3);
	if (unit2->type == '+') {
		unit1->addADD();
	}
	else {
		unit1->addSUB();
		if (unit1->getType()->baseType == Type::Pointer && unit3->getType()->baseType == Type::Pointer) {
			unit1->setType(SignedIntType);
		}
	}
	
	return unit1;
}

void generateAddSubAssignExpression(CommandPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	BaseValueType op = unit1->popCommand();
	unit1->addPSH();
	unit1->addRawValue(op);
	unit1->addPSH();
	
	shared_ptr<Type> finalType = unit1->getType();
	shared_ptr<Type> tempType = implicitConverseType(unit1, unit3);
	if (!tempType->canImplicitConverseTo(*finalType)) {
		throw InvalidTypeConversionException();
	}
	
	if (unit1->getType()->baseType == Type::Pointer) {
		shared_ptr<PointerType> type = dynamic_pointer_cast<PointerType>(unit1->getType());
		if (type->pointType->size() > 1) {
			unit3->addPSH();
			unit3->addIMM(type->pointType->size());
			unit3->addMUL();
		}
	}
	
	unit1->addCodes(unit3);
	if (unit2->type == CWord::ADD_ASSIGN) {
		unit1->addADD();
	}
	else {
		unit1->addSUB();
	}
	
	unit1->setType(tempType);
	converseType(unit1, finalType);
	
	switch (op) {
	case Command::L8:
		unit1->addS8();
		break;
	case Command::L16:
		unit1->addS16();
		break;
	case Command::L32:
		unit1->addS32();
		break;
	case Command::L64:
		unit1->addS64();
		break;
	default:
		throw NoLeftValueException();
	}
}

void generateCompoundAssignExpression(CommandPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	BaseValueType op = unit1->popCommand();
	unit1->addPSH();
	unit1->addRawValue(op);
	unit1->addPSH();
	
	shared_ptr<Type> finalType = unit1->getType();
	shared_ptr<Type> tempType = implicitConverseType(unit1, unit3);
	if (!tempType->canImplicitConverseTo(*finalType)) {
		throw InvalidTypeConversionException();
	}
	
	unit1->addCodes(unit3);
	switch (unit2->type) {
	case CWord::MUL_ASSIGN:
		unit1->addMUL();
		break;
	case CWord::DIV_ASSIGN:
		if (unit1->getType()->isUnsigned() || unit3->getType()->isUnsigned()) {
			if (unit1->getType()->baseType == Type::SignedLongLong || unit1->getType()->baseType == Type::UnsignedLongLong) {
				unit1->addDIVU64();
			}
			else
			{
				unit1->addDIVU32();
			}
		}
		else {
			unit1->addDIV();
		}
		break;
	case CWord::MOD_ASSIGN:
		if (unit1->getType()->isUnsigned() || unit3->getType()->isUnsigned()) {
			if (unit1->getType()->baseType == Type::SignedLongLong || unit1->getType()->baseType == Type::UnsignedLongLong) {
				unit1->addMODU64();
			}
			else
			{
				unit1->addMODU32();
			}
		}
		else {
			unit1->addMOD();
		}
		break;
	case CWord::LEFT_ASSIGN:
		unit1->addSHL();
		break;
	case CWord::RIGHT_ASSIGN:
		if (unit1->getType()->isUnsigned() || unit3->getType()->isUnsigned()) {
			if (unit1->getType()->baseType == Type::SignedLongLong || unit1->getType()->baseType == Type::UnsignedLongLong) {
				unit1->addSHRU64();
			}
			else
			{
				unit1->addSHRU32();
			}
		}
		else {
			unit1->addSHR();
		}
		break;
	case CWord::AND_ASSIGN:
		unit1->addAND();
		break;
	case CWord::XOR_ASSIGN:
		unit1->addXOR();
		break;
	case CWord::OR_ASSIGN:
		unit1->addOR();
		break;
	}
	
	converseType(unit1, finalType);
	
	switch (op) {
	case Command::L8:
		unit1->addS8();
		break;
	case Command::L16:
		unit1->addS16();
		break;
	case Command::L32:
		unit1->addS32();
		break;
	case Command::L64:
		unit1->addS64();
		break;
	default:
		throw NoLeftValueException();
	}
}

void generateCompoundExpression(CommandPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	unit1->addPSH();
	unit1->addCodes(unit3);
	implicitConverseType(unit1, unit3);
	switch (unit2->type) {
	case '*':
		unit1->addMUL();
		break;
	case '/':
		if (unit1->getType()->isUnsigned() || unit3->getType()->isUnsigned()) {
			if (unit1->getType()->baseType == Type::SignedLongLong || unit1->getType()->baseType == Type::UnsignedLongLong) {
				unit1->addDIVU64();
			}
			else
			{
				unit1->addDIVU32();
			}
		}
		else {
			unit1->addDIV();
		}
		break;
	case '%':
		if (unit1->getType()->isUnsigned() || unit3->getType()->isUnsigned()) {
			if (unit1->getType()->baseType == Type::SignedLongLong || unit1->getType()->baseType == Type::UnsignedLongLong) {
				unit1->addMODU64();
			}
			else
			{
				unit1->addMODU32();
			}
		}
		else {
			unit1->addMOD();
		}
		break;
	case CWord::LEFT_OP:
		unit1->addSHL();
		break;
	case CWord::RIGHT_OP:
		if (unit1->getType()->isUnsigned() || unit3->getType()->isUnsigned()) {
			if (unit1->getType()->baseType == Type::SignedLongLong || unit1->getType()->baseType == Type::UnsignedLongLong) {
				unit1->addSHRU64();
			}
			else
			{
				unit1->addSHRU32();
			}
		}
		else {
			unit1->addSHR();
		}
		break;
	case '<':
		if (unit1->getType()->isUnsigned() || unit3->getType()->isUnsigned()) {
			unit1->addLTU();
		}
		else {
			unit1->addLT();
		}
		break;
	case '>':
		if (unit1->getType()->isUnsigned() || unit3->getType()->isUnsigned()) {
			unit1->addGTU();
		}
		else {
			unit1->addGT();
		}
		break;
	case CWord::LE_OP:
		if (unit1->getType()->isUnsigned() || unit3->getType()->isUnsigned()) {
			unit1->addLEU();
		}
		else {
			unit1->addLE();
		}
		break;
	case CWord::GE_OP:
		if (unit1->getType()->isUnsigned() || unit3->getType()->isUnsigned()) {
			unit1->addGEU();
		}
		else {
			unit1->addGE();
		}
		break;
	case CWord::EQ_OP:
		unit1->addEQ();
		break;
	case CWord::NE_OP:
		unit1->addNE();
		break;
	case '&':
		unit1->addAND();
		break;
	case '^':
		unit1->addXOR();
		break;
	case '|':
		unit1->addOR();
		break;
	}
}

void generateLogicExpression(CommandPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	size_t size = unit3->size();
	switch (unit2->type) {
	case CWord::AND_OP:
		unit1->addBZ(unit1->size() + 2 + unit3->size());
		break;
	case CWord::OR_OP:
		unit1->addBNZ(unit1->size() + 2 + unit3->size());
		break;
	}
	unit1->addCodes(unit3);
	unit1->setType(SignedIntType);
}


// expression OP expression : 
// unary_expression assignment_operator assignment_expression
// expression ',' assignment_expression
CommandPtr generateExpression(CommandPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	switch (unit2->type) {
	case ',':
		unit1->addCodes(unit3);
		break;
	case '=':
		generateAssignExpression(unit1, unit2, unit3);
		break;
	case CWord::ADD_ASSIGN:
	case CWord::SUB_ASSIGN:
		generateAddSubAssignExpression(unit1, unit2, unit3);
		break;
	case CWord::MUL_ASSIGN:
	case CWord::DIV_ASSIGN:
	case CWord::MOD_ASSIGN:
	case CWord::LEFT_ASSIGN:
	case CWord::RIGHT_ASSIGN:
	case CWord::AND_ASSIGN:
	case CWord::XOR_ASSIGN:
	case CWord::OR_ASSIGN:
		generateCompoundAssignExpression(unit1, unit2, unit3);
		break;
	case '+':
	case '-':
		return generateAddSubExpression(unit1, unit2, unit3);
	case '*':
	case '/':
	case '%':
	case CWord::LEFT_OP:
	case CWord::RIGHT_OP:
	case '<':
	case '>':
	case CWord::LE_OP:
	case CWord::GE_OP:
	case CWord::EQ_OP:
	case CWord::NE_OP:
	case '&':
	case '^':
	case '|':
		generateCompoundExpression(unit1, unit2, unit3);
		break;
	case CWord::AND_OP:
	case CWord::OR_OP:
		generateLogicExpression(unit1, unit2, unit3);
		break;
	}
	return unit1;
}

// conditional_expression : 
// logical_or_expression '?' expression ':' conditional_expression
CommandPtr generateConditionalExpression(CommandPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3,
		TerminalUnitPtr unit4, CommandPtr unit5) {
	PosType pos1 = unit1->size() + 4 + unit3->size();
	PosType pos2 = pos1 + unit5->size();
	unit1->addBZ(pos1);
	unit1->addCodes(unit3);
	unit1->addJMP(pos2);
	unit1->addCodes(unit5);
	return unit1;
}

#endif