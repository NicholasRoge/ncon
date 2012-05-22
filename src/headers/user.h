/******************************************************************************\
|File:  /src/headers/user.h     											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef USER_H
#define USER_H

#include <iostream>
#include <ctime>
#include "string.h"
#include "list.h"

class NConFile;

struct UserRecord{
	unsigned u_id;
	String username;
	String password;  //this should NEVER be plaintext.  TODO:  This should NOT be plaintext

	String homeDir;
	
	List<String> command_history;
};

class User{
	private:
		UserRecord _user_info;

		static List<UserRecord> user_db;

		int _createNewUser(String username);
		static void* _loadUserDB(void* argument);  //Only to be used by System

	public:
		User();
		User(UserRecord record);
		User(String username);
		User(unsigned u_id);

		static unsigned getUserIndex(const String username);
		
		String getUsername();
		String getHomeDirectory();  //Returns home directory relative to NCon
		List<String>* getCommandHistory();
		void setPassword(String password);
		void setLastLogin(unsigned login_time=time(NULL));

	friend class System;
};
#endif