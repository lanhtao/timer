#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_
#include <string>
#include "common.h"
#include "Signal.h"

/*
	如果定时器定时时间过长，interval会溢出。此时采取策略：将间隔时间大于1天的定时器暂且存在数组里
*/
struct timerData
{
	int 	timerFd;			//定时器的文件描述符
	uint32 	interval;	//定时时间,单位毫秒
	void* 	userdata;	//回调函数的数据指针
	int   	len;		//数据长度
};
typedef enum 
{
	NIL,
	DAY ,
	WEEK,
	MONTH,
	YEAR,
	REPEAT
}TimerType;

class timer
{
public:
	timer(uint32 time,  void* userdate, int len);
	//年月日时间点类型定时.时间格式 2015-07-12 12:00:00
	timer(std::string time, void* userdate, int len);
	int getTimerfd();
	void setTimerType(bool circle,TimerType type);
	uint32 start();
	int  stop();
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
	bool m_circle ; //是否需要循环定时
	TimerType m_type;//循环类型
	//信号源
	Gallant::Signal2<void* , int >  m_timeout;
};

#endif /* SRC_TIMER_H_ */
