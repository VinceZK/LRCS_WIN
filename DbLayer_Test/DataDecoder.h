#pragma once

#ifdef DBLAYER_TEST_EXPORTS
#define DBLAYER_TEST_API __declspec(dllexport)
#else
#define DBLAYER_TEST_API __declspec(dllimport)
#endif

class DBLAYER_TEST_API DataDecoder
{
public:
	DataDecoder(void);
	~DataDecoder(void);

public:

};

