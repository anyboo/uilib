#ifndef QM_SQLITE_H
#define QM_SQLITE_H

#include "Poco/Data/Session.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>
#include <vector>


typedef Poco::Tuple<std::string, int, __time64_t, __time64_t, __int64, int> readSearchVideo;
typedef Poco::Tuple<std::string, int, __time64_t, __time64_t, __int64> writeSearchVideo;
typedef Poco::Tuple<std::string, std::string, int> SearchDevice;

//search video result table
#define CREATE_SEARCH_VIDEO_TABLE   "CREATE TABLE SearchVideo(name VARCHAR(100), channel INTEGER, starttime DATETIME, stoptime DATETIME, size BIGINT, id INTEGER PRIMARY KEY)"
#define DELETE_ALL_SEARCH_VIDEO     "delete from SearchVideo"
#define DROP_SEARCH_VIDEO_TABLE		"DROP TABLE IF EXISTS SearchVideo"
#define SELECT_ALL_SEARCH_VIDEO		"SELECT * FROM SearchVideo"
#define SELECT_ID_SEARCH_VIDEO		"SELECT * FROM SearchVideo where id="

//search device result table
#define CREATE_SEARCH_DEVICE_TABLE  "CREATE TABLE SearchDevice(fcatoryname VARCHAR(20), ip VARCHAR(30), port INTEGER)"
#define DELETE_ALL_SEARCH_DEVICE    "delete from SearchDevice"
#define DROP_SEARCH_DEVICE_TABLE	"DROP TABLE IF EXISTS SearchDevice"
#define SELECT_ALL_SEARCH_DEVICE	"SELECT * FROM SearchDevice"



using namespace Poco::Data;
using namespace std;

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
	bool GetData(string sql, vector<readSearchVideo>& Record);
	bool GetData(string sql, vector<SearchDevice>& Record);
	bool writeData(writeSearchVideo searchrecode);
	bool writeData(SearchDevice searchrecode);
	bool writeDataByVector(vector<writeSearchVideo>& Record);
	bool writeDataByVector(vector<SearchDevice>& Record);
	bool cleanData(string sql);
	bool dropTable(string sql);
	bool createTable(string sql);
private:
	bool Initialize();
	bool creatSessionPool();
	void closeConnect(Session sess);
	bool checkConnect(Session sess);
	bool unInitialize();
	void closeSessionPool();
	bool execSql(string sql);
	Session connectDb();
	SessionPool *m_pool;
	
};


#endif 
