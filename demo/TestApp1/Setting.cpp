#include "stdafx.h"
#include "Setting.h"

#include <Poco/SingletonHolder.h>
#include <Poco/Path.h>
#include <Poco/UnicodeConverter.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <fstream>

#include <locale>
#include <codecvt>

using namespace Poco;
using namespace rapidjson;

CSetting::CSetting()
{
	initialize();
	load();
}

CSetting::~CSetting()
{
	save();
}

CSetting& CSetting::Inst()
{
	static SingletonHolder<CSetting> sh;
	return *sh.get();
}

static std::wstring convert(std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(str);
}

static std::string convert(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(wstr);
}

void CSetting::initialize()
{
	location = Path::tempHome();
	container = ".mp4";
	configuration = Path::home() + "config.json";
}
	
#include <rapidjson/document.h>

void CSetting::load()
{
	TRACE
	preload();
	std::ifstream ifs(configuration);
	IStreamWrapper osw(ifs);
	
	Document d;
	d.ParseStream(osw);

	if (d.MemberCount() <= 0) return;

	if (d.HasMember("location")) 
		location = d["location"].GetString();

	if (d.HasMember("container"))
		container = d["container"].GetString();

	if (d.HasMember("configuration"))
		configuration = d["configuration"].GetString();
}

void CSetting::save()
{
	std::ofstream ofs(configuration);
	OStreamWrapper osw(ofs);
	PrettyWriter<OStreamWrapper> writer(osw);

	Serialize(writer);
	ofs.close();
}

void CSetting::preload()
{
	TRACE
}

std::string CSetting::GetLocation()const
{
	return location;
}

std::string CSetting::GetEncode()const
{
	return container;
}

std::string CSetting::GetConfiguration() const
{
	return configuration;
}

void CSetting::SetLocation(const std::string& path)
{
	location = path;
	save();
}

#include <algorithm>
void CSetting::SetEncode(const std::string& fileExt)
{
	//eg. format to .mp4 and lowercase it.
	container.clear();
	container += ".";
	container += fileExt;
	std::transform(container.begin(), container.end(), container.begin(), ::tolower);
	save();
}
void CSetting::SetConfiguration(const std::string& path)
{
	configuration = path;
	save();
}

void CSetting::GetLocation(std::wstring& wstr)
{
	wstr = convert(location);
}

void CSetting::GetEncode(std::wstring& wstr)
{
	wstr = convert(container);
}

void CSetting::GetConfiguration(std::wstring& wstr)
{
	wstr = convert(configuration);
}

void CSetting::SetLocation(const std::wstring& location)
{
	std::string str = convert(location);
	SetLocation(str);
}

void CSetting::SetEncode(const std::wstring& container)
{
	std::string str = convert(container);
	SetEncode(str);
}

void CSetting::SetConfiguration(const std::wstring& configuration)
{
	std::string str = convert(configuration);
	SetConfiguration(str);
}