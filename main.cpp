#include <unistd.h>
#include <signal.h>
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
	timerManager manager;
	//timer t1(argv[1],NULL,1);
	timer t1(2000,NULL,1);
	t1.setTimerType(true,REPEAT);
	t1.AttachTimeoutFuc(&test, &Test::func1);
	if(manager.addTimer(&t1))
	{
		printf("error\n");
	}
	timer t2(3000,NULL,1);
	t2.AttachTimeoutFuc(&test, &Test::func2);
	timer t3(10000,NULL,1);
	t3.AttachTimeoutFuc(&test, &Test::func3);
	manager.addTimer(&t2);
	manager.addTimer(&t3);
	while(1)
	{
		//system("date");
		sleep(1);
	}

	return 0;
}
