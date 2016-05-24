#include "stdafx.h"
#include "RecordHandler.h"
#include <exception>
#include <cassert>

CRecordHandler::CRecordHandler()
{
	init();
}


CRecordHandler::~CRecordHandler()
{
	uninit();
}

void CRecordHandler::init()
{
	///sscobs_init();
}

void CRecordHandler::uninit()
{
	///sscobs_uninit();
}

void CRecordHandler::start()
{
	if (Recording) return;
	///sscobs_startRecording(arg);
	if (MuteVolume)
	{
		///sscobs_closeVolume();
	}
		
	if (MuteMicroPhone)
	{
		///sscobs_closeMic();
	}
		
}
void CRecordHandler::stop()
{
	if (Recording)
	{
		///sscobs_stopRecording();
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