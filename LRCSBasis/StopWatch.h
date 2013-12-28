#pragma once
#include <stdio.h>
#include <iostream>
#include <windows.h>

class __declspec(dllexport) StopWatch
{
public:
	StopWatch(); //start counting time
	~StopWatch();
	// starts counting 
	void start();
	// returns number of milliseconds since start
	double stop();
	double stopToFile(const char* x);

	double quietStop();

	//static unsigned long ticks();
	//static double secondsPerTick();

	void lapStart();
	void lapStop();
private:
	//clock_t watch;
	//struct tms t, u;
	//long r1, r2;
	double totalReal;
	LARGE_INTEGER m_liPerfFreq;
	LARGE_INTEGER m_liPerfStart;
	LARGE_INTEGER liPerfNow;
};

