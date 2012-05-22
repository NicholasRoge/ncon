/******************************************************************************\
|File:  /src/headers/_fileioexception.cpp									   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "_fileioexception.h"

FileIOException::FileIOException(FileIOException::ErrCode code):Exception("",code){
	switch(code){
		case FILE_NOT_EXIST:
			this->_message="An attempt was made to open a file in read only mode that does not exist.";
			break;
	}
}

FileIOException::FileIOException(String errorDescription,unsigned errorCode):Exception(errorDescription,errorCode){
}
