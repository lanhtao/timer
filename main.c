#include "timer.h"
#include "timerManager.h"
#include <stdio.h>

int func1(void* userdata, int len)
{
	printf("func 1\n");
	return 1;
}

int func2(void* userdata, int len)
{
	printf("func 2\n");
	return 1;
}

int func3(void* userdata, int len)
{
	printf("func 3\n");
	return 1;
}
int main()
{
  timer t1(2000,(void*)(func1),NULL,1);
	timer t2(4000,(void*)(func2),NULL,1);
	timer t3(10000,(void*)(func3),NULL,1);
	timerManager manager;
	if(manager.addTimer(&t1))
	{
		printf("error\n");
	}
	manager.addTimer(&t2);
	manager.addTimer(&t3);
	return 0;
}
