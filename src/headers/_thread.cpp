/******************************************************************************\
|File:  /src/headers/_thread.cpp											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "_thread_manager.h"
#include "_thread.h"

Thread::Thread(String threadName,Runner runner,void* args,bool critical){
	this->_name=threadName;
	this->_runner=runner,
	this->_arguments=args;
	this->_critical=critical;
	this->_running=false;

	pthread_attr_init(&(this->_attributes));
}

int Thread::start(){
	int thread_id=pthread_create(&(this->_t_id),&(this->_attributes),Thread::_wrapper,this);

	return thread_id;
}

void Thread::stop(){
	pthread_cancel(this->_t_id);
}

void* Thread::waitForThread(){
	void* return_val;
	pthread_join(this->_t_id,&return_val);

	return return_val;
}

bool Thread::isRunning(){
	return this->_running;
}

bool Thread::isCritical(){
	return this->_critical;
}

String Thread::getName(){
	return this->_name;
}

void* Thread::_wrapper(void* args){
	void* returnValue=NULL;
	
	Thread* context=(Thread*)args;
	context->_running=true;
	returnValue=context->_runner(context->_arguments);
	context->_running=false;

	return returnValue;
}