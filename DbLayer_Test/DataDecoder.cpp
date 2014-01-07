#include "stdafx.h"
#include "DataDecoder.h"
#include "AtlBase.h"
#include "AtlConv.h"
#include <memory>

#include "../LRCSBasis/LZDecoder.h"
#include "../LRCSBasis/StringDecoder.h"
#include "../LRCSBasis/Block.h"

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
		memset(pBuf, 0, 1024 * sizeof(TCHAR));
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
	{
		TCHAR pBuf[1024];
		memset(pBuf, 0, 1024 * sizeof(TCHAR));
		std::shared_ptr<Decoder> pStringDecoder(new StringDecoder(true));
		std::shared_ptr<Decoder> pDecoder(new LZDecoder(pStringDecoder.get()));
		pDecoder->setBuffer(byData);

		Block* pBlock = pDecoder->getNextBlock();
		ValPos* pVal = pBlock->getNext();

		while (pVal != NULL)
		{
			switch (pVal->type)
			{
			case ValPos::INTTYPE:
			case ValPos::LONGTYPE:
				_stprintf_s(pBuf, 1024, _T("%d"), *((int*)pVal->value));
				break;
			case ValPos::FLOATTYPE:
				_stprintf_s(pBuf, 1024, _T("%f"), *((float*)pVal->value));
				break;
			case ValPos::STRINGTYPE:
			{
				char szBuf[1024];
				strncpy_s(szBuf, pVal->getSize() + 1, (char*)pVal->value, pVal->getSize());
				_tcscpy_s(pBuf, 1024, CA2T(szBuf));
				break;
			}
			case ValPos::DOUBLETYPE:
				_stprintf_s(pBuf, 1024, _T("%f"), *((double*)pVal->value));
				break;
			default:
				//NOTYPE
				break;
			}		

			result += pBuf;
			pVal = pBlock->getNext();
		}

		break;
	}
	case eDTBitmap:
		//break;
	case eDTRLE:
		//break;
	default:
		_ASSERT(_T("Unsupported decoder type!"));
		break;
	}

	return result;
}