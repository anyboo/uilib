#pragma once

class CSetting
{
public:
	CSetting();
	CSetting(const STDSTRING& filename);
	
	~CSetting();

	static CSetting& Inst();
	void load();
	void save();
private:
	void preload();
	STDSTRING location;
	STDSTRING encode;
	STDSTRING configuration;
};

