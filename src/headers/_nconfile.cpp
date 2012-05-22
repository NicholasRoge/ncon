/******************************************************************************\
|File:  /src/headers/_nconfile.cpp											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "_nconfile.h"

#include <errno.h>
#include "kernel.h"
#include "list.h"
#include "system.h"

NConFile::NConFile(String file,Mode mode,bool createIfNotExists){
	this->_fileLocation=file;
	this->_mode=mode;
	this->_handle=NULL;
	
	switch(this->_mode){
		case READ_ONLY:
			if(!FileExists(this->_fileLocation)){
				throw FileIOException(FileIOException::FILE_NOT_EXIST);
			}
			this->_handle=fopen(this->_fileLocation,"rb");
			break;
		case READ_WRITE:
			if(!FileExists(this->_fileLocation)){
				if(createIfNotExists){
					this->_handle=fopen(this->_fileLocation,"w+b");
				}else{
					throw FileIOException(FileIOException::FILE_NOT_EXIST);
				}
			}else{
				this->_handle=fopen(this->_fileLocation,"r+b");
			}
			break;
	}

	if(!this->_handle){
		String exceptionString="Could not open file:  ";
		exceptionString+=this->_fileLocation;
		exceptionString+=".";
		throw FileIOException(exceptionString,1);
	}

	fseek(this->_handle,0,SEEK_END);
	this->_size=ftell(this->_handle);
	fseek(this->_handle,0,0);

	if(!this->_handle){
		throw FileIOException("Could not open file.  Unknown cause.",0);
	}
}

NConFile::~NConFile(){
	if(this->_handle){
		fclose(this->_handle);
	}
}

void NConFile::changeFileMode(Mode mode){
	if(this->_handle){
		switch(mode){
			case READ_ONLY:
				freopen(this->getFileLocation(),"rb",this->_handle);
				break;
			case READ_WRITE:
				freopen(this->getFileLocation(),"r+b",this->_handle);
				break;
		}
		
		this->_mode=mode;
	}
}

String NConFile::getFileLocation() const{
	return this->_fileLocation;
}

unsigned NConFile::getSize() const{
	return _size;
}

void NConFile::append(String stringToAppend,unsigned numChars){
	if(!this->_handle){
		throw FileIOException("An attempt was made to append characters to a file that isn't opened.",1);
	}
	if(this->_mode==READ_ONLY){
		throw FileIOException("A write command was issued, but the file is opened in READ_ONLY mode.",1);
	}

	this->_lockAndWait();
	
	if(ftell(this->_handle)!=this->_size){
		fseek(this->_handle,0,SEEK_END);
	}
	
	fwrite(stringToAppend,1,numChars,this->_handle);

	this->_unlock();
}

String NConFile::getString(long startPosition,const unsigned numChars){
	String retString="";
	
	for(unsigned counter=0;counter<numChars;counter++){
		retString+='\0';
	}
	
	if(!this->_handle){
		throw FileIOException("An attempt was made to get a string from an unopened file.",1);
	}
	if(startPosition>=this->_size){
		throw FileIOException("An attempt was made to access a location outside of the acceptable parameters of the file.",1);
	}

	this->_lockAndWait();

	fseek(this->_handle,startPosition,SEEK_SET);
	fread(retString,1,numChars,this->_handle);

	this->_unlock();

	return retString;
}

String NConFile::getLine(char delimeter,unsigned numChars){	
	String retString="";
	char nextChar='\0';

	for(unsigned counter=0;(numChars==0?true:counter<numChars);counter++){
		nextChar=fgetc(this->_handle);

		if(nextChar==delimeter||nextChar==EOF){
			return retString;
		}
		retString+=nextChar;
	}

	return retString;
}

void NConFile::clear(){
    if(this->_mode==NConFile::READ_WRITE){
        if(this->_handle){
            freopen(this->getFileLocation(),"w+b",this->_handle);
        }
    }
}