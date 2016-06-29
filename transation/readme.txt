
1.数据库类获取指针
    QMSqlite *pDb = QMSqlite::getInstance();

2.创建搜索结果内存表,输入参数SQL语句,已定义
    pDb->createTable(CREATE_SEARCH_VIDEO_TABLE);
    pDb->createTable(CREATE_SEARCH_DEVICE_TABLE);

3.插入数据
    分两种模式
    1.一次插入一条记录
    writeSearchVideo sr1("10000", 10000, curTime1, curTime1, 1 * 1024 * 1024 * 1024);
    writeData(sr1);
    2.一次插入所有数据
    vector<writeSearchVideo> Record;
    writeDataByVector(Record);

4.读记录
    传入查询语句，vector，返回所有结果
    vector<readSearchVideo> readRecord;
    string sql(SELECT_ID_SEARCH_VIDEO);
    sql.append("100");
    GetData(sql, readRecord);

5.删除所有记录
    传入查询语句,已定义
    cleanData(DELETE_ALL_SEARCH_VIDEO);

6.删除表
    传入查询语句,已定义
    dropTable(DROP_SEARCH_DEVICE_TABLE);



完整DEMO
#include "stdafx.h"
#include "QMSqlite.h"
#include <iostream>
#include <vector>


int _tmain(int argc, _TCHAR* argv[])
{
    //获取指针
    QMSqlite *pDb = QMSqlite::getInstance();
    

    //创建记录表
    pDb->createTable(CREATE_SEARCH_VIDEO_TABLE);

    //一次插入所有数据
    vector<writeSearchVideo> Record;
    int i;
    for (i = 0; i < 10000; i++)
    {
        writeSearchVideo sr;
        std::string strName = "test" + std::to_string(i);
        //文件名称
        sr.set<0>(strName);
        //通道号
        sr.set<1>(i);
        //开始时间
        __time64_t curTime;
        _time64(&curTime);
        sr.set<2>(curTime);
        //结束时间
        sr.set<3>(curTime);
        //文件大小
        __int64 fileszie = (__int64)100*1024*1024*1024;
        sr.set<4>(fileszie);        
        Record.push_back(sr);
    }   
    pDb->writeDataByVector(Record);

    //一次写一条记录
    __time64_t curTime1;
    _time64(&curTime1);
    writeSearchVideo sr1("10000", 10000, curTime1, curTime1, 1 * 1024 * 1024 * 1024);
    pDb->writeData(sr1);
    

    //读记录
    vector<readSearchVideo> readRecord;
    string sql(SELECT_ID_SEARCH_VIDEO);
    sql.append("100");
    cout << sql << endl;
    pDb->GetData(sql, readRecord);
    for (i = 0; i < readRecord.size(); i++)
    {
        std::string str1;
        readSearchVideo sr = readRecord[i];
        str1 += sr.get<0>();
        str1.append("--");
        str1 += std::to_string(readRecord[i].get<1>());
        str1.append("--");
        str1.append(std::to_string(readRecord[i].get<2>()));
        str1.append("--");
        str1.append(std::to_string(readRecord[i].get<3>()));
        str1.append("--");
        str1.append(std::to_string(readRecord[i].get<4>()));
        str1.append("--");
        str1.append(std::to_string(readRecord[i].get<5>()));
        
        std::cout << i << "--" << str1 << std::endl;        
    }

    //getchar();

    //删除所有记录
    pDb->cleanData(DELETE_ALL_SEARCH_VIDEO);
    //getchar();

    //删除表
    pDb->dropTable(DROP_SEARCH_DEVICE_TABLE);

    getchar();
    return 0;
})