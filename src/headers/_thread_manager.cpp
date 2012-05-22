/******************************************************************************\
|File:  /src/headers/_thread_manager.cpp									   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "_thread_manager.h"
#include "_thread.h"

ThreadManager::ThreadManager(){
}

ThreadManager::~ThreadManager(){
	for(unsigned index=0;index<this->threads.getSize();index++){
		delete this->threads.getElementAt(index);
	}
}

Thread* ThreadManager::getNewThread(String threadName,Runner runner,void *args,bool critical){
	return this->threads.getElementAt(this->threads.addElement(new Thread(threadName,runner,args,critical))-1);
}

Thread* ThreadManager::getThreadAt(unsigned index){
	if(index<0||index>this->threads.getSize()){
		return NULL;
	}

	return this->threads.getElementAt(index);
}

Thread* ThreadManager::getThreadByName(String threadName){
	for(unsigned index=0;index<this->threads.getSize();index++){
		if(this->threads[index]->getName()==threadName){
			return this->threads[index];
		}
	}

	return NULL;
}

unsigned ThreadManager::getNumThreads(){
	return this->threads.getSize();
}

ThreadManager ThreadMan;