#include "ReciveUINotification.h"


ReciveUINotification::ReciveUINotification(bool bCancel)
{
	m_bCancel = bCancel;
}


ReciveUINotification::~ReciveUINotification()
{
}

bool ReciveUINotification::GetData() const
{
	return m_bCancel;
}
