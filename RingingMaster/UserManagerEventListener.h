#pragma once

class UserManagerEventListener
{
public:
	UserManagerEventListener(void) {};
	~UserManagerEventListener(void) {};

	virtual void userManager_notifyUserListChange() = 0;
};

typedef CArray<UserManagerEventListener*, UserManagerEventListener*> UserManagerEventListeners;
