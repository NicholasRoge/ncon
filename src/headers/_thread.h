/******************************************************************************\
|File:  /src/headers/_thread.h  											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include "string.h"


typedef void* (*Runner)(void* args);

/***************************************************************\
|Class[ Thread ]:												|
|	The intent behind the following class is simply to make the	|
|	management of threads within NCon to be much easier.		|
\***************************************************************/
class Thread{
	private:
		String _name;

		pthread_t _t_id;
		pthread_attr_t _attributes;

		bool _running;
		bool _critical;

		Runner _runner;
		void* _arguments;		

		/***************************************************************\
		|Method[ _wrapper ]:											|
		|	This is the method that is actually run in the				|
		|	pthread_create function.  It then calls the function stored	|
		|	in _runner, with arguments _arguments.						|
		|Return:														|
		|	Returns the value that _runner returns.						|
		|																|
		|Param[ args ]:													|
		|	Will always be the context of the current running thread	|
		|	including it's member data and methods.						|
		\***************************************************************/
		static void* _wrapper(void* args);

	public:
		/***************************************************************\
		|Method[ Thread ]:												|
		|	Constructor;  Initializes all member variables to their		|
		|	values as set by the parameters.							|
		|																|
		|Param[ threadName ]:											|
		|	This parameter has no bearing on the thread itself.  It		|
		|	simply helps to get a handle to the thread, and in			|
		|	identification.												|
		|Param[ runner ]:												|
		|	Handle of the method to be run by the thread.				|
		|Param[ args ]:													|
		|	Arguments to be passed into the runner function.			|
		|Param[ critical ]:												|
		|	Variable to define if this thread needs to run until		|
		|	completion, before being shut down.							|
		\***************************************************************/
		Thread(String threadName,Runner runner,void* args,bool critical=false);

		/***************************************************************\
		|Method[ start ]:												|
		|	Starts the thread.											|
		|Return:														|
		|	Returns the id of the thread created.						|
		\***************************************************************/
		int start();

		/***************************************************************\
		|Method[ stop ]:												|
		|	Stops the thread.											|
		\***************************************************************/
		void stop();

		/***************************************************************\
		|Method[ waitForThread ]:										|
		|	Locks the calling thread in this method while the thread	|
		|	_runner is still running.									|
		|Return:														|
		|	Returns the value that _runner returns.						|
		\***************************************************************/
		void* waitForThread();

		/***************************************************************\
		|Method[ isRunning ]:											|
		|	Simple check to tell whether or not the current thread is	|
		|	running.													|
		|Return:														|
		|	Returns true if the thread is running, and false otherwise.	|
		\***************************************************************/
		bool isRunning();

		/***************************************************************\
		|Method[ isCritical ]:											|
		|	Checks to see if the thread is a critical thread.			|
		|Return:														|
		|	Returns true if the thread is critical, and false otherwise.|
		\***************************************************************/
		bool isCritical();
		
		/***************************************************************\
		|Method[ getName ]:												|
		|	Simple getter method returning the value of the _name		|
		|	variable.													|
		|Return:														|
		|	Returns the value of the _name variable.					|
		\***************************************************************/
		String getName();
};

#endif