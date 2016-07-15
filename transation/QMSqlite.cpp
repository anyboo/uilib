#include "QMSqlite.h"
#include <stdio.h>
#include <iostream>


using namespace Poco::Data::Keywords;
using namespace Poco::Data;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::Statement;


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
	static QMSqlite memorySqlite;
	return &memorySqlite;
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
		throw DatabaseException(ex.displayText());
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
		Session sess = connectDb();
		sess << "PRAGMA synchronous = OFF; ", now;
#else
		m_pool = new SessionPool(SQLite::Connector::KEY, ":memory:", 1, 100, 10);
#endif
		if (!m_pool->isActive())
			throw DatabaseException("new session fail");
	}
	catch (Poco::Exception &ex)
	{
		throw DatabaseException(ex.displayText());
		return false;
	}
	
	return true;
}


Session QMSqlite::connectDb()
{
	//get session
	Session sess(m_pool->get());
	if (!sess.isConnected())	
		throw DatabaseException("session get error");

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
		throw DatabaseException(ex.displayText());
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
		if (sql.empty())
			return false;
		sess << sql , now;
		closeConnect(sess);
	}
	catch (Poco::Exception &ex)
	{
		throw DatabaseException(ex.displayText());
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
		if (sx.empty())
			return false;
		Statement select(sess);
		string sql = "SELECT name FROM SearchFactory where sx like \"" + sx;
		sql.append("%\"");		
		select << sql, into(Record), now;
		closeConnect(sess);
	}
	catch (Poco::Exception &ex)
	{
		throw DatabaseException(ex.displayText());
		closeConnect(sess);
		return false;
	}

	return true;
}

