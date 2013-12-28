#pragma once
#include <iterator>
#include <cassert>
#include <map>
#include <vector>

#include "HashFunction.h"
#include "IntValPos.h"
#include "UnexpectedException.h"

using namespace std;

class __declspec(dllexport) HashMap
{
public:
	HashMap(unsigned int numBuckets_, unsigned int dataCapacity_ = 100000);
	~HashMap();
	void init(unsigned int numBuckets_, unsigned int dataCapacity_ = 100000);

	// set max memory capacity
	//void setDataCapacity(unsigned int dataCapacity_);

	bool isEmpty();

	// memory capacity in bytes
	unsigned int getDataCapacity();
	// memory footprint in bytes
	unsigned int getDataSize();

	// number of hash buckets
	unsigned int getNumBuckets();
	// number of unique keys stored
	unsigned int getNumKeys();

	void put(ValPos* key_, void *value_, unsigned int size_);
	void put(int key_, void *value_, unsigned int size_);
	void* get(ValPos* key_);
	void* get(int key_);
	void* remove(ValPos* key_);
	void* remove(int key_);

	void setHashFunction(HashFunction *hashFunc);

	ValPos* getKey();

private:

	struct Obj {
		Obj() {
			_count = 0;
		}
		Obj(ValPos* key_, void *value_, unsigned int size_) : _key(key_), _value(value_), _size(size_) {
			_next = NULL;
		}
		ValPos* _key;
		void *_value;
		unsigned int _size;
		Obj* _next;
		unsigned int _count;
	};

	struct IntObj {
		IntObj() {
			_count = 0;
		}
		IntObj(int key_, void *value_, unsigned int size_) : _key(key_), _value(value_), _size(size_) {
			_next = NULL;
		}
		int _key;
		void *_value;
		unsigned int _size;
		IntObj* _next;
		unsigned int _count;
	};

	//void** _buckets;
	Obj* _buckets;
	IntObj* _intbuckets;
	//int* bucketSize;

	unsigned int _minNonEmptyBucket;

	unsigned int _numBuckets;
	unsigned int _numKeys;
	unsigned int _dataCapacity;
	unsigned int _currDataSize;

	HashFunction* _hashFunc;
	bool useIntObjs;
	ValPos* playvp;
};

