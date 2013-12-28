#include "ExceptionTst.h"
#include "Exception.h"
#include "AccessException.h"
#include "CodingException.h"
#include "UnexpectedException.h"
#include "UnimplementedException.h"
/*Example: How to using exception classes!*/

ExceptionTst::ExceptionTst()
{
}


ExceptionTst::~ExceptionTst()
{
}

bool ExceptionTst::run(char* arg1, char* arg2) {
	try{
		throwAccessException();
	}
	catch(Exception* e){
		cout << e->msg << endl;
		delete e;
	}

	try{
		throwCodingException();
	}
	catch (Exception* e){
		cout << e->msg << endl;
		delete e;
	}

	try{
		throwUnexpectedException();
	}
	catch (Exception* e){
		cout << e->msg << endl;
		delete e;
	}

	try{
		throwUnimplementedException();
	}
	catch (Exception* e){
		cout << e->msg << endl;
		delete e;
	}

	return true;
}

void ExceptionTst::throwAccessException(){
	throw new AccessException("Access Exception!");
}
void ExceptionTst::throwCodingException(){
	throw new CodingException("Coding Exception!");
}
void ExceptionTst::throwUnexpectedException(){
	throw new UnexpectedException("Unexpected Exception!");
}
void ExceptionTst::throwUnimplementedException(){
	throw new UnimplementedException("Unimplemeneted Exception!");
}