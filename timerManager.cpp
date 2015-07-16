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
void timerManager::start()
{
	if(!m_isRunning)
	{
		m_isRunning = true;
		active();
	}
}

void timerManager::stop()
{
	if(m_isRunning)
	{
		deactive();
		m_isRunning = false;
	}
}

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
			timer* t = it->second;
			t->exec();
		}
	}
	return 0;
}
