/******************************************************************************\
|File:  /src/headers/user.cpp   											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "user.h"

#include "kernel.h"
#include "nconfile.h"
#include "thread.h"
#include "system.h"

List<UserRecord> User::user_db;

//User* CurrentUser=NULL;

User::User(){
	this->_user_info.u_id=0;
	this->_user_info.username="";
	this->_user_info.password="";
	this->_user_info.homeDir="/";
	//User should be treated as a guest.
}

User::User(UserRecord record){
	this->_user_info=record;
}

User::User(String username){
}

User::User(unsigned u_id){
	this->_user_info=User::user_db[u_id];
}

void* User::_loadUserDB(void* argument){
	String tmpString="";
	unsigned num_records=0;
	unsigned record_offset=2;
	unsigned username_length=0;

	if(User::user_db.getSize()>0){  //In other words, if the user database has been loaded...
		return (void*)-1;
	}

	NConFile* user_db=System::getSysFile(System::USER_DB);
	
	tmpString=user_db->getString(0,2);
	num_records=(tmpString[0]<<8)+tmpString[1];  //Convert that number to hex and store it as such

	UserRecord record;
	for(unsigned index=0;index<num_records;index++){
		//Get the u_id
		tmpString=user_db->getString(record_offset,2);
		record.u_id=(tmpString[0]<<8)+tmpString[1];
		record_offset+=2;

		//Get the username
		tmpString=user_db->getString(record_offset,1);
		username_length=tmpString[0];
		record_offset+=1;

		tmpString=user_db->getString(record_offset,username_length);
		record.username=tmpString;
		record_offset+=username_length;

		//Get the password
		tmpString=user_db->getString(record_offset,20);
		record.password=tmpString;
		record_offset+=20;

		//Get the home directory
		record.homeDir="/";  //TODO:  do this properly.  :/

		//Add the record
		User::user_db.addElement(record);
	}

	return (void*)0;
}

unsigned User::getUserIndex(const String username){	
	for(unsigned index=0;index<User::user_db.getSize();index++){
		if(User::user_db[index].username.equalTo(username)){
			return index;
		}
	}
	
	return -1;
}

String User::getUsername(){
	return this->_user_info.username;
}

String User::getHomeDirectory(){
	return this->_user_info.homeDir;
}

List<String>* User::getCommandHistory(){
	return &(this->_user_info.command_history);
}

void User::setPassword(String password){
	//TODO:  Herp derp.  What do you think you need to do.
}

void User::setLastLogin(unsigned login_time){
	//TODO:  Herp derp.  What do you think you need to do.
}