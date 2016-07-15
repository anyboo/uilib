#ifndef QM_FILE_SQLITE_H
#define QM_FILE_SQLITE_H


#include "Poco/Tuple.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>
#include <vector>
#include "DatabaseException.h"

using namespace std;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::Statement;

//��¼��Ϣ�����
#define CREATE_LOGIN_DEVICE_INFO_TABLE      "CREATE TABLE LoginDeviceInfo(name VARCHAR(100), ip VARCHAR(20), port INTEGER, user VARCHAR(50), pass VARCHAR(50), factory INTEGER, id VARCHAR(100))"
#define DELETE_ALL_LOGIN_DEVICE_INFO		"DELETE from LoginDeviceInfo"
#define DROP_LOGIN_DEVICE_INFO_TABLE		"DROP TABLE IF EXISTS LoginDeviceInfo"
#define SELECT_ALL_LOGIN_DEVICE_INFO		"SELECT * FROM LoginDeviceInfo"
#define INSERT_LOGIN_DEVICE_INFO			"INSERT INTO LoginDeviceInfo VALUES(:name, :ip, :port, :user, :pass, :factory, :id)"
typedef Poco::Tuple<std::string, std::string, int, std::string, std::string, int, std::string> LoginDeviceInfo;

//������Ϣ�����
#define CREATE_DOWNLOAD_VIDEO_INFO_TABLE	"CREATE TABLE DownloadVideoInfo()"
#define DELETE_ALL_DOWNLOAD_VIDEO_INFO		"DELETE from DownloadVideoInfo"
#define DROP_DOWNLOAD_VIDEO_INFO_TABLE		"DROP TABLE IF EXISTS DownloadVideoInfo"
#define SELECT_ALL_DOWNLOAD_VIDEO_INFO		"SELECT * FROM DownloadVideoInfo"
#define INSERT_DOWNLOAD_VIDEO_INFO			"INSERT INTO DownloadVideoInfo VALUES()"

//������־��
#define CREATE_MOTION_LOG_TABLE				"CREATE TABLE MotionLog(time DATETIME, operation NVARCHAR(200), describe NVARCHAR(500))"
#define DELETE_ALL_MOTION_LOG				"DELETE from MotionLog"
#define DROP_MOTION_LOG_TABLE				"DROP TABLE IF EXISTS MotionLog"
#define SELECT_ALL_MOTION_LOG				"SELECT * FROM MotionLog"
#define INSERT_MOTION_LOG					"INSERT INTO MotionLog VALUES(:time, :operation, :describe)"

//������ѯ��
#define CREATE_SEARCH_AN_JIAN_TABLE			"CREATE TABGLE SearchAnJian(time DATETIME, address NVARCHAR(300), gatherer NVARCHAR(200), casename NVARCHAR(300), casedescribe NVARCHAR(1000), IP VARCHA(20), describe NVARCHAR(1000))"
#define DELETE_ALL_SEARCH_AN_JIAN			"DELETE from SearchAnJian"
#define DROP_SEARCH_AN_JIAN_TABLE			"DROP TABLE IF EXISTS SearchAnJian"
#define SELECT_ALL_SEARCH_AN_JIAN			"SELECT * FROM SearchAnJian"
#define INSERT_SEARCH_AN_JIAN				"INSERT INTO MotionLog VALUES(:time, :address, :gatherer, :casename, :casedescribe, :IP, :describe)"

class QFileSqlite
{
public:	
	
	static QFileSqlite *getInstance();
private:
	QFileSqlite();
	~QFileSqlite();	
	QFileSqlite(QFileSqlite const& other);            
	QFileSqlite& operator=(QFileSqlite const& other); 

	
public:
	template<typename T> 
	bool GetData(string sql, std::vector<T>& Record)
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
			throw DatabaseException(ex.displayText());
			closeConnect(sess);
			return false;
		}

		return true;
	}
	template<typename T>
	bool writeData(string sql, T searchrecode)
	{
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
