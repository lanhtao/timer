#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/timerfd.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "timer.h"
#include <signal.h>

//typedef int (*timer_callback)(void* userdata, int len); //user callback

timer::timer(uint32 time,  void* userdata, int len):m_circle(false),m_type(NIL)
{
	m_interval.interval = time;
	m_interval.userdata = userdata;
	m_interval.len = len;
	if(init() == -1)
	{
		printf("timer init error\n");
	}
}

timer::timer(std::string time, void* userdata, int len):m_circle(false),m_type(NIL)
{
	struct tm st_time;
	unsigned long interval;
	struct timeval now;
	time_t scheduledTime;//预定的时间

	char* str = strptime(time.c_str(), "%Y-%m-%d %H:%M:%S", &st_time);
	if(str == NULL)
			printf("time format error");
	scheduledTime = mktime(&st_time);
	gettimeofday(&now, NULL);
	interval = scheduledTime *1000 - (now.tv_sec * 1000 + now.tv_usec/1000);
	printf("time = %ld\n",interval);

	m_interval.interval = interval;
	m_interval.userdata = userdata;
	m_interval.len = len;

	if(init() == -1)
	{
		printf("timer init error\n");
	}
}



timer::~timer()
{
	stop();
}

/**
 * 初始化定时器;
 * 失败返回-1，成功返回描述法
 */
int timer::init()
{
	int fd = timerfd_create(CLOCK_REALTIME, 0);
	if(fd == -1)
	{
		return -1;
	}

	m_interval.timerFd = fd;
	//timerfd_create的flags参数必须为0
	//设置为阻塞方式读写
	int flags = fcntl(fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if (-1 == fcntl(fd, F_SETFL, flags)) {
		return -1;
	}
	return fd;
}

void timer::setTimerType(bool circle,TimerType type)
{
	m_circle = circle;
	m_type = type;
}

/**
 * 启动定时器:成功返回定时时间，失败返回-1
 *
 */
uint32 timer::start()
{
	struct itimerspec time;//it_value用于指定当前的定时器到期时间。当定时器到期，it_value的值会被更新成it_interval 的值
	memset(&time, 0, sizeof(time));
	time.it_value.tv_sec =  m_interval.interval/1000;
	time.it_value.tv_nsec = (m_interval.interval%1000) * 1000000;
#if 1
	if(m_circle)
	{
		switch(m_type)
		{
			case DAY:
				time.it_interval.tv_sec  = 24*60*60*1000;
				time.it_interval.tv_nsec = 0;
				break;
			case WEEK:
				time.it_interval.tv_sec  = 7*24*60*60*1000;
				time.it_interval.tv_nsec = 0;
				break;
			/*
			case MONTH://数据太大，溢出
				time.it_interval.tv_sec  = 30*24*60*60*1000;
				time.it_interval.tv_nsec = 0;
				break;
			case YEAR://数据太大，溢出
				time.it_interval.tv_sec  = 360*24*60*60*1000;
				time.it_interval.tv_nsec = 0;
				break;
			*/
			case REPEAT:
				time.it_interval.tv_sec  = m_interval.interval/1000;
				time.it_interval.tv_nsec = (m_interval.interval%1000) * 1000000;	
				break;
			default:
				break;
		}
	}
#endif
	timerfd_settime(m_interval.timerFd, 0, &time, NULL);
	
	return m_interval.interval;
}

int   timer::stop()
{
	if(!m_circle)
	{
	    	close(m_interval.timerFd);
		return 0;
	}
	return -1;
}

int timer::getTimerfd()
{
	return m_interval.timerFd;
}
