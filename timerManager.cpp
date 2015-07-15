#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "timerManager.h"

timerManager::timerManager():ActiveObject("timerManager"),m_fd(0),m_isRunning(false),m_events(NULL)
{
	if(init() == -1)
	{
		printf("init error\n");
	}
}

timerManager::~timerManager()
{
	free(m_events);
	close(m_fd);
}

/**
 * 初始化定时器：
 * 1.创建epool文件描述符
 * 2.创建事件集合 
 */
int timerManager::init()
{
	m_fd = epoll_create1 (0);
	if (m_fd == -1)
	{
	  printf ("epoll_create");
	  return -1;
	}
	m_events = (struct epoll_event*) malloc (MAXEVENTS *sizeof (struct epoll_event));
	if(m_events == NULL)
	{
		return -1;
	}
	return 0;
}

/**
*将小于2天的定时器开启定时
*/
void timerManager::managerTimerArray(void* userdata, int len)
{
	for(int i = 0; i<MAX_NUM_TIMER; i++ )
	{
		//如果小于2天，在此处开启定时，否则，定时器时间减 1 天。
		//只关心定时时间小于2天的定时器
		
	}
}

/**
 * 开启定时器管理线程
 * 
 */
void timerManager::start()
{
	if(!m_isRunning)
	{
		m_isRunning = true;
		active();
	}
	m_timerManager = new timer(24*60*60*1000,NULL,0);
	m_timerManager->setTimerType(true,REPEAT);
	m_timerManager->AttachTimeoutFuc(this,&timerManager::managerTimerArray);
}

/**
 * 停止定时器管理线程
 */
void timerManager::stop()
{
	if(m_isRunning)
	{
		deactive();
		m_isRunning = false;
	}
}

/**
 * 增加定时器到epool，并开启线程进行事件监听；
 * 定时器实际启动的地方
 */
int timerManager::addTimer(timer* t)
{
	int fd = t->getTimerfd();
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET;

	int ret = epoll_ctl(m_fd, EPOLL_CTL_ADD, fd, &ev);
	if(ret == -1)
	{
		return -1;
	}
	m_timersMap[fd] = t;
	t->start();
	if(!m_isRunning)
	{
		start();
	}
	return 0;
}

/**
 * 移出定时器
 */
int timerManager::removeTimer(timer* t)
{
	int ret = t->stop();
	if(ret == 0)
	{
		int fd = t->getTimerfd();

		struct epoll_event ev;
		ev.data.fd = fd;
		ev.events = EPOLLIN | EPOLLET;
		epoll_ctl(m_fd, EPOLL_CTL_DEL, fd, &ev);

		m_timersMap.erase(fd);
	}
	return 0;
}

/**
 * 时间循环监听定时器事件，并执行定时器回调
 */
int timerManager::run()
{
	int nfds = epoll_wait (m_fd, m_events, MAXEVENTS, -1);
	for (int i = 0; i < nfds; i++)
	{
		if ((m_events[i].events & EPOLLERR) || (m_events[i].events & EPOLLHUP) || (!(m_events[i].events & EPOLLIN)))
		{
			/* An error has occured on this fd, or the socket is not
			   ready for reading (why were we notified then?) */
			printf ( "epoll error\n");
			close (m_events[i].data.fd);
			continue;
		}
		std::map<int, timer*>::iterator it = m_timersMap.find(m_events[i].data.fd);
		if (it != m_timersMap.end())
		{
			char buf[10];
			int ret = 0 ;
			/*定时器超时，需要把数据读出来，否则会出现堵塞，重复定时效果失效。读出的数据只有1个*/
			while (ret = read(m_events[i].data.fd, buf, 10) > 0)
			{
                    		//read all data
				//printf("%d\n",ret);//ret = 1
                	}
			//定时器时间到
			timer* t = it->second;
			t->emit(); //发送超时信号
			removeTimer(t);
		}
	}
	return 0;
}
