/******************************************************************************\
|File:  /src/headers/_filemanager.cpp										   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "_filemanager.h"

FileManager FileMan;

FileManager::~FileManager(){
	for(unsigned index=0;index<this->_handles.getSize();index++){
		delete this->_handles[index];
	}
}

NConFile* FileManager::fetchFileHandle(String fileLocation,NConFile::Mode mode,bool createIfNotExists){
	if(fileLocation.equalTo("")){  //Figure out how to overcome the "ambiguous operator" problem to be able to use fileLocation==""
		return NULL;
	}
    
	try{
		this->_handles.addElement(new NConFile(fileLocation,mode,createIfNotExists));
	}catch(FileIOException e){
		if(e.getCode()==FileIOException::FILE_NOT_EXIST){
			return NULL;
		}
        
        throw;
	}

	return this->_handles[this->_handles.getSize()-1];
}