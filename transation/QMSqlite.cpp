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


bool QMSqlite::createTable(int tag)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;

	try
	{
		switch (tag)
		{
		case SEARCH_VIDEO:
			sess << "CREATE TABLE SearchVideo(name VARCHAR(100), channel INTEGER, starttime DATETIME, stoptime DATETIME, size BIGINT, alias VARCHAR(100))", now;
			break;
		case SEARCH_DEVICE:
			sess << "CREATE TABLE SearchDevice(fcatoryname VARCHAR(20), ip VARCHAR(30), port INTEGER)", now;
			break;
		default:;
		}
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

bool QMSqlite::GetData(std::vector<SearchVideo>& Record)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{		
		Statement select(sess);		
		select << "SELECT * FROM SearchVideo", into(Record), now;
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

bool QMSqlite::GetData(std::vector<SearchDevice>& Record)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{
		Statement select(sess);
		select << "SELECT * FROM SearchDevice", into(Record), now;		
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


bool QMSqlite::writeData(SearchVideo searchrecode)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{
		sess << "INSERT INTO SearchVideo VALUES(:name, :channel, :starttime, :stoptime, :size, :alias)", use(searchrecode), now;
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


bool QMSqlite::writeDataByVector(std::vector<SearchVideo>&Record)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{
		Statement insert(sess);
		insert << "INSERT INTO SearchVideo VALUES(:name, :channel, :starttime, :stoptime, :size, :alias)", use(Record), now;
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


bool QMSqlite::cleanData(int tag)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{		
		switch (tag)
		{
		case SEARCH_DEVICE:
			sess << "delete from SearchDevice" << now;
			break;
		case SEARCH_VIDEO:
			sess << "delete from SearchVideo" << now;
			break;
		default:;
		}
		
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


bool QMSqlite::dropTable(int tag)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{		
		switch (tag)
		{
		case SEARCH_DEVICE:
			sess << "DROP TABLE IF EXISTS SearchDevice" << now;
			break;
		case SEARCH_VIDEO:
			sess << "DROP TABLE IF EXISTS SearchVideo" << now;
			break;
		default:;
		}		
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

