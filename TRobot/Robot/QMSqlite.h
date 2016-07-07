#ifndef QM_SQLITE_H
#define QM_SQLITE_H


#include "Poco/Tuple.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>
#include <vector>

using namespace Poco::Data::Keywords;
using namespace Poco::Data;
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




class QMSqlite
{
public:

	static QMSqlite* getInstance();
private:
	QMSqlite();
	~QMSqlite();
	static QMSqlite* m_instance;

	class Garbo

	{

	public:

		~Garbo()

		{

			if (QMSqlite::m_instance)

			{

				//cout << "Garbo dtor" << endl;

				delete QMSqlite::m_instance;

			}

		}

	};

	static Garbo garbo;
public:
	template<typename T>
	bool GetData(std::string sql, std::vector<T>& Record)
	{
		Poco::Data::Session sess = connectDb();
		if (!checkConnect(sess))
			return false;
		try
		{
			Poco::Data::Statement select(sess);
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
	template<typename T>
	bool writeData(std::string sql, T searchrecode)
	{
		Session sess = connectDb();
		if (!checkConnect(sess))
			return false;
		try
		{
			sess << sql, use(searchrecode), now;
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

	template<typename T>
	bool writeDataByVector(std::string sql, std::vector<T>& Record)
	{
		Session sess = connectDb();
		if (!checkConnect(sess))
			return false;
		try
		{
			Statement insert(sess);
			insert << sql, use(Record), now;
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
	bool cleanData(std::string sql);
	bool dropTable(std::string sql);
	bool createTable(std::string sql);
	bool searchFactoryName(std::string sx, std::vector<std::string>& Record);
private:
	bool Initialize();
	bool creatSessionPool();
	void closeConnect(Poco::Data::Session sess);
	bool checkConnect(Poco::Data::Session sess);
	bool unInitialize();
	void closeSessionPool();
	bool execSql(std::string sql);
	Poco::Data::Session connectDb();
	Poco::Data::SessionPool *m_pool;

};


#endif 