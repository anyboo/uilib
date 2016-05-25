#pragma once


#if !defined(SISEC_DLL_EXPORT)
#define SISEC_EXPORTING  __declspec(dllimport)
#else
#define SISEC_EXPORTING __declspec(dllexport)    
#endif

SISEC_EXPORTING const char* sscobs_getLastError();
SISEC_EXPORTING bool sscobs_init(const char* sConfigDir = 0);
SISEC_EXPORTING bool sscobs_uninit();
SISEC_EXPORTING bool sscobs_startRecording(const char* pathFile, int x, int y, int w, int h);
SISEC_EXPORTING bool sscobs_stopRecording();
SISEC_EXPORTING void sscobs_pauseRecording();
SISEC_EXPORTING void sscobs_resumeRecording();
SISEC_EXPORTING void sscobs_closeVolume();
SISEC_EXPORTING void sscobs_openVolume();
SISEC_EXPORTING void sscobs_closeMic();
SISEC_EXPORTING void sscobs_openMic();