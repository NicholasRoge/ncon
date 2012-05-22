/******************************************************************************\
|File:  /src/headers/exception.h											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>

#include "string.h"

/***************************************************************\
|Class[ Exception ]:											|
|	This class is to be used to be used as a container when		|
|	throwing exceptions.  It is also very useful for			|
|	inheritance.												|
\***************************************************************/
class Exception{
	protected:
		unsigned _eCode;
		String _message;

	public:
		/***************************************************************\
		|Method[ Exception ]:											|
		|	Constructor;  Sets the _eCode and _message variables to		|
		|	the appropriate values as indicated by its parameters.		|
		|																|
		|Param[ message ]:												|
		|	Message to be thrown.										|
		|Param[ eCode ]:												|
		|	Unique identifier for the message to be thrown.  This is 	|
		|	useful in which some exceptions can be safely ignored, or	|
		|	when checking if a particular exception has been thrown.	|
		\***************************************************************/
		Exception(String message,unsigned eCode);

		/***************************************************************\
		|Method[ getCode ]:												|
		|	Standard getter method.										|
		|Return:														|
		|	returns private variable _eCode.							|
		\***************************************************************/
		unsigned getCode();
		
		/***************************************************************\
		|Method[ getMessage ]:											|
		|	Standard getter method.										|
		|Return:														|
		|	returns private variable _message.							|
		\***************************************************************/
		String getMessage();
};

#endif