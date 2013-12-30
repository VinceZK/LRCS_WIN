
// LRCS_TestData.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CLRCS_TestDataApp:
// See LRCS_TestData.cpp for the implementation of this class
//

class CLRCS_TestDataApp : public CWinApp
{
public:
	CLRCS_TestDataApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CLRCS_TestDataApp theApp;