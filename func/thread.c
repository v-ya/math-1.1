#include "../main.h"

// .thread.* 线程相关函数

func(thread_self)
{
	ret->type=type_long|type_unsign;
	ret->v.v_long=pthread_self();
	return ret;
}


