
#include "PlayVideoWorker.h"

CPlayVideoWorker::CPlayVideoWorker(Device* pDev, const RecordFile& file, const HWND hWnd, NotificationQueue& queue) :
m_queue(queue)
{
	m_pDev = pDev;
	m_file = file;
	m_hWnd = hWnd;
}

CPlayVideoWorker::~CPlayVideoWorker()
{
}

void CPlayVideoWorker::run()
{

}
