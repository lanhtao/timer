#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/timerfd.h>
#include <string.h>
#include <stdio.h>
#include "timer.h"

//typedef int (*timer_callback)(void* userdata, int len); //user callback

timer::timer(uint32 time, void* cb, void* userdata, int len)
{
	m_interval.interval = time;
	m_interval.cb = cb;
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
	timerfd_settime(m_interval.timerFd, 0, &time, NULL);
	return m_interval.interval;
}

void  timer::stop()
{
    close(m_interval.timerFd);
}

int timer::getTimerfd()
{
	return m_interval.timerFd;
}

void* timer::getCallBack()
{
	return m_interval.cb;
}

void timer::exec()
{
	 ((timer_callback) m_interval.cb)(m_interval.userdata,m_interval.len);
}
