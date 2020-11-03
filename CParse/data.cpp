#include "data.h"

DataManager& DataManager::getInstance() {
	static DataManager sDataManager;
	return sDataManager;
}

MemoryType DataManager::addInt(BaseValueType value) {
	size_t size = sizeof(int);
	mSize = ((mSize + (size - 1)) / size) * size;
	MemoryType pos = (MemoryType)(mData + mSize);
	*(MemoryType*)(pos) = value;
	mSize += size;
	return pos;
}

MemoryType DataManager::addInt(const BaseValueType* values, size_t count) {
	size_t size = sizeof(int);
	mSize = ((mSize + (size - 1)) / size) * size;
	MemoryType pos = (MemoryType)(mData + mSize);
	for (size_t i = 0; i < count; ++i) {
		*(BaseValueType*)(pos + (i * size)) = values[i];
	}
	mSize += count * size;
	return pos;
}

MemoryType DataManager::addChar(BaseValueType value) {
	MemoryType pos = (MemoryType)(mData + mSize);
	*(char*)(pos) = (char)value;
	mSize += 1;
	return pos;
}

MemoryType DataManager::addChar(char value) {
	MemoryType pos = (MemoryType)(mData + mSize);
	*(char*)(pos) = value;
	mSize += 1;
	return pos;
}

MemoryType DataManager::addChar(const BaseValueType* values, size_t count) {
	MemoryType pos = (MemoryType)(mData + mSize);
	for (size_t i = 0; i < count; ++i) {
		*(char*)(pos + i) = (char)values[i];
	}
	mSize += count;
	return pos;
}

MemoryType DataManager::addString(const char* values, size_t count) {
	MemoryType pos = (MemoryType)(mData + mSize);
	for (size_t i = 0; i < count; ++i) {
		*(char*)(pos + i) = values[i];
	}
	if (values[count - 1]) {
		*(char*)(pos + count) = 0;
		++count;
	}
	mSize += count;
	return pos;
}

MemoryType DataManager::applyMemory(BaseValueType size) {
	mSize = ((mSize + (MemorySize - 1)) / MemorySize) * MemorySize;
	MemoryType pos = (MemoryType)(mData + mSize);
	mSize += size;
	return pos;
}

void DataManager::init() {
	mSize = 0;
	if (mData) delete mData;
	size_t dataSize = 1024 * 1024;
	mData = new char[dataSize]();
}