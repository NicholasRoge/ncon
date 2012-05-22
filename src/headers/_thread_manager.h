/******************************************************************************\
|File:  /src/headers/_thread_manager.h										   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "list.h"
#include "string.h"

class Thread;
typedef void* (*Runner)(void* args);

class ThreadManager{
	private:
		List<Thread*> threads;

	public:
		ThreadManager();
		~ThreadManager();

		Thread* getNewThread(String threadName,Runner runner,void *args,bool critical=false);

		Thread* getThreadAt(unsigned index);

		Thread* getThreadByName(String threadName);

		unsigned getNumThreads();
};

extern ThreadManager ThreadMan;

#endif