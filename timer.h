#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_
#include "common.h"

typedef int (*timer_callback)(void* userdata, int len); //user callback
struct timerData
{
	int 	timerFd;			//定时器的文件描述符
	uint32 	interval;	//定时时间
	void* 	cb;			//定时计时到，执行的回调函数
	void* 	userdata;	//回调函数的数据指针
	int   	len;		//数据长度
};
class timer
{
public:
	timer(uint32 time, void* cb, void* userdate, int len);
	int getTimerfd();
	void* getCallBack();
	void exec();
	uint32 start();
	void stop();
	virtual ~timer();
private:
	int init();
	timerData m_interval;
};

#endif /* SRC_TIMER_H_ */
