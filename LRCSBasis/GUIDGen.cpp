#include "stdafx.h"
#include "GUIDGen.h"
#include "UnexpectedException.h"
#include <objbase.h>
#include <stdio.h>

GUIDGen::GUIDGen()
{
}


GUIDGen::~GUIDGen()
{
}

string GUIDGen::getGUID(){
	char buffer[33] = { 0 };
	GUID guid;
	if (CoCreateGuid(&guid) == S_OK)
	{
		_snprintf_s(buffer, sizeof(buffer),
		   "%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X",
			guid.Data1,
			guid.Data2,
			guid.Data3,
			guid.Data4[0], guid.Data4[1],
			guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]);
	}
	else{
		throw new  UnexpectedException("GUIDGen: GUID generate error!");
	}
	//return buffer;
	return string(buffer);
}
