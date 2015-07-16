#include <assert.h>
#include <map>
#include <string>
#include <sstream>
#include "ActiveObject.h"

ActiveObjectFactory * ActiveObjectFactory::_instance = NULL;

ActiveObjectFactory *ActiveObjectFactory::Instance()
{
	if(_instance == NULL)
		_instance = new ActiveObjectFactory();
		
	return _instance;
}

int ActiveObjectFactory::Register(ActiveObject *obj, const char *name)
{
	ObjectInfo_t info;

	info.name = name;
	info.state= Stopped;
	info.obj  = obj;
	
	time(&info.lastUpdate);
	
	_Lock.Lock();
	_objects.push_back(info);
	_Lock.Unlock();
	
	return 0;
}

int ActiveObjectFactory::unRegister(ActiveObject *obj)
{
    std::list<ObjectInfo_t>::iterator iter;		
	
	  _Lock.Lock();
	  for(iter = _objects.begin(); iter != _objects.end(); iter ++)
	  {
	  	  if(iter->obj == obj)
	  	  {
	  	      _objects.erase(iter);
	  	      break;
	  	  }
	  }
  	
	  _Lock.Unlock();
	  return 0;
}

int ActiveObjectFactory::setState(ActiveObject *obj, ActiveObjectState_t state)
{
    std::list<ObjectInfo_t>::iterator iter;		
	
	  _Lock.Lock();
	  for(iter = _objects.begin(); iter != _objects.end(); iter ++)
	  {
	  	  if(iter->obj == obj)
	  	  {
					  iter->state = state;
						time(&iter->lastUpdate);
	  	      break;
	  	  }
	  }
	  _Lock.Unlock();
	  
	  return 0;
}
		
std::string ActiveObjectFactory::getObjectInfo()
{
    std::list<ObjectInfo_t>::iterator iter;		
    std::ostringstream ostr;	  	
    
	  for(iter = _objects.begin(); iter != _objects.end(); iter ++)
	  {
	  	  ostr << "name=" << iter->name << "\tobj=" << (unsigned long)iter->obj << "\tstate=" << iter->state << std::endl;
	  }
	  
	  return ostr.str();
}

ActiveObject::ActiveObject(const char *name)
	:_Factory(ActiveObjectFactory::Instance())
{
	_Running = 0;
	_Factory->Register(this, name);
}

ActiveObject::~ActiveObject()
{
	_Factory->unRegister(this);
}

int ActiveObject::active()
{
    int err;
    assert(_Running == 0);
	
	  _Running = 1;
	  
	  err = pthread_create(&_tid, NULL, ActiveObjectHelper, this);
  	if(err)
	  {
		    _Running = 0;
	  } else {
    	  _Factory->setState(this, ActiveObjectFactory::Running);
    }
  
    return err ? -1 : 0;		
}


int ActiveObject::deactive(int join )
{
    assert(_Running);
	
	  _Running = 0;
	  	
	  _Factory->setState(this, ActiveObjectFactory::Exitting);

	  if(join) pthread_join(_tid, NULL);

	  return 0;
}


void *ActiveObject::ActiveObjectHelper(void * arg)
{
	  ActiveObject *obj = (ActiveObject *)arg;
	  
	  while(obj->_Running)
	  {
	      obj->_Factory->setState(obj, ActiveObjectFactory::Running);
	      obj->run();
	  }
	  
    obj->_Factory->setState(obj, ActiveObjectFactory::Stopped);
	  return NULL;
}
