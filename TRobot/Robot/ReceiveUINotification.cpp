#include "ReceiveUINotification.h"


ReceiveUINotification::ReceiveUINotification(bool bCancel)
{
	m_bCancel = bCancel;
}


ReceiveUINotification::~ReceiveUINotification()
{
}

bool ReceiveUINotification::GetData() const
{
	return m_bCancel;
}
