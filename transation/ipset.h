#ifndef IP_SET_H
#define IP_SET_H

#include "Poco/Runnable.h"

class IPSet : public Poco::Runnable
{
	void deepConfig();
	void ipsetting();
	void run();
};


#endif