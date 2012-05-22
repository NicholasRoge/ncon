/******************************************************************************\
|File:  /src/headers/mutex.cpp  											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "mutex.h"

#include <sys/types.h>

Mutex::Mutex():_locked(false){
}

void Mutex::_lockAndWait(){
	if(!this->_locked){
		this->_locked=true;
	}else{
		this->_threadWaitQueue.addElement(getpid());
		while(this->_isInQueue(getpid()));  //Just wait.
	}
}

void Mutex::_unlock(){
	if(this->_threadWaitQueue.getSize()==0){  //If this is the last write in the queue
		this->_locked=false;
	}else{
		this->_threadWaitQueue.removeElementAt(0);
	}
}

bool Mutex::_isInQueue(unsigned threadId){
	if(this->_threadWaitQueue.getSize()==0){
		return false;
	}

	for(unsigned index=0;index<this->_threadWaitQueue.getSize();index++){
		if(this->_threadWaitQueue[index]==threadId){
			return true;
		}
	}

	return false;
}