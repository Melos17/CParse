#ifndef REDUCE_STATEMENT_H
#define REDUCE_STATEMENT_H

#include "reduce.h"

// statement_list : 
// statement_list statement
CommandPtr generateStatementList(CommandPtr unit1, CommandPtr unit2) {
	unit1->addCodes(unit2);
	return unit1;
}


// iteration_statement : 
// WHILE '(' expression ')' statement
CommandPtr generateIterationStatement(TerminalUnitPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3,
		TerminalUnitPtr unit4, CommandPtr unit5) {
	PosType pos1 = 0;
	PosType pos2 = unit3->size() + unit5->size() + 4;
	unit3->addBZ(pos2);
	unit3->addCodes(unit5);
	unit3->addJMP(pos1);
	unit3->setContinuePosition(pos1);
	unit3->setBreakPosition(pos2);
	return unit3;
}

// DO statement WHILE '(' expression ')' ';'
CommandPtr generateIterationStatement(TerminalUnitPtr unit1, CommandPtr unit2, TerminalUnitPtr unit3,
		TerminalUnitPtr unit4, CommandPtr unit5, TerminalUnitPtr unit6, TerminalUnitPtr unit7) {
	PosType pos1 = unit2->size();
	PosType pos2 = unit2->size() + unit5->size() + 2;
	unit2->addCodes(unit5);
	unit2->addBNZ(0);
	unit2->setContinuePosition(pos1);
	unit2->setBreakPosition(pos2);
	return unit2;
}


// FOR '(' expression_statement expression_statement expression ')' statement
CommandPtr generateIterationStatement(TerminalUnitPtr unit1, TerminalUnitPtr unit2, CommandPtr initCommand,
	CommandPtr judgeCommand, CommandPtr iterCommand, TerminalUnitPtr unit6, CommandPtr statementCommand) {
	PosType loopStartPos = initCommand->size();
	PosType outPos = initCommand->size() + judgeCommand->size() + statementCommand->size() + iterCommand->size() + 4;
	PosType iterPos = initCommand->size() + judgeCommand->size() + statementCommand->size() + 2;

	if(judgeCommand->size() == 0) {
		judgeCommand->addIMM(1);
		judgeCommand->setType(SignedIntType);
	}

	CommandPtr cmd = make_shared<Command>();
	cmd->addCodes(initCommand);
	cmd->addCodes(judgeCommand);
	cmd->addBZ(outPos);
	cmd->addCodes(statementCommand);
	cmd->addCodes(iterCommand);
	cmd->addJMP(loopStartPos);
	cmd->setContinuePosition(iterPos);
	cmd->setBreakPosition(outPos);

	return cmd;
}

// FOR '(' expression_statement expression_statement ')' statement
CommandPtr generateIterationStatement(TerminalUnitPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3,
		CommandPtr unit4, TerminalUnitPtr unit5, CommandPtr unit6) {
	return generateIterationStatement(unit1, unit2, unit3, unit4, (CommandPtr) nullptr, unit5, unit6);
}


// SelectionStatement : 
// IF '(' expression ')' statement
// SWITCH '(' expression ')' statement
CommandPtr generateSelectionStatement(TerminalUnitPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3,
		TerminalUnitPtr unit4, CommandPtr unit5) {
	switch (unit1->type) {
	case CWord::IF: {
		PosType pos = unit3->size() + unit5->size() + 2;
		unit3->addBZ(pos);
		unit3->addCodes(unit5);
		break;
	}
	case CWord::SWITCH:
		assert(false); // no support now
		break;
	}
	return unit3;
}

// IF '(' expression ')' statement ELSE statement
CommandPtr generateSelectionStatement(TerminalUnitPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3,
		TerminalUnitPtr unit4, CommandPtr unit5, TerminalUnitPtr unit6, CommandPtr unit7) {
	PosType pos1 = unit3->size() + 4 + unit5->size();
	PosType pos2 = pos1 + unit7->size();
	unit3->addBZ(pos1);
	unit3->addCodes(unit5);
	unit3->addJMP(pos2);
	unit3->addCodes(unit7);
	return unit3;
}


// labeled_statement : 
// DEFAULT ':' statement
// 	IDENTIFIER ':' statement
CommandPtr generateLabeledStatement(TerminalUnitPtr unit1, TerminalUnitPtr unit2, CommandPtr unit3) {
	switch(unit1->type) {
	case CWord::DEFAULT:

		break;
	case CWord::IDENTIFIER:

		break;
	}
	assert(false); // no support now
	return unit3;
}

// CASE constant_expression ':' statement
CommandPtr generateLabeledStatement(TerminalUnitPtr unit1, CommandPtr unit2, TerminalUnitPtr unit3, CommandPtr unit4) {
	assert(false); // no support now
	return unit2;
}


// compound_statement : 
// '{' '}'
CommandPtr generateCompoundStatement(TerminalUnitPtr unit1, TerminalUnitPtr unit2) {
	CommandPtr code = make_shared<Command>();
	return code;
}

// '{' statement_list '}'
CommandPtr generateCompoundStatement(TerminalUnitPtr unit1, CommandPtr unit2, TerminalUnitPtr unit3) {
	CommandPtr code = make_shared<Command>();
	code->addCodes(unit2);
	return code;
}

// '{' declaration_list '}'
CommandPtr generateCompoundStatement(TerminalUnitPtr unit1, DeclarationListPtr unit2, TerminalUnitPtr unit3) {
	CommandPtr code = make_shared<Command>();
	return code;
}

// '{' declaration_list statement_list '}'
CommandPtr generateCompoundStatement(TerminalUnitPtr unit1, DeclarationListPtr unit2, CommandPtr unit3, TerminalUnitPtr unit4) {
	CommandPtr code = make_shared<Command>();
	BaseValueType size = VariableManager::getInstance().getStackSizeNextLevel();
	code->addADJ(-size);
	code->addCodes(unit2->initializer);
	code->addCodes(unit3);
	return code;
}

TerminalUnitPtr advanceLevel(TerminalUnitPtr unit1) {
	// advance level in case no parameter
	if (IdentifierManager::getInstance().getLevel() == 0) {
		IdentifierManager::getInstance().advanceLevel();
	}
	IdentifierManager::getInstance().advanceLevel();
	return unit1;
}

TerminalUnitPtr retreatLevel(TerminalUnitPtr unit1) {
	IdentifierManager::getInstance().retreatLevel();
	return unit1;
}



// expression_statement : 
// ';'
CommandPtr generateExpressionStatement(TerminalUnitPtr unit1) {
	CommandPtr command = make_shared<Command>();
	return command;
}

// expression ';'
CommandPtr generateExpressionStatement(CommandPtr unit1, TerminalUnitPtr unit2) {
	return unit1;
}


// jump_statement : 
// CONTINUE ';'
// 	BREAK ';'
// 	RETURN ';'
CommandPtr generateJumpStatement(TerminalUnitPtr unit1, TerminalUnitPtr unit2) {
	CommandPtr code = make_shared<Command>();
	switch(unit1->type) {
	case CWord::CONTINUE:
		code->addCONTINUE();
		break;
	case CWord::BREAK:
		code->addBREAK();
		break;
	case CWord::RETURN:
		code->addLEV();
		break;
	}
	return code;
}

// GOTO IDENTIFIER ';'
CommandPtr generateJumpStatement(TerminalUnitPtr unit1, TerminalUnitPtr unit2, TerminalUnitPtr unit3) {
	assert(false); // no support now
	CommandPtr code = make_shared<Command>();
	return code;
}

// RETURN expression ';'
CommandPtr generateJumpStatement(TerminalUnitPtr unit1, CommandPtr unit2, TerminalUnitPtr unit3) {
	unit2->addLEV();
	return unit2;
}

#endif