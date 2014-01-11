#pragma once

#include <omp.h>

class ParallelHelper
{
public:
	ParallelHelper();
	~ParallelHelper();

public:
	static inline int GetThreadNumber(int nLoopNum)
	{
		const int g_ncore = omp_get_num_procs();
		const int MIN_ITERATOR_NUM = 4;
		int max_tn = nLoopNum / MIN_ITERATOR_NUM;
		int tn = max_tn > g_ncore ? g_ncore : max_tn;
		if (tn < 1)
		{
			tn = 1;
		}
		return tn;
	}
};

