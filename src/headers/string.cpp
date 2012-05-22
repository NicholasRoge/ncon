/******************************************************************************\
|File:  /src/headers/string.cpp  											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "string.h"

#include <cstring>
#include "exception.h"

String::String(){
	this->addElement('\0');
}
String::String(const String& str){
	for(unsigned index=0;index<str.getSize();index++){
		this->addElement(str[index]);
	}
	this->addElement('\0');
}
String::String(const char* str){
	if(str[0]!='\0'){  //If the first character is NULL, str is empty.
		for(unsigned index=0;index<strlen(str);index++){
			this->addElement(str[index]);
		}	
	}
	
	this->addElement('\0');
}
String::String(const char& ch){
	this->addElement(ch);
	this->addElement('\0');
}

List<String> String::explode(char explodeOn,bool removeEmptyStrings) const{
	List<String> explodedString;

	if(this->getSize()==0||(this->getSize()==1&&this->_list[0]==explodeOn)){
		return explodedString;
	}

	explodedString.addElement("");
	for(unsigned index=0;index<this->getSize();index++){
		if(this->_list[index]==explodeOn){
			explodedString.addElement("");
		}else{
			explodedString[explodedString.getSize()-1]+=this->_list[index];
		}
	}
	
	if(removeEmptyStrings){
		for(unsigned index=0;index<explodedString.getSize();index++){
			if(explodedString[index].getSize()==0){
				explodedString.removeElementAt(index);
				index--;
			}
		}
	}

	return explodedString;
}

String String::trim(char charToTrim) const{
	String copy=*this;
	
	while(copy.getSize()>0){
		if(copy[0]==charToTrim){
			copy.removeElementAt(0);
		}else{
			break;
		}
	}
	
	for(unsigned index=copy.getSize()-1;index>=0;index++){
		if(copy[index]==charToTrim){
			copy.removeElementAt(index);
		}else{
			break;
		}
	}
	
	return copy;
}

unsigned String::getSize() const{
	return List<char>::getSize()-1;  //We don't want to count the NULL terminator at the end towards the length.
}

bool String::equalTo(const String& str) const{
	bool nullComparison1;
	bool nullComparison2;
	
	for(unsigned index=0;;index++){
		nullComparison1=this->_list[index]=='\0';
		nullComparison2=str[index]=='\0';
		
		if(nullComparison1||nullComparison2){
			if(nullComparison1){
				if(nullComparison2){
					return true;
				}else{
					return false;
				}
			}else{  //If one is false while the other is true, it's guarenteed to be not equal.  I think.
				return false;
			}
		}
		
		if(this->_list[index]!=str[index]){
			return false;
		}
	}
	
	return true;
}

char& String::operator[](unsigned index) const{
	if(index<0&&index>=this->getSize()){
		throw Exception("Attempt to access index location outside bounds.",1);
	}
	
	return this->_list[index];
}

String& String::operator=(const String& str){
	this->clear();
	
	for(unsigned index=0;index<str.getSize();index++){
		this->addElement(str[index]);
	}
	this->addElement('\0');
	
	return *this;
}
String& String::operator=(const char& ch){
	this->clear();
	
	this->addElement(ch);
	this->addElement('\0');
	
	return *this;
}
String& String::operator=(const char* str){
	this->clear();
	
	for(unsigned index=0;index<strlen(str);index++){
		this->addElement(str[index]);
	}
	this->addElement('\0');
	
	return *this;
}

String& String::operator+=(const String& str){
	for(unsigned index=0;index<str.getSize();index++){
		this->addElement(str[index],this->getSize());
	}
	
	return *this;
}
String& String::operator+=(const char& ch){
	this->addElement(ch,this->getSize());  //Remember that this->getSize() is not List<char>::getSize()
	
	return *this;
}
String& String::operator+=(const char* str){
	for(unsigned index=0;index<strlen(str);index++){
		this->addElement(str[index],this->getSize());
	}
	
	return *this;
}

String String::operator+(const String& str){
	String copy=*this;
	copy+=str;
	
	return copy;
}
String String::operator+(const char& ch){
	String copy=*this;
	copy+=ch;
	
	return copy;
}
String String::operator+(const char* str){
	String copy=*this;
	copy+=str;
	
	return copy;
}

bool String::operator==(String& str){
	return this->equalTo(str);
}

bool String::operator!=(String& str){
	return !(this->equalTo(str));
}

String::operator char*() const{
	return this->_list;
}