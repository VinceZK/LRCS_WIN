#pragma once
#include "IntValPos.h"

class __declspec(dllexport) Predicate
{
public:

	// Enumeration that describes the type of operation this predicate performs
	enum PredicateType {
		OP_LESS_THAN,
		OP_LESS_THAN_OR_EQUAL,
		OP_EQUAL,
		OP_GREATER_THAN_OR_EQUAL,
		OP_GREATER_THAN,
		//OP_EVEN,
		//OP_ODD	
	};

	// Instatiate a predicate with nothing bound
	Predicate(PredicateType type_);

	// Instatiate a predicate with the rhs bound (useful for > k style predicates)
	Predicate(PredicateType type_, ValPos* rhs_);

	// Copy constructor
	Predicate(const Predicate& predicate_);

	// Destructor
	virtual ~Predicate();

	// Bind the RHS of operator	
	void setRHS(ValPos* rhs_);

	// Bind the RHS of operator	
	void setRHS(int rhsint);

	// Bind the lhs of operator	
	void setLHS(ValPos* lhs_);

	// Get the value of the lhs of operator
	ValPos* getLHS();

	// Get the value of the rhs of operator
	ValPos* getRHS();

	// Get the type of predicate
	PredicateType getPredType();

	// Evaluate the predicate
	bool evalPredicate();

	// Evaluate the predicate with the special case that the rhs is an int
	bool evalPredicate(int rhs);

protected:
	ValPos* lhsVal;
	ValPos* rhsVal;
	bool isBinary;
	bool isBoundLHS;
	bool isBoundRHS;
	PredicateType predType;
	ValPos* vp;
};

