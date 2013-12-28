#include "StringUtil.h"

StringUtil::StringUtil(){}
StringUtil::~StringUtil(){}

//used for parse tab delimited data files
char* StringUtil::extractDelimitedSubstring(char* d, char* strLine, int index)
{
	char *p;
	p = strtok(strLine, d);
	for (int columnID = 1; columnID < index; columnID++){
		p = strtok(NULL, d);
		if (p == NULL) break;
	}
	return p;
}

//used for parse tab delimited Configuraton/Status files
string StringUtil::splitDelimitedSubstring(char c, string str, int index)
{
	int col = 0, left = 0, right = 0;

	for (unsigned int i = 0; i < str.size(); i++)
	{
		if (str[i] == c)
		{
			col++;

			if (col == index)
			{
				right = i;
				break;
			}
			else
				left = i;
		}
	}
	if (index != 1)
		left++;
	return str.substr(left, (right - left));
}


string StringUtil::ltrim(const string &str, const string &whitespace)
{

	unsigned int idx = str.find_first_not_of(whitespace);
	if (idx != string::npos)
		return str.substr(idx);

	return "";
}

string StringUtil::rtrim(const string &str, const string &whitespace)
{

	unsigned int idx = str.find_last_not_of(whitespace);
	if (idx != string::npos)
		return str.substr(0, idx + 1);

	return str;
}

string StringUtil::trim(const string &str, const string &whitespace)
{

	return rtrim(ltrim(str, whitespace), whitespace);
}

unsigned char* StringUtil::getSmallestLargerValue(char* value, int valsize)
{
	unsigned char* ptr = NULL;
	unsigned char ascchar;
	unsigned char temparea[100];
	//int valsize = strlen(value);
	//assert(valsize < 100); //The value size should less than 100

	memset(temparea, 0, 100);
	memcpy(temparea, value, valsize);
	ptr = temparea + valsize - 1; //get the last charactor of the char string

	for (int i = 0; i<valsize; i++){
		ascchar = *ptr;
		ascchar++;
		if (ascchar == 0)ptr--;
		else{
			memset(ptr, ascchar, 1);
			break;
		}
	}

	unsigned char* smallestLargerVal = new unsigned char[valsize];
	memset(smallestLargerVal, 0, valsize);
	memcpy(smallestLargerVal, temparea, valsize);

	return smallestLargerVal;

}
