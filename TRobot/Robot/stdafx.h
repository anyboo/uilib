// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
// System
#include "windows.h"
#include <iostream>

// Json 
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "ostreamwrapper.h"
#include "istreamwrapper.h"

// Jxj SDK
#include "inc\mb_api.h"
#include "inc\JNetSDK.h"
#include "inc\stdint.h"
#include "inc\Jtype.h"
#include "inc\AVPlayer.h"

#pragma comment(lib, "lib\\JNetSDK")
#pragma comment(lib, "lib\\AVPlayer")

using namespace std;
using namespace rapidjson;

#define DRAW_MAX_COUNT		2048*10
#define DRAW_MAX_TROUGH_COUNT		16

#define GET_MAX_DEV_NUM	    81   //最大的获取设备数量
#define OUTPUT_INTERVAL     3  
#define GET_MAX_IP      50

//查询设备
typedef struct queryDevConfig
{
	char				dst_id[MB_SERIAL_LEN];
	j_Device_T			stDev;
}DevConfig;

typedef enum
{
	Encoder_Unknow = -1,
	Encoder_DM365 = 0,
	Encoder_DM368 = 1,
	Encoder_DM8127 = 2,
	Encoder_DM8168 = 3,
	Encoder_HI = 0x100 - 1,
	Encoder_3507 = 0x100,
	Encoder_3510,
	Encoder_3511,
	Encoder_3512,
	Encoder_3515,
	Encoder_3516,
	Encoder_3517,
	Encoder_3518,
	Encoder_3520,
	Encoder_3521,
	Encoder_3531,
	Encoder_3532,
	Encoder_3531_3532,
}eEncoderType;
