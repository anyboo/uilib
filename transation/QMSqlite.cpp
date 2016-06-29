#include "QMSqlite.h"
#include <stdio.h>
#include <iostream>
#include "Poco/Tuple.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SQLite/Connector.h"

QMSqlite::Garbo QMSqlite::garbo;  // 一定要初始化，不然程序结束时不会析构garbo  

QMSqlite* QMSqlite::m_instance = NULL;

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
		m_pool = new SessionPool(SQLite::Connector::KEY, ":memory:", 1, 100, 10);
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
	/*Session sess = connectDb();
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

	return true;*/
	return execSql(sql);
}

bool QMSqlite::GetData(string sql, std::vector<readSearchVideo>& Record)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{		
		Statement select(sess);		
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

bool QMSqlite::GetData(string sql, std::vector<SearchDevice>& Record)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{
		Statement select(sess);
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


bool QMSqlite::writeData(writeSearchVideo searchrecode)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{
		sess << "INSERT INTO SearchVideo VALUES(:name, :channel, :starttime, :stoptime, :size, :id)", use(searchrecode), now;
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

bool QMSqlite::writeData(SearchDevice searchrecode)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{
		sess << "INSERT INTO SearchDevice VALUES(:fcatoryname, :ip, :port)", use(searchrecode), now;		
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


bool QMSqlite::writeDataByVector(std::vector<writeSearchVideo>&Record)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{
		Statement insert(sess);
		insert << "INSERT INTO SearchVideo VALUES(:name, :channel, :starttime, :stoptime, :size, :id)", use(Record), now;
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

bool QMSqlite::writeDataByVector(std::vector<SearchDevice>& Record)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{
		Statement insert(sess);
		insert << "INSERT INTO SearchDevice VALUES(:fcatoryname, :ip, :port)", use(Record), now;
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
		sess << sql , now;
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