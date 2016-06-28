#ifndef QM_SQLITE_H
#define QM_SQLITE_H

#include "Poco/Data/Session.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>
#include <vector>

typedef Poco::Tuple<std::string, int, __time64_t, __time64_t, __int64, std::string> SearchVideo;
typedef Poco::Tuple<std::string, std::string, int> SearchDevice;


typedef enum _TABLE_TAG
{
	SEARCH_DEVICE = 1,               //search device table
	SEARCH_VIDEO,                    //search video table
}TABLE_TAG;


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
	bool GetData(std::vector<SearchVideo>& Record);
	bool GetData(std::vector<SearchDevice>& Record);
	bool writeData(SearchVideo searchrecode);
	bool writeData(SearchDevice searchrecode);
	bool writeDataByVector(std::vector<SearchVideo>& Record);
	bool writeDataByVector(std::vector<SearchDevice>& Record);
	bool cleanData(int tag);
	bool dropTable(int tag);
	bool createTable(int tag);
private:
	bool Initialize();
	bool creatSessionPool();
	void closeConnect(Session sess);
	bool checkConnect(Session sess);
	bool unInitialize();
	void closeSessionPool();
	Session connectDb();
	SessionPool *m_pool;
	
};


#endif 
