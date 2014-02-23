#include "stdafx.h"
#include "LRCSValue.h"
#include "AtlBase.h"
#include "AtlConv.h"

LRCSValue::LRCSValue()
{
	m_pValueData = NULL;
	m_nDataSize = 0;
	m_eValueType = NOTYPE;
}

LRCSValue::LRCSValue(BYTE* pData, int nDataSize, LRCSValueType type)
{
	m_pValueData = new BYTE[nDataSize];
	memcpy(m_pValueData, pData, nDataSize);

	m_nDataSize = nDataSize;
	m_eValueType = type;
}

LRCSValue::LRCSValue(const LRCSValue& src)
{
	m_pValueData = new BYTE[src.m_nDataSize];
	memcpy(m_pValueData, src.m_pValueData, src.m_nDataSize);

	m_nDataSize = src.m_nDataSize;
	m_eValueType = src.m_eValueType;
}

LRCSValue* LRCSValue::operator = (const LRCSValue& src)
{
	if (&src != this)
	{
		SetValue(src.m_pValueData, src.m_nDataSize, src.m_eValueType);
	}

	return this;
}

void LRCSValue::SetValue(BYTE* pData, int nDataSize, LRCSValueType dataType)
{
	if (m_pValueData != NULL)
	{
		delete[] m_pValueData;
		m_pValueData = NULL;
	}

	m_pValueData = new BYTE[nDataSize];
	memcpy(m_pValueData, pData, nDataSize);

	m_nDataSize = nDataSize;
	m_eValueType = dataType;
}
LRCSValue::~LRCSValue()
{
	if (m_pValueData != NULL)
	{
		delete[] m_pValueData;
		m_pValueData = NULL;
	}
}
