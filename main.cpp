#include <unistd.h>
#include <stdlib.h>
#include "timer.h"
#include "timerManager.h"
#include <stdio.h>
#include "Test.h"

int main(int argc,char** argv)
{
	//ModbusDataCenter center;
	//center.start();
	Test test;
	timer t1(argv[1],NULL,1);
	t1.AttachTimeoutFuc(&test, &Test::func1);
	timer t2(4000,NULL,1);
	t2.AttachTimeoutFuc(&test, &Test::func2);
	timer t3(10000,NULL,1);
	t3.AttachTimeoutFuc(&test, &Test::func3);
	timerManager manager;
	if(manager.addTimer(&t1))
	{
		printf("error\n");
	}
	manager.addTimer(&t2);
	manager.addTimer(&t3);
	while(1)
	{
		system("date");
		sleep(1);
	}

	return 0;
}
