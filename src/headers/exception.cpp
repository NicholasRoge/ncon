/******************************************************************************\
|File:  /src/headers/exception.cpp											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "exception.h"

Exception::Exception(String message,unsigned eCode){
	this->_message=message;
	this->_eCode=eCode;
}

unsigned Exception::getCode(){
	return this->_eCode;
}

String Exception::getMessage(){
	return this->_message;
}