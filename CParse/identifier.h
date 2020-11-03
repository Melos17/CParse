#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;

class IdentifierManager {
public:
	enum IdentifierType {
		NonExist, Var, Typename, Function,
	};
	
	static IdentifierManager& getInstance();
	
	void addFunction(const string& identifier);
	void addVariable(const string& identifier);
	void addTypename(const string& identifier);

	IdentifierType getType(const string& identifier);

	bool hasThisLevelIdentifier(const string& identifier);
	bool hasIdentifier(const string& identifier);
	
	void advanceLevel();
	void retreatLevel();

	int getLevel();
	
	void init();
	
private:
	IdentifierManager() { init(); }
	
	// 0 : Global
	// 1 : Parameter
	// >=2 : Local (no allow level2 to define same identifier as parameter)
	int mLevel;
	vector<unordered_map<string, IdentifierType>> mIdentifiers;
};

#endif