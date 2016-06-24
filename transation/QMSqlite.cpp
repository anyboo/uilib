#include "QMSqlite.h"
#include <stdio.h>
#include <iostream>
#include "Poco/Tuple.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SQLite/Connector.h"

QMSqlite::Garbo QMSqlite::garbo;  // 一定要初始化，不然程序结束时不会析构garbo  

const QMSqlite* QMSqlite::m_instance = NULL;

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
}


const QMSqlite *QMSqlite::getInstance()
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
		if (!m_pool->isActive)
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


bool QMSqlite::createSearchTable()
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;

	try
	{		
		sess << "CREATE TABLE Search(name VARCHAR(100), channel INTEGER, starttime DATETIME, stoptime DATETIME, size BIGINT, alias VARCHAR(100))", now;
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


bool QMSqlite::GetSearchData(std::vector<SearchRecord>&Record)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{		
		Statement select(sess);
		select << "SELECT * FROM Search", into(Record), now;
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


bool QMSqlite::writeSearchData(SearchRecord searchrecode)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{		
		sess << "INSERT INTO Search VALUES(:name, :channel, :starttime, :stoptime, :size, :alias)", use(searchrecode), now;
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

bool QMSqlite::writeSearchDataByVector(std::vector<SearchRecord>&Record)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{
		Statement insert(sess);
		insert << "INSERT INTO Search VALUES(:name, :channel, :starttime, :stoptime, :size, :alias)", use(Record), now;
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


bool QMSqlite::cleanSearchData()
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{		
		sess << "delete from Search" << now;
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


bool QMSqlite::dropSearchTable()
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{		
		sess << "DROP TABLE IF EXISTS Search" << now;
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