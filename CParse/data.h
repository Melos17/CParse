#ifndef DATA_H
#define DATA_H

#include "data.h"
#include "command.h"

class DataManager {
public:
	static DataManager& getInstance();
	
	MemoryType addInt(BaseValueType value);
	MemoryType addInt(const BaseValueType* values, size_t count);

	MemoryType addChar(BaseValueType value);
	MemoryType addChar(char value);
	MemoryType addChar(const BaseValueType* values, size_t count);
	MemoryType addString(const char* values, size_t count);
	
	MemoryType applyMemory(BaseValueType size);
	
	void init();
	
private:
	DataManager() { init(); }
	
	char* mData;
	MemoryType mSize;
};

#endif