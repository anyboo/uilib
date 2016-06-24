#pragma once
class QMSqlite
{
public:
	
	static const QMSqlite *getInstance();
private:
	QMSqlite();
	~QMSqlite();
	static const QMSqlite* m_instance;
public:
	int Initialize();
	int creatDatabase();
	int connectDb();
	int createSearchTable();
	int GetSearchData();
	int writeSearchData();
	int cleanSearchData();
};

