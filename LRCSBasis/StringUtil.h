#pragma once
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
using namespace std;

class __declspec(dllexport) StringUtil
{
public:
	StringUtil();
	virtual ~StringUtil();

	static char* extractDelimitedSubstring(char* d, char* strLine, int index);
	static string splitDelimitedSubstring(char c, string str, int index);
	static string ltrim(const string &str, const string &whitespace);
	static string rtrim(const string &str, const string &whitespace);
	static string trim(const string &str, const string &whitespace);
	/*Get the smallest value that larger than a give string value*/
	static unsigned char* getSmallestLargerValue(char* value, int valsize);
};

