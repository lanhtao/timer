
#ifndef __CRITICAL_LOCK__
#define __CRITICAL_LOCK__

#include <pthread.h>

struct CriticalLock
{
      pthread_mutex_t _Mutex;
 	  pthread_mutexattr_t attr;
    
	  CriticalLock(int recursive = 0)  
	  {   
 	  	  pthread_mutexattr_init(&attr);
	  	  if(recursive)
	  	  {
	  	  	  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	  	  }
	  	  pthread_mutex_init(&_Mutex, &attr); 
	  }
	  ~CriticalLock() { pthread_mutex_destroy(&_Mutex);  pthread_mutexattr_destroy(&attr);}
	  
	  void Lock()   { pthread_mutex_lock(&_Mutex);   }
	  void Unlock() { pthread_mutex_unlock(&_Mutex); }
};

#endif
