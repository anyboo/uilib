
1.数据库类获取指针
    QMSqlite *pDb = QMSqlite::getInstance();

2.创建搜索结果内存表 
    pDb->createTable(SEARCH_VIDEO);
    pDb->createTable(SEARCH_DEVICE);

3.插入数据
    分两种模式
    1.一次插入一条记录
    SearchRecord sr1("10000", 10000, curTime1, curTime1, 1 * 1024 * 1024 * 1024, "10000");
    writeData(sr1);
    2.一次插入所有数据
    vector<SearchRecord> Record;
    writeDataByVector(Record);

4.读记录
    传入vector，返回所有结果
    vector<SearchRecord> readRecord;
    GetData(readRecord);

5.删除所有记录
    cleanData();

6.删除表
    dropTable();



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
    pDb->createTable(SEARCH_VIDEO);

    //一次插入所有数据
    vector<SearchVideo> Record; 
    int i;
    for (i = 0; i < 10000; i++)
    {
        SearchVideo sr;
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
        //别名
        sr.set<5>(strName);
        Record.push_back(sr);
    }   
    pDb->writeDataByVector(Record);

    //一次写一条记录
    __time64_t curTime1;
    _time64(&curTime1);
    SearchVideo sr1("10000", 10000, curTime1, curTime1, 1 * 1024 * 1024 * 1024, "10000");
    pDb->writeData(sr1);
    

    //读记录
    vector<SearchVideo> readRecord;
    pDb->GetData(readRecord);
    for (i = 0; i < readRecord.size(); i++)
    {
        std::string str1;
        SearchVideo sr = readRecord[i];
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
    pDb->cleanData(SEARCH_VIDEO);
    //getchar();

    //删除表
    pDb->dropTable(SEARCH_VIDEO);

    //删除设备搜索结果表
    pDb->dropTable(SEARCH_DEVICE);

    //创建设备搜索结果表
    pDb->createTable(SEARCH_DEVICE);

    //一次插入所有数据
    vector<SearchDevice> DeviceRecord;
    for (i = 0; i < 200; i++)
    {
        SearchDevice sd;
        std::string strName = "大华" + std::to_string(i);
        //厂商名称
        sd.set<0>(strName);
        //IP
        sd.set<1>("192.168.0.111");     
        //port
        int port = 37777;
        sd.set<2>(port);        
        DeviceRecord.push_back(sd);
    }
    pDb->writeDataByVector(DeviceRecord);

    //一次写一条记录
    SearchDevice sr2("大华201", "192.168.0.112", 37777);
    pDb->writeData(sr2);

    //读记录
    vector<SearchDevice> DeviceRecord1;
    pDb->GetData(DeviceRecord1);
    for (i = 0; i < DeviceRecord1.size(); i++)
    {
        std::string str1;
        SearchDevice sr = DeviceRecord1[i];
        str1 += sr.get<0>();
        str1.append("--");
        str1 += sr.get<1>();
        str1.append("--");
        str1 += std::to_string(DeviceRecord1[i].get<2>());
        
        std::cout << i << "--" << str1 << std::endl;
    }
    getchar();
    return 0;
}