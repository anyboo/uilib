#include "stdafx.h"
#include "RecordHandler.h"
#include <exception>
#include <cassert>
#include <Poco/Path.h>
#include <Poco/SingletonHolder.h>
#include <Poco/Exception.h>
#include "SisecObsApi"
#include "Setting.h"

CRecordHandler::CRecordHandler()
:MuteVolume(false),
MuteMicroPhone(false),
recording(false),
count(0)
{
	init();
}


CRecordHandler::~CRecordHandler()
{
	uninit();
}

CRecordHandler& CRecordHandler::Inst()
{
	static Poco::SingletonHolder<CRecordHandler> sh;
	return *sh.get();
}

void CRecordHandler::init()
{
	CSetting& s = CSetting::Inst();
	std::string config = Poco::Path::home();

	if (!sscobs_init(config.c_str()))
	{
		throw std::exception("sscobs_init failed!");
	}
}

void CRecordHandler::uninit()
{
	sscobs_uninit();
}

#include <sstream>
#include <iomanip>
#include <ctime>

static std::string currentTime()
{
	time_t current;
	time(&current);
	auto tm = *std::localtime(&current);

	std::stringstream str;
	str << std::put_time(&tm, "%Y%m%d_%H%M%S");
	return str.str();
}

std::string CRecordHandler::GenerateFileName()
{
	CSetting& s = CSetting::Inst();

	std::stringstream ss;
	ss << s.GetLocation();
	if (ss.str().back() != '\\')
		ss << '\\';
	ss << currentTime() << s.GetEncode() << std::ends;

	return ss.str();
}

void CRecordHandler::start()
{
	if (recording) return;
	
	std::string filename = GenerateFileName();

	sscobs_startRecording(filename.c_str(), _p.x, _p.y, _s.cx, _s.cy);

	if (MuteVolume)
	{
		sscobs_closeVolume();
	}
		
	if (MuteMicroPhone)
	{
		sscobs_closeMic();
	}

	recording = true;
}
void CRecordHandler::stop()
{
	if (recording)
	{
		sscobs_stopRecording();
		recording = false;
	}
}
void CRecordHandler::pause()
{
	//Not Implement
	assert(0);
	throw std::exception("pause is not implement.");
}
void CRecordHandler::resume()
{
	//Not Implement
	assert(0);
	throw std::exception("resume is not implement.");
}
void CRecordHandler::SetVolume(bool mute)
{
	MuteVolume = mute;
}
void CRecordHandler::SetMicro(bool Mute)
{
	MuteMicroPhone = Mute;
}

void CRecordHandler::SetArea(const POINT& p, const SIZE& s)
{
	_p = p;
	_s = s;
}