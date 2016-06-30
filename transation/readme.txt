
1.数据库类获取指针
    QMSqlite *pDb = QMSqlite::getInstance();

2.创建搜索结果内存表,输入参数SQL语句,已定义
    pDb->createTable(CREATE_SEARCH_VIDEO_TABLE);
    pDb->createTable(CREATE_SEARCH_DEVICE_TABLE);

3.插入数据
    分两种模式
    1.一次插入一条视频搜索记录
    writeSearchVideo sr1("10000", 10000, curTime1, curTime1, 1 * 1024 * 1024 * 1024);
    string sql(INSERT_SEARCH_VIDEO);
    writeData(sql, sr1);
    2.一次插入所有视频搜索数据
    vector<writeSearchVideo> Record;
    string sql(INSERT_SEARCH_VIDEO);
    writeDataByVector(INSERT_SEARCH_VIDEO, Record);

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
    

    //删除表
    pDb->dropTable(DROP_SEARCH_VIDEO_TABLE);
    //创建记录表
    pDb->createTable(CREATE_SEARCH_VIDEO_TABLE);

    //一次插入所有数据
    std::vector<writeSearchVideo> Record;
    int i;
    for (i = 0; i < 10000; i++)
    {
        writeSearchVideo sr;
        std::string strName = "测试" + std::to_string(i);
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
    string sql(INSERT_SEARCH_VIDEO);
    pDb->writeDataByVector(sql, Record);

    //一次写一条记录
    __time64_t curTime1;
    _time64(&curTime1);
    writeSearchVideo sr1("10000", 10000, curTime1, curTime1, 1 * 1024 * 1024 * 1024);
    sql = INSERT_SEARCH_VIDEO;
    pDb->writeData(sql, sr1);
    

    //读记录
    std::vector<readSearchVideo> readRecord;
    sql = SELECT_ID_SEARCH_VIDEO;
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

    
    //删除所有记录
    pDb->cleanData(DELETE_ALL_SEARCH_VIDEO);
    //getchar();

    

    //创建厂商表
    pDb->dropTable(DROP_SEARCH_FACTORY_TABLE);
    pDb->createTable(CREATE_SEARCH_FACTORY_TABLE);

    //初始化厂商表    
    std::vector<SearchFactory> sfRecord;
    for (i = 0; i < 134; i++)
    {
        SearchFactory sf;
        sf.set<0>(Factory[i][0]);
        sf.set<1>(Factory[i][1]);
        sfRecord.push_back(sf);
    }
    pDb->writeDataByVector(INSERT_SEARCH_FACTORY, sfRecord);

    //查询厂商表
    std::vector<string> sRecord;
    std::string str1("D");
    pDb->searchFactoryName(str1, sRecord);
    for (i = 0; i < sRecord.size(); i++)
    {
        std::cout << i << "--" << sRecord[i] << std::endl;
    }
    
    str1 = "DZ";
    pDb->searchFactoryName(str1, sRecord);
    for (i = 0; i < sRecord.size(); i++)
    {
        std::cout << i << "--2" << "--" << sRecord[i] << std::endl;
    }
    

    getchar();
    return 0;
}

string Factory[134][2] = { "DH", "大华",
"GZLL", "广州利凌",
"DZP", "迪智浦",
"HST", "海视泰",
"XDGC", "新达共创",
"XPAF", "鑫鹏安防",
"LSXJ", "蓝色星际",
"DJL", "德加拉",
"JMS", "俊明视",
"JAGD", "九安光电",
"LAD", "立安达",
"SZSL", "深圳思浪",
"JR", "吉锐",
"TKWY", "泰康伟业",
"XWRJ", "星网锐捷",
"ZTAB", "中田安保",
"TSD", "天视达",
"DYGJ", "东阳国际",
"YDS", "云典视",
"KSA", "旷视安",
"HKWS", "宏康威视",
"SZTW", "深圳同为",
"DLKJ", "大立科技",
"SXAF", "世星安防",
"SWZ", "守卫者",
"HNWE", "霍尼韦尔",
"NNGT", "南宁冠泰",
"GDLY", "广东领域",
"HJKJ", "航景科技",
"BXS", "宝欣盛",
"LAD", "立安达OEM海康",
"XAHF", "西安海方OEM海康",
"BJZFSD", "北京正方时代OEM海康",
"TKWY", "泰康伟业OEM海康",
"TM", "天敏",
"KE", "科恩",
"MFWY", "美防伟业",
"GZBSDZ", "广州邦世电子",
"GZSAAF", "广州视安安防",
"QA", "乔安",
"LSX", "朗视兴",
"YA", "亚安",
"TWGK", "泰威高科",
"HBHS", "华邦海视",
"GZYS", "广州亿视",
"SZHKWS", "深圳宏康威视OEM雄迈",
"AJN", "安居鸟",
"BJYRGS", "北京益融冠世",
"DDJZPT", "东大金智平台",
"GEYPLXJ", "GE硬盘录像机",
"GZMK", "广州米卡61系列",
"GZMK", "广州米卡8081系列NVR",
"HY", "恒忆",
"SZBA", "深圳八安",
"SZBH", "深圳百汇",
"SZHSA", "深圳海视安",
"SZJF", "深圳巨罦",
"SZSQ", "深圳赛清",
"SZXKA", "深圳兴科安",
"TTY", "天天佑",
"HB", "汉邦",
"TDWY", "天地伟业",
"XM", "雄迈",
"GZHX", "广州宏翔",
"SNA", "施耐安",
"JF", "巨峰",
"SALX", "狮安联讯",
"WSD", "沃仕达",
"JXJ", "佳信捷",
"BL", "波粒",
"HK", "海康",
"ZW", "中维",
"YSKJ", "宇视科技",
"ZSS", "臻视顺",
"JSPK", "金山品科",
"XFWS", "旭峰威视",
"XSB", "欣视宝",
"JSA", "佳视安",
"KER", "开尔瑞",
"AN", "安尼",
"AWT", "奥威拓",
"JAA", "君安安",
"XJX", "鑫捷讯",
"HY", "华洋",
"QX", "巧芯",
"DK", "邮科",
"JAB", "居安宝",
"NZ", "南中",
"LZJ", "龙之净",
"SX", "世星",
"WTS", "玮特视",
"AZX", "安之讯",
"BKB", "百科博",
"DTHQ", "大唐华强",
"HY", "海伊",
"ALRS", "安联锐视",
"XTKJ", "旭霆科技",
"HXGJ", "华翔国际",
"EJ", "尔佳",
"HWSX", "宏为视讯",
"XRQ", "欣荣泉",
"XMDZ", "谐美电子",
"AKS", "澳科森",
"HRRS", "恒润睿视",
"AJKJ", "安嘉科技",
"NAKJ", "尼安科技",
"LTSX", "蓝图视讯",
"WDS", "威迪斯",
"SZSBD", "深圳市本德",
"YMKJ", "印梦科技",
"RLT", "锐郎特",
"YFT", "英飞拓",
"LATX", "龙安天下",
"WBDZ", "维邦电子",
"XRHK", "新锐华科",
"DBDZ", "点博电子",
"FHX", "飞鸿信",
"CBL", "超本乐",
"YSSM", "宇烁商贸",
"PNK", "帕尼柯",
"BSJ", "宝视佳",
"KL", "科兰",
"BST", "北视通",
"KDKJ", "科达科技",
"SBZN", "山本智能",
"JDR", "精达锐",
"HZSS", "杭州圣尚",
"HHDF", "汉和东方",
"XBWS", "星宝威视", 
"BJYKJY", "北京英康嘉业",
"GZSTH", "广州市泰豪",
"SZSAY", "深圳市安裕",
"GZZT", "广州左田",
"JT", "吉田"
};