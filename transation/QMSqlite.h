#ifndef QM_SQLITE_H
#define QM_SQLITE_H

#include "Poco/Data/Session.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>
#include <vector>

typedef Poco::Tuple<std::string, int, __time64_t, __time64_t, LONGLONG, std::string> SearchRecord;


using namespace Poco::Data;
using namespace std;

class QMSqlite
{
public:
	
	static const QMSqlite *getInstance();
private:
	QMSqlite();
	~QMSqlite();
	static const QMSqlite* m_instance;

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
	
	bool createSearchTable();
	bool GetSearchData(std::vector<SearchRecord>&Record);
	bool writeSearchData(SearchRecord searchrecode);
	bool writeSearchDataByVector(std::vector<SearchRecord>&Record);
	bool cleanSearchData();
	bool dropSearchTable();
private:
	bool Initialize();
	bool creatSessionPool();
	void closeConnect(Session sess);
	bool checkConnect(Session sess);
	Session connectDb();
	SessionPool *m_pool;
	
};


#endif 
