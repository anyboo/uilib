#pragma once

#include <string>
#include <ctime>

struct _video_info
{
	std::string name;
	size_t channel;
	std::time_t start;
	std::time_t end;
	size_t size;
}video_info, *pvideo_info;

class SearchNotify
{
public:
	virtual void OnReported(const _video_info& info) = 0;
	virtual void OnFinished() = 0;
	virtual void OnProcessStatus(const size_t percent) = 0;
};
