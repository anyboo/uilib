
#include "ipset.h"
#include <iostream>
#include <memory>
#include <string>
#include "windowutils.h"
#include "PING.h"

using namespace std;

void IPSet::deepConfig()
{
	std::shared_ptr<bool> bResult = std::make_shared<bool>(false);
	std::shared_ptr<string> pIP = std::make_shared<string>();
	std::shared_ptr<string> pNetGate = std::make_shared<string>();
	std::shared_ptr<bool> bpCancel = std::make_shared<bool>(false);
	
	for (int i = 0; i < 255; i++)
	{
		string sNetGate("192.168.");
		sNetGate.append(std::to_string(i));
		sNetGate.append(".1");
		string sIP("192.168.");
		sIP.append(std::to_string(i));
		sIP.append(".44");
		*pNetGate = sNetGate;
		*pIP = sIP;

		if (!WindowUtils::setNetConfig(WindowUtils::getLocalUuid(), *pIP, "255.255.255.0", *pNetGate, true))
		{
			continue;
		}


		if (CPing::instance().instance().ScanOneIP(*pNetGate, *pIP, false))
		{
			*bResult = true;
			break;
		}
	}

	if (*bResult)
	{
		cout << "current ip:" << *pIP << endl;
		cout << "255.255.255.0" << endl;
		cout << "dest ip: " << *pNetGate << endl;
	}
	else{
		cout << "ip setting failed." << endl;
	}
}

void IPSet::ipsetting()
{
	//�ж�����״̬
	if (!WindowUtils::isOnLine())
	{		
		return ;
	}

	std::shared_ptr<string> pIP = std::make_shared<string>();
	std::shared_ptr<string> pNetGate = std::make_shared<string>();
	std::shared_ptr<string> pMask = std::make_shared<string>("255.255.255.0");
	std::shared_ptr<bool> bResult = std::make_shared<bool>(false);
	std::shared_ptr<bool> bpCancel = std::make_shared<bool>(false);
	
	//ͨ��DHCP��ʽ����IP��ַ
	*bResult = WindowUtils::setIPByDHCP(*pIP, *pMask, *pNetGate);

	if (!*bResult)
	{
		//ֱ����ʽ��ȡip������
		*bResult = WindowUtils::getDirectDevice(*pIP, *pNetGate);
		if (*bResult)
		{
			//ͨ����̬��ʽ����IP��ַ
			if (!WindowUtils::setNetConfig(WindowUtils::getLocalUuid(), *pIP, "255.255.255.0", *pNetGate, true))
			{
				*bResult = false;
			}
		}
	}


	if (*bResult)
	{
		cout << " current ip:" << *pIP << endl;
		cout << " other ip:" << *pNetGate << endl;
	}
	else{		
		//���ƥ�䷽ʽ����ip
		deepConfig();
	}
}

void IPSet::run()
{
	time_t t_start, t_end;
	t_start = time(NULL);
	ipsetting();
	t_end = time(NULL);
	printf("0 time: %.0f s\n", difftime(t_end, t_start));
	
}