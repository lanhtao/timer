#ifndef __ACTIVE_OBJECT_H__
#define __ACTIVE_OBJECT_H__

#include <assert.h>
#include <list>
#include <string>
#include <time.h>
#include "CriticalLock.h"

//Forward declare
class ActiveObject;

class ActiveObjectFactory {
public:
	typedef enum {
		Stopped, Running, Exitting
	} ActiveObjectState_t;

	typedef struct {
		std::string name;
		ActiveObjectState_t state;
		ActiveObject *obj;

		time_t lastUpdate;
	} ObjectInfo_t;

	static ActiveObjectFactory *Instance();

	template<typename T>
	T *createObject(const char *name) {
		return new T(name);
	}

	std::string getObjectInfo();
private:
	friend class ActiveObject;
	int Register(ActiveObject *obj, const char *name);
	int unRegister(ActiveObject *obj);
	int setState(ActiveObject *obj, ActiveObjectState_t state);

	std::list<ObjectInfo_t> _objects;
	static ActiveObjectFactory *_instance;
	CriticalLock _Lock;
};

class ActiveObject {
protected:
	ActiveObject(const char *name);
	ActiveObject() { _Running = 0; }
	virtual int run() = 0;
public:
	virtual ~ActiveObject();
	// // 激活主动对象，开始执行run虚函数
	int active();
	int deactive(int join = 1);
	static void *ActiveObjectHelper(void *arg);
protected:
	int _Running;
private:
	pthread_t _tid;
	ActiveObjectFactory* _Factory;
};

#endif
