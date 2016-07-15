#ifndef QM_SQLITE_H
#define QM_SQLITE_H


#include "Poco/Tuple.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>
#include <vector>
#include "DatabaseException.h"

using namespace std;
//using namespace Poco::Data::Keywords;
//using namespace Poco::Data;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::Statement;


//search video result table
#define CREATE_SEARCH_VIDEO_TABLE   "CREATE TABLE SearchVideo(name VARCHAR(100), channel INTEGER, starttime DATETIME, stoptime DATETIME, size BIGINT, id INTEGER PRIMARY KEY)"
#define DELETE_ALL_SEARCH_VIDEO     "DELETE from SearchVideo"
#define DROP_SEARCH_VIDEO_TABLE		"DROP TABLE IF EXISTS SearchVideo"
#define SELECT_ALL_SEARCH_VIDEO		"SELECT * FROM SearchVideo"
#define SELECT_ID_SEARCH_VIDEO		"SELECT * FROM SearchVideo where id="
#define INSERT_SEARCH_VIDEO			"INSERT INTO SearchVideo VALUES(:name, :channel, :starttime, :stoptime, :size, :id)"									 
//struct search video
typedef Poco::Tuple<std::string, int, __time64_t, __time64_t, __int64, int> readSearchVideo;
typedef Poco::Tuple<std::string, int, __time64_t, __time64_t, __int64> writeSearchVideo;

//search device result table
#define CREATE_SEARCH_DEVICE_TABLE  "CREATE TABLE SearchDevice(fcatoryname VARCHAR(20), ip VARCHAR(30), port INTEGER)"
#define DELETE_ALL_SEARCH_DEVICE    "DELETE from SearchDevice"
#define DROP_SEARCH_DEVICE_TABLE	"DROP TABLE IF EXISTS SearchDevice"
#define SELECT_ALL_SEARCH_DEVICE	"SELECT * FROM SearchDevice"
#define INSERT_SEARCH_DEVICE		"INSERT INTO SearchDevice VALUES(:fcatoryname, :ip, :port)"
//struct search device
typedef Poco::Tuple<std::string, std::string, int> SearchDevice;

//search factory table
#define CREATE_SEARCH_FACTORY_TABLE	 "CREATE TABLE SearchFactory(sx VARCHAR(10), name VARCHAR(20))"
#define DELETE_ALL_SEARCH_FACTORY	 "DELETE from SearchFactory"
#define DROP_SEARCH_FACTORY_TABLE	 "DROP TABLE IF EXISTS SearchFactory"
#define INSERT_SEARCH_FACTORY		"INSERT INTO SearchFactory VALUES(:sx, :name)"
//#define SELECT_ALL_SEARCH_FACTORY	 "SELECT name FROM SearchFactory where sx like \""
//struct search factory
typedef Poco::Tuple<std::string, std::string> SearchFactory;

//scan port result table
#define CREATE_SCAN_PORT_TABLE		"CREATE TABLE ScanPort(ip VARCHAR(20), port INTEGER)"
#define DELETE_ALL_SCAN_PORT		"DELETE from ScanPort"
#define DROP_SCAN_PORT_TABLE		"DROP TABLE IF EXISTS ScanPort"
#define SELECT_ALL_SCAN_PORT		"SELECT * FROM ScanPort"
#define INSERT_SCAN_PORTE			"INSERT INTO ScanPort VALUES(:ip, :port)"
//struct search device
typedef Poco::Tuple<std::string, int> ScanPortRecord;




class QMSqlite
{
public:
	
	static QMSqlite* getInstance();
private:
	QMSqlite();
	~QMSqlite();
	QMSqlite(QMSqlite const& other);
	QMSqlite& operator=(QMSqlite const& other);
public:
	template<typename T> 
	bool GetData(string sql, std::vector<T>& Record)
	{
		if (sql.empty())
			return false;
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
			throw DatabaseException(ex.displayText());
			closeConnect(sess);
			return false;
		}

		return true;
	}
	template<typename T>
	bool writeData(string sql, T searchrecode)
	{
		if (sql.empty())
			return false;

		Session sess = connectDb();
		if (!checkConnect(sess))
			return false;
		try
		{
			sess << sql, Poco::Data::Keywords::use(searchrecode), Poco::Data::Keywords::now;
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
	
	template<typename T>
	bool writeDataByVector(string sql, std::vector<T>& Record)
	{
		if (sql.empty() || Record.empty())
			return false;
		Session sess = connectDb();
		if (!checkConnect(sess))
			return false;
		try
		{
			Statement insert(sess);
			insert << sql, Poco::Data::Keywords::use(Record), Poco::Data::Keywords::now;
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
	bool cleanData(string sql);
	bool dropTable(string sql);
	bool createTable(string sql);
	bool searchFactoryName(string sx, std::vector<string>& Record);
private:
	bool Initialize();
	bool creatSessionPool();
	void closeConnect(Session sess);
	bool checkConnect(Session sess);
	bool unInitialize();
	void closeSessionPool();
	bool execSql(string sql);
	Session connectDb();
	Poco::Data::SessionPool *m_pool;
	
};


#endif 
