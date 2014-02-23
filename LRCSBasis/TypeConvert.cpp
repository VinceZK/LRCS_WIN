#include "stdafx.h"
#include "TypeConvert.h"

TypeConvert::TypeConvert()
{
}


TypeConvert::~TypeConvert()
{
}

bool TypeConvert::StringToBoolean(string& value_){
	return !value_.empty() &&
		(strcmp(value_.c_str(), "true") == 0 ||
		strcmp(value_.c_str(), "TRUE") == 0 ||
		atoi(value_.c_str()) != 0);
}

int TypeConvert::StringToInt(string& value_){
	return atoi(value_.c_str());
}