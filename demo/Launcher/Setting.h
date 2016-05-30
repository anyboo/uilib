#pragma once
#include "NewUICtrol.h"
#include <vector>
#include "MyHandle.h"


class Config
{
public:
	Config();
	~Config();

	void		GetConfigPath();
	void		LoadConfig(vector<LayOut_Info>& AllLyt, CPaintManagerUI& m_pm);
	void		SaveToConfig(vector<LayOut_Info>& AllLyt);
private:
	STDSTRING WriteablePath;
	STDSTRING configfile;
	STDSTRING AppLetter;
	STDSTRING tmp;
public:
	CMyHandle			m_MyHandle;
};

