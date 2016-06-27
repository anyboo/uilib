
1.数据库类获取指针
    QMSqlite *pDb = QMSqlite::getInstance();

2.创建搜索结果内存表 
    createSearchTable();

3.插入数据
    分两种模式
    1.一次插入一条记录
    SearchRecord sr1("10000", 10000, curTime1, curTime1, 1 * 1024 * 1024 * 1024, "10000");
    writeSearchData(sr1);
    2.一次插入所有数据
    vector<SearchRecord> Record;
    writeSearchDataByVector(Record);

4.读记录
    传入vector，返回所有结果
    vector<SearchRecord> readRecord;
    GetSearchData(readRecord);

5.删除所有记录
    cleanSearchData();

6.删除表
    dropSearchTable();



完整DEMO
#include "QMSqlite.h"
#include <iostream>
#include <vector>


int _tmain(int argc, _TCHAR* argv[])
{
    //获取指针
    QMSqlite *pDb = QMSqlite::getInstance();
    

    //创建记录表
    pDb->createSearchTable();

    //一次插入所有数据
    vector<SearchRecord> Record;    
    int i;
    for (i = 0; i < 10000; i++)
    {
        SearchRecord sr;        
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
        __int64 fileszie = 1 * 1024 * 1024 * 1024;
        sr.set<4>(fileszie);
        //别名
        sr.set<5>(strName);
        Record.push_back(sr);
    }   
    pDb->writeSearchDataByVector(Record);

    //一次写一条记录
    __time64_t curTime1;
    _time64(&curTime1);
    SearchRecord sr1("10000", 10000, curTime1, curTime1, 1 * 1024 * 1024 * 1024, "10000");
    pDb->writeSearchData(sr1);
    

    //读记录
    vector<SearchRecord> readRecord;
    pDb->GetSearchData(readRecord);
    for (i = 0; i < readRecord.size(); i++)
    {
        std::string str1;
        SearchRecord sr = readRecord[i];
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
        str1.append(readRecord[i].get<5>());
        
        std::cout << i << "--" << str1 << std::endl;        
    }

    //getchar();

    //删除所有记录
    pDb->cleanSearchData();
    getchar();

    //删除表
    pDb->dropSearchTable();
    return 0;
}