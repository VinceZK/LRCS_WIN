// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



// TODO: reference additional headers your program requires here

#ifdef LRCSAPI_EXPORTS
#define LRCSAPI_API	__declspec(dllexport)
#else
#define LRCSAPI_API	__declspec(dllimport)
#endif

#pragma warning (disable:4251)

#ifdef UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif
