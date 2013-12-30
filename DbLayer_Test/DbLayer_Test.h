// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DBLAYER_TEST_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DBLAYER_TEST_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DBLAYER_TEST_EXPORTS
#define DBLAYER_TEST_API __declspec(dllexport)
#else
#define DBLAYER_TEST_API __declspec(dllimport)
#endif

#include <string>
#include <memory>
#include <vector>

class Db;
class Dbc;
class Dbt;

// This class is exported from the DbLayer_Test.dll
class DBLAYER_TEST_API CDbLayer_Test {
public:
	CDbLayer_Test(void);
	~CDbLayer_Test();
	// TODO: add your methods here.

public:
	bool InitDb(LPCTSTR szDbFile);
	void DestroyDb();

	std::vector<tstring> GetFirstData();
	std::vector<tstring> GetNextData();

private:
	tstring ToTString(char* psz, int nLen);
private:
	bool m_bInit;
	tstring	m_strDbFile;

	std::shared_ptr<Db>	m_spDb;
	Dbc*	m_pCursor;
};

extern DBLAYER_TEST_API int nDbLayer_Test;

DBLAYER_TEST_API int fnDbLayer_Test(void);
