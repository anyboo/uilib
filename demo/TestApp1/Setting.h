#pragma once
#include <string>

class CSetting
{
public:
	CSetting();
	CSetting(const std::string& filename);

	~CSetting();

	void load();
	void save();
private:
	void preload();
};

