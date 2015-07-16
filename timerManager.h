#ifndef SRC_TIMERMANAGER_H_
#define SRC_TIMERMANAGER_H_

#include <sys/epoll.h>
#include <map>
#include "ActiveObject.h"
#include "timer.h"

#define MAXEVENTS 64


class timerManager:public ActiveObject
{
public:
	timerManager();
    void start();
    void stop();
    int addTimer(timer* t);
	virtual ~timerManager();
private:
	int init();
    int  run();
    int 	m_fd;
    bool 	m_isRunning;
    struct epoll_event *m_events;
    std::map<int, timer*> m_timersMap;
};

#endif /* SRC_TIMERMANAGER_H_ */
