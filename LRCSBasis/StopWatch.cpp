#include "StopWatch.h"

#include <fstream>
#include <iostream>
using namespace std;

StopWatch::StopWatch()
{
	totalReal = 0;
	QueryPerformanceFrequency(&m_liPerfFreq);
}

StopWatch::~StopWatch()
{
	//using namespace std;
	//cout << "Total Lapped time: " << totalReal << endl;
}

void StopWatch::start() {
	//r1 = times(&t);
	QueryPerformanceCounter(&m_liPerfStart);
}

double StopWatch::stop() {
	//clock_t diff = clock()-watch;
	//r2 = times(&u);
	QueryPerformanceCounter(&liPerfNow);
	
	//printf("user time=%f\n",
	//	((float)(u.tms_utime - t.tms_utime)) / (HZ));
	//printf("system time=%f\n",
	//	((float)(u.tms_stime - t.tms_stime)) / (HZ));
	printf("run time=%f\n",
		((double)(liPerfNow.QuadPart - m_liPerfStart.QuadPart)*1000) / m_liPerfFreq.QuadPart);
	//return (((double) diff)/((double) CLOCKS_PER_SEC))*1000;
	return (((double)(liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) / (m_liPerfFreq.QuadPart));
}

double StopWatch::stopToFile(const char* x) {
	ofstream flStream;
	flStream.open(x, ios::out | ios::app);
	flStream.seekp(ios::end);
	//clock_t diff = clock()-watch;
	QueryPerformanceCounter(&liPerfNow);

	flStream << "run time=" <<
		((double)(liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) / (m_liPerfFreq.QuadPart) << std::endl;
	//return (((double) diff)/((double) CLOCKS_PER_SEC))*1000;
	return (((double)(liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) / (m_liPerfFreq.QuadPart));
}


double StopWatch::quietStop()
{
	QueryPerformanceCounter(&liPerfNow);
	// user time!
	return (((double)(liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) / (m_liPerfFreq.QuadPart));
	//(((float)(r2-r1))/(HZ))*1000;
}

void StopWatch::lapStart() {
	start();
}
void StopWatch::lapStop() {
	totalReal += stop();
}