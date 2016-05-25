#include "stdafx.h"
#include "Setting.h"

#include <Poco/SingletonHolder.h>
#include <Poco/Path.h>

using namespace Poco;
using namespace rapidjson;

CSetting::CSetting()
{
	TRACE
}

CSetting::CSetting(const STDSTRING& filename)
:configuration(filename), location(""), encode("")
{
	TRACE 
}

CSetting::~CSetting()
{
	TRACE
}

CSetting& CSetting::Inst()
{
	TRACE
	static SingletonHolder<CSetting> sh;
	return *sh.get();
}

void CSetting::load()
{
	TRACE
	preload();
	std::string home = Path::home();
	//hom() + setting.json
}

void CSetting::save()
{
	TRACE
}

void CSetting::preload()
{
	TRACE
}