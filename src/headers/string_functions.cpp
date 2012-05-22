/******************************************************************************\
|File:  /src/headers/string_functions.cpp									   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "string_functions.h"

#include <math.h>

#include "exception.h"

#include <stdio.h>

String implode(const List<String> explodedString,char implodeOn){
	String retString="";

	for(unsigned index=0;index<explodedString.getSize();index++){
		if(!explodedString[index].equalTo("")){
			retString+=explodedString[index];
		}

		if(index<explodedString.getSize()-1){
			retString+=implodeOn;
		}
	}

	return retString;
}

String intToString(unsigned numStr){
	String retString="";
	
	bool truncateZeroes=true;
	int modBy=0;
	int result=0;
	
	for(unsigned index=10;index>0;index--){
		/*Do le math*/
		modBy=10;
		for(unsigned counter=0;counter<index-1;counter++){
			modBy*=10;
		}
		result=((numStr%modBy)/(modBy/10));
		
		/*Actual string append*/
		if(truncateZeroes&&result==0){
				continue;
		}
		
		retString+=(char)(((int)'0')+result);
		
		truncateZeroes=false;
	}
	
	return retString;
}

unsigned stringToInt(String str){
	unsigned retVal=0;
	int currentNum=0;
	
	for(unsigned index=0;index<str.getSize();index++){
		currentNum=(((int)str[index])-((int)'0'));
		if(currentNum<0||currentNum>9){
			throw Exception("Non-numeric symbol found when converting string to int.",1);
		}
		
		retVal+=currentNum*pow(10,9-(index));
	}
	
	return retVal;
}