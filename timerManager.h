#ifndef SRC_TIMERMANAGER_H_
#define SRC_TIMERMANAGER_H_

#include <sys/epoll.h>
#include <map>
#include "ActiveObject.h"
#include "timer.h"

#define MAXEVENTS 64
#define MAX_NUM_TIMER 128

class timerManager:public ActiveObject
{
	public:
		timerManager();
		void start();
		void stop();
		int addTimer(timer* t);
		int removeTimer(timer* t);
		virtual ~timerManager();
	private:
		void managerTimerArray(void* userdata, int len);
		int init();
		int  run();
		timer* m_timerManager;
		int m_timer[MAX_NUM_TIMER];
		int 	m_fd;
		bool 	m_isRunning;
		struct epoll_event *m_events;
		std::map<int, timer*> m_timersMap;
};

#endif /* SRC_TIMERMANAGER_H_ */
