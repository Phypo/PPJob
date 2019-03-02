#include "PPUtils.h"


namespace PPUtils{ 


	PPTimeMicro70 GetMicroTime70()
	{
		struct timeval tp;
		::gettimeofday( &tp, NULL);
		return (tp.tv_sec * 1000000) + tp.tv_usec ;
	}


};
