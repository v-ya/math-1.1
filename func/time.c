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


