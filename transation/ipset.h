#ifndef IP_SET_H
#define IP_SET_H

#include "Poco/Runnable.h"
#include <iostream>

class IPSet : public Poco::Runnable
{
public:
	void run();
private:
	static bool sortByIp(std::string srFirst, std::string srSecond);	
	void ipsetting();
};


#endif