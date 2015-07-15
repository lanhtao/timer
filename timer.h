#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_
#include <string>
#include "common.h"
#include "Signal.h"

struct timerData
{
	int 	timerFd;			//定时器的文件描述符
	uint32 	interval;	//定时时间,单位毫秒
	void* 	userdata;	//回调函数的数据指针
	int   	len;		//数据长度
};
class timer
{
public:
	timer(uint32 time,  void* userdate, int len);
	//年月日时间点类型定时.时间格式 2015-07-12 12:00:00
	timer(std::string time, void* userdate, int len);
	int getTimerfd();
	uint32 start();
	void stop();
	virtual ~timer();
		//注册超时函数
	template<class CLASS>
	void AttachTimeoutFuc(CLASS* pObj, void (CLASS::* func)(void* , int ))
	{
		m_timeout.Connect(pObj, func);
	}
	//解注册
	template<class CLASS>
	void DetachTimeoutFuc(CLASS* pObj, void (CLASS::* func)(void* , int ))
	{
		m_timeout.Disconnect(pObj, func);
	}
	void emit()
	{
		m_timeout.Emit(m_interval.userdata ,m_interval.len);
	}
private:
	int init();
	timerData m_interval;
		//信号源
	Gallant::Signal2<void* , int >  m_timeout;
};

#endif /* SRC_TIMER_H_ */
