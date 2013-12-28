#pragma once
class __declspec(dllexport) HashFunction {
public:
	virtual ~HashFunction(){};
	virtual unsigned int operator()(unsigned int value) = 0;

};

class __declspec(dllexport) IdentityHashFunction : public HashFunction {
public:
	IdentityHashFunction(int max) {
		_max = max;
	}
	virtual ~IdentityHashFunction(){};
	virtual unsigned int inline operator()(unsigned int value) { return value % _max; }

private:
	int _max;
};

