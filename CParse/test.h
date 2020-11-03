#ifndef TEST_H
#define TEST_H

#include <vector>
#include <fstream>
#include <exception>

#include "Parse\parse.h"
#include "vm.h"

using std::string;
using std::to_string;
using std::vector;
using std::getline;
using std::ifstream;
using std::ios;
using std::cout;
using std::cin;
using std::istreambuf_iterator;
using std::exception;

struct TestResult {
	bool pass;
	string report;
};

class ReturnValueCompare {
public:
	TestResult run(const string& codePath, BaseValueType returnValue) {
		TestResult testResult;
		std::ifstream ifs(codePath);
		string codes((istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		
		if(codes.empty()) {
			testResult.pass = false;
			testResult.report = "Invalid test file path:" + codePath;
			return testResult;
		}
		
		shared_ptr<Command> command;
		BaseValueType ret = -1;
		CParse parse(codes);
		parse.init();
		parse.parse();
		command = parse.fetchCommand();
		try {
		}
		catch (const exception& e) {
			testResult.pass = false;
			testResult.report = string() + "Compile Error:" + e.what();
			return testResult;
		}
		catch (...) {
			testResult.pass = false;
			testResult.report = "Unkonwn Compile Error";
			return testResult;
		}
		
		try {
			VirtualMachine vm(*command);
			ret = vm.run();
		}
		catch (const exception& e) {
			testResult.pass = false;
			testResult.report = string() + "Runtime Error:" + e.what();
			return testResult;
		}
		catch (...) {
			testResult.pass = false;
			testResult.report = "Unkonwn Runtime Error";
			return testResult;
		}
		
		testResult.pass = ret == returnValue;
		if (!testResult.pass) {
			testResult.report = "Error Return Value:" + to_string(ret) + "\nCorrect:" + to_string(returnValue);
		}
		return testResult;
	}
};

class CommandCompare {
public:
	TestResult run(const string& codePath, const string& commandPath) {
		TestResult testResult;
		std::ifstream ifs(codePath);
		string codes((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		std::ifstream ifs2(commandPath);
		string correctCommand((std::istreambuf_iterator<char>(ifs2)), (std::istreambuf_iterator<char>()));
		
		if(codes.empty()) {
			testResult.pass = false;
			testResult.report = "Invalid test file path:" + codePath;
			return testResult;
		}
		
		shared_ptr<Command> command;
		try {
			CParse parse(codes);
			parse.init();
			parse.parse();
			command = parse.fetchCommand();
		}
		catch (const exception& e) {
			testResult.pass = false;
			testResult.report = string() + "Compile Error:" + e.what();
			return testResult;
		}
		catch (...) {
			testResult.pass = false;
			testResult.report = "Unkonwn Compile Error";
			return testResult;
		}
		
		bool isSame;
		vector<string> commandList1 = split(command->toString(), "\n");
		vector<string> commandList2 = split(correctCommand, "\n");
		string cmp = mixCommandList(commandList1, commandList2, &isSame);
		
		return testResult;
	}
	
private:
	vector<string> split(const string& source, const string& delimiter) {
		vector<string> ret;
		string::size_type l = 0;
		string::size_type r = source.find(delimiter);
		while(r != string::npos || l != string::npos) {
			ret.emplace_back(source.substr(l, r-l));
			l = r;
			r = source.find(delimiter, r);
		}
		return ret;
	}

	string mixCommandList(vector<string>& commandList1, vector<string>& commandList2, bool* isSame) {
		*isSame = true;
		string ret;
		size_t i = 0;
		size_t i2 = 0;
		if (commandList1.size() != commandList2.size()) *isSame = false;
		while (i < commandList1.size() && i2 < commandList2.size()) {
#ifdef _DEBUG
			if (commandList1[i].find("--") != string::npos || commandList2[i2].find("--") != string::npos) {
				if (commandList1[i].find("--") != string::npos) {
					ret += "1.";
					ret += commandList1[i++];
				}
				if (commandList2[i2].find("--") != string::npos) {
					ret += "2.";
					ret += commandList2[i2++];
				}
				ret += '\n';
				continue;
			}
#endif
			ret += commandList1[i];
			for (int j = 0; j < 10 - (int)commandList1[i].size(); ++j) {
				ret += ' ';
			}
			ret += commandList2[i2];
			if (commandList1[i] != commandList2[i2]) {
				ret += "---DIF";
				*isSame = false;
			}
			ret += '\n';
			++i;
			++i2;
		}
		while (i < commandList1.size()) {
			ret += commandList1[i++] + '\n';
		}
		while (i2 < commandList2.size()) {
			ret += "          " + commandList2[i2++] + '\n';
		}
		return ret;
	}
};

class CompileErrorCompare {
public:
	TestResult run(const string& codePath, const string& error) {
		TestResult testResult;
		std::ifstream ifs(codePath);
		string codes((istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		
		if(codes.empty()) {
			testResult.pass = false;
			testResult.report = "Invalid test file path:" + codePath;
			return testResult;
		}
		
		try {
			CParse parse(codes);
			parse.init();
			parse.parse();
		}
		catch (const exception& e) {
			if (error == e.what()) {
				testResult.pass = true;
			}
			else {
				testResult.pass = false;
				testResult.report = string() + "Compile Error:" + e.what();
				testResult.report += "\nExpect Compile Error:" + error;
			}
			return testResult;
		}
		catch (...) {
			testResult.pass = false;
			testResult.report = "Unkonwn Compile Error";
			return testResult;
		}
		
		testResult.pass = false;
		testResult.report = "CompileErrorCompare : No Compile Error Occurs!";
		return testResult;
	}
};

class SmokeTest {
public:
	TestResult run(const string& codePath) {
		TestResult testResult;
		std::ifstream ifs(codePath);
		string codes((istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		
		if(codes.empty()) {
			testResult.pass = false;
			testResult.report = "Invalid test file path:" + codePath;
			return testResult;
		}
		
		try {
			CParse parse(codes);
			parse.init();
			parse.parse();
		}
		catch (const exception& e) {
			testResult.pass = false;
			testResult.report = string() + "Compile Error:" + e.what();
			return testResult;
		}
		catch (...) {
			testResult.pass = false;
			testResult.report = "Unkonwn Compile Error";
			return testResult;
		}
		
		testResult.pass = true;
		return testResult;
	}
};

class TestManager {
public:
	static TestManager& getInstance() {
		static TestManager sInstance;
		return sInstance;
	}

	void run(const string& targetPath) {
		string line;
		ifstream file;
		file.open(targetPath, ios::in);
		assert(file.is_open());

		int totalCase = 0;
		int failCase = 0;

		while (getline(file, line)) {
			TestResult tr;
			if (line.empty() || line.find("//") != string::npos) {
				continue;
			}
			else if (line == "ReturnValueCompare") {
				ReturnValueCompare compare;
				string codePath;
				getline(file, codePath);
				cout << totalCase << '.' << codePath << endl;
				getline(file, line);
				BaseValueType returnValue = atoll(line.c_str());
				tr = compare.run(codePath, returnValue);
			}
			else if(line == "CommandCompare") {
				CommandCompare compare;
				string codePath;
				getline(file, codePath);
				cout << totalCase << '.' << codePath << endl;
				string commandPath;
				getline(file, commandPath);
				tr = compare.run(codePath, commandPath);
			}
			else if(line == "CompileErrorCompare") {
				CompileErrorCompare compare;
				string codePath;
				getline(file, codePath);
				cout << totalCase << '.' << codePath << endl;
				string error;
				getline(file, error);
				tr = compare.run(codePath, error);
			}
			else if(line == "SmokeTest") {
				SmokeTest compare;
				string codePath;
				getline(file, codePath);
				cout << totalCase << '.' << codePath << endl;
				tr = compare.run(codePath);
			}
			else {
				tr.pass = false;
				tr.report = "Unknown test:" + line;
			}

			++totalCase;
			if (!tr.pass) {
				++failCase;
				cout << tr.report << endl;
			}
			cout << endl;
		}

		cout << "Total case:" << totalCase << ", Fail case:" << failCase << endl;
	}
private:
	TestManager() { }
};

#endif