/******************************************************************************\
|File:  /src/headers/string.h   											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef STRING_H
#define STRING_H

#include "list.h"

class String:public List<char>{
	public:
		String();
		String(const String& str);
		String(const char* str);
		String(const char& ch);
		
		List<String> explode(char explodeOn,bool removeEmptyStrings=false) const;
		String trim(char charToTrim=' ') const;
		
		unsigned getSize() const;
		
		bool equalTo(const String& str) const;
		
		char& operator[](unsigned index) const;

		String& operator=(const String& str);
		String& operator=(const char& ch);
		String& operator=(const char* str);
		
		String& operator+=(const String& str);
		String& operator+=(const char& ch);
		String& operator+=(const char* str);
		
		String operator+(const String& str);
		String operator+(const char& ch);
		String operator+(const char* str);
		
		bool operator==(String& str);
		
		bool operator!=(String& str);
		
		operator char*() const;
};

#endif