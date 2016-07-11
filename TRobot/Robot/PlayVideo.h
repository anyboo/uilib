#pragma once

#include "Poco/Runnable.h"

using Poco::Runnable;

class CPlayVideo :
	public Runnable
{
public:
	CPlayVideo();
	~CPlayVideo();
};

