#pragma once

#include <string>
#include "AtlBase.h"
#include "AtlConv.h"
class LRCSAPI_API LRCSValue
{
public:
	enum LRCSValueType{
		NOTYPE = 0,
		INTTYPE = 1,
		FLOATTYPE = 2,
		STRINGTYPE = 3,
		LONGTYPE = 4,
		DOUBLETYPE = 5
	};

public:
	LRCSValue();
	LRCSValue(BYTE* pData, int nDataSize, LRCSValueType dataType);
	LRCSValue(const LRCSValue& src);
	~LRCSValue();

	LRCSValue* operator = (const LRCSValue& src);

	void SetValue(BYTE* pData, int nDataSize, LRCSValueType dataType);

	template<class T>
	T GetValue()
	{
		return *((T*)m_pValueData);
	}

	template<>
	tstring GetValue<tstring>()
	{
		USES_CONVERSION;

		char* szData = new char[m_nDataSize + 1];
		memset(szData, 0, m_nDataSize + 1);

		strncpy_s(szData, m_nDataSize + 1, (char*)m_pValueData, m_nDataSize);

		CA2T tsz(szData);
		delete[] szData;
		return tsz;
	}

	LRCSValueType	ValueType(){ return m_eValueType; }
	LRCSValueType	ValueType(LRCSValueType type){ 
		m_eValueType = type;
		return m_eValueType; 
	}

protected:
	BYTE*			m_pValueData;
	int				m_nDataSize;
	LRCSValueType	m_eValueType;

};

