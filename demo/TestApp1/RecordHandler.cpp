#include "stdafx.h"
#include "RecordHandler.h"
#include <exception>
#include <cassert>
#include <Poco/SingletonHolder.h>
#include <Poco/Exception.h>
#include "SisecObsApi.h"

CRecordHandler::CRecordHandler()
:format("mp4"),
MuteVolume(false),
MuteMicroPhone(false),
Recording(false)
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
	std::string config("");
	if (!sscobs_init(config.c_str()))
	{
		throw std::exception("sscobs_init failed!");
	}
}

void CRecordHandler::uninit()
{
	sscobs_uninit();
}

void CRecordHandler::start()
{
	count++;

	if (!Recording) return;
	std::string file("d:\\test\\record");
	file += count;
	file += ".mp4";
	sscobs_startRecording(file.c_str(), _p.x, _p.y, _s.cx, _s.cy);

	if (MuteVolume)
	{
		sscobs_closeVolume();
	}
		
	if (MuteMicroPhone)
	{
		sscobs_closeMic();
	}

	Recording = true;
}
void CRecordHandler::stop()
{
	if (Recording)
	{
		sscobs_stopRecording();
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

void CRecordHandler::SetEncode(const ENCODE& type)
{
	assert(type < FLV || type > MP4);

	if (MP4 == type)
		format = "mp4";
	else if (FLV == type)
		format = "flv";
}

void CRecordHandler::SetArea(const POINT& p, const SIZE& s)
{
	_p = p;
	_s = s;
}