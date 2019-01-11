#include "../main.h"

func(stamp)
{
	static char *label=".time.stamp";
	ret->type=type_long|type_unsign;
	ret->v.v_long=time(NULL);
	return ret;
}

func(clock)
{
	static char *label=".time.clock";
	ret->type=type_long|type_unsign;
	ret->v.v_long=clock();
	return ret;
}

func(ustamp)
{
	static char *label=".time.ustamp";
	struct timeval tv={0};
	gettimeofday(&tv,NULL);
	ret->type=type_long|type_unsign;
	ret->v.v_long=(tv.tv_sec&0x00000fffffffffffUL)*1000000+tv.tv_usec;
	return ret;
}


