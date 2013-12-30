#include "stdafx.h"
#include "DataDecoder.h"
#include "AtlBase.h"
#include "AtlConv.h"
#include <memory>

DataDecoder::DataDecoder(void)
{
}


DataDecoder::~DataDecoder(void)
{
}

tstring DataDecoder::ToString(BYTE* byData, int nDataLen, DataEncodeType dt)
{
	USES_CONVERSION;

	tstring result;
	switch (dt)
	{
	case eDTInt:
	{
		TCHAR pBuf[1024];
		memset(pBuf, 0, 1024);
		_stprintf_s(pBuf, 1024, _T("%d"), *((int*)byData));
		result = pBuf;
		break;
	}
	case eDTString:
	{
		std::shared_ptr<char> pBuf(new char[nDataLen + 1]);
		memset(pBuf.get(), 0, nDataLen + 1);
		strncpy_s(pBuf.get(), nDataLen + 1, (char*)byData, nDataLen);
		result = A2CT(pBuf.get());
		break;
	}
	case eDTLZ:
		break;
	case eDTBitmap:
		break;
	case eDTRLE:
		break;
	}

	return result;
}