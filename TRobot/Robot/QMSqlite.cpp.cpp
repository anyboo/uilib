#include "QMSqlite.h"
#include <stdio.h>
#include <iostream>


QMSqlite::Garbo QMSqlite::garbo;  // 一定要初始化，不然程序结束时不会析构garbo  

QMSqlite* QMSqlite::m_instance = NULL;

using namespace std;

QMSqlite::QMSqlite() :m_pool(NULL)
{
	Initialize();
	creatSessionPool();
}


QMSqlite::~QMSqlite()
{
	closeSessionPool();
	unInitialize();
}


QMSqlite* QMSqlite::getInstance()
{
	if (NULL == m_instance)
		m_instance = new QMSqlite;
	return m_instance;
}



bool QMSqlite::Initialize()
{
	//setting session type
	try
	{
		SQLite::Connector::registerConnector();
	}
	catch (Poco::Exception &ex)
	{
		throw(ex.displayText());
		return false;
	}


	return true;
}


bool QMSqlite::creatSessionPool()
{
	try
	{
		//create pool
#ifdef _DEBUG
		m_pool = new SessionPool(SQLite::Connector::KEY, "memory.db", 1, 100, 10);
#else
		m_pool = new SessionPool(SQLite::Connector::KEY, ":memory:", 1, 100, 10);
#endif
		if (!m_pool->isActive())
			throw "new session fail";
	}
	catch (Poco::Exception &ex)
	{
		throw(ex.displayText());
		return false;
	}

	return true;
}


Session QMSqlite::connectDb()
{
	//get session
	Session sess(m_pool->get());
	if (!sess.isConnected())
		throw "session get error";

	return sess;
}


bool QMSqlite::createTable(string sql)
{
	return execSql(sql);
}


bool QMSqlite::cleanData(string sql)
{
	return execSql(sql);
}


bool QMSqlite::dropTable(string sql)
{
	return execSql(sql);
}

void QMSqlite::closeConnect(Session sess)
{
	if (sess.isConnected())
		sess.close();
}

bool QMSqlite::checkConnect(Session sess)
{
	if (!sess.isConnected())
		return false;

	return true;
}

bool QMSqlite::unInitialize()
{
	try
	{
		SQLite::Connector::unregisterConnector();
	}
	catch (Poco::Exception &ex)
	{
		throw(ex.displayText());
		return false;
	}

	return true;
}

void QMSqlite::closeSessionPool()
{
	delete m_pool;
}

bool QMSqlite::execSql(string sql)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{
		sess << sql, now;
		closeConnect(sess);
	}
	catch (Poco::Exception &ex)
	{
		throw(ex.displayText());
		closeConnect(sess);
		return false;
	}
	return true;
}

bool QMSqlite::searchFactoryName(string sx, std::vector<string>& Record)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{
		Statement select(sess);
		string sql = "SELECT name FROM SearchFactory where sx like \"" + sx;
		sql.append("%\"");
		select << sql, into(Record), now;
		closeConnect(sess);
	}
	catch (Poco::Exception &ex)
	{
		throw(ex.displayText());
		closeConnect(sess);
		return false;
	}

	return true;
}