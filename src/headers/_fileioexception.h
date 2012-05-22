/******************************************************************************\
|File:  /src/headers/_fileioexception.h 									   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef FILEIOEXCEPTION_H
#define FILEIOEXCEPTION_H

#include "exception.h"
#include "string.h"

class FileIOException:public Exception{
	public:
		enum ErrCode{
			FILE_NOT_EXIST
		};

		FileIOException(ErrCode code);
		FileIOException(String errorDescription,unsigned errorCode);
};

#endif
