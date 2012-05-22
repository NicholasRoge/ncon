/******************************************************************************\
|File:  /src/headers/mutex.h      											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef MUTEX_H
#define MUTEX_H

#include "list.h"

/***************************************************************\
|Class[ Mutex ]:                                                |
|   Attempts to allow mutual exclusing for any classes which    |
|   inherit this class.                                         |
\***************************************************************/
class Mutex{
	protected:
		bool _locked;
		List<unsigned> _threadWaitQueue;

        /***************************************************************\
        |Method[ Mutex ]:                                               |
        |   Constructor; Simply initializes the objects variables to    |
        |   default values.                                             |
        \***************************************************************/
		Mutex();

		/***************************************************************\
		|Method[ _lockAndWait ]:										|
		|	Allows the first thread to call this methods exclusive		|
		|	access to the file, and adding each subsequent call by a	|
		|	thread to the wait queue.  This thread also makes the		|
		|	subsequent calling thread wait until it's no longer in the  |
		|   wait queue.                                             	|
		\***************************************************************/
		void _lockAndWait();

        /***************************************************************\
        |Method[ _unlock ]:                                             |
        |   Removes the lock for the calling thread, allowing the next  |
        |   thread in the queue to begin running.                       |
        \***************************************************************/
		void _unlock();
        
        /***************************************************************\
        |Method[ _isInQueue ]:                                          |
        |   Method to check it the thread identified by threadId is in  |
        |   the wait queue.                                             |
        |Return:                                                        |
        |   Returns true if the thread is in the queue, and false       |
        |   otherwise.                                                  |
        |                                                               |
        |Param[ threadId ]:                                             |
        |   Id of the thread to check for in the wait queue.            |
        \***************************************************************/
		bool _isInQueue(unsigned threadId);
};

#endif