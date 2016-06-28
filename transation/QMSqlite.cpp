#include "QMSqlite.h"

const QMSqlite* QMSqlite::m_instance = new QMSqlite;

QMSqlite::QMSqlite()
{
}


QMSqlite::~QMSqlite()
{
}


const QMSqlite *QMSqlite::getInstance()
{
	return m_instance;
}



int QMSqlite::Initialize()
{
	return 0;
}


int QMSqlite::creatDatabase()
{
	return 0;
}


int QMSqlite::connectDb()
{
	return 0;
}


int QMSqlite::createSearchTable()
{
	return 0;
}


int QMSqlite::GetSearchData()
{
	return 0;
}


int QMSqlite::writeSearchData()
{
	return 0;
}


int QMSqlite::cleanSearchData()
{
	return 0;
}
