#ifndef IVIDEOSERVER_H
#define IVIDEOSERVER_H
#include <Windows.h>
#include <windef.h>
#include <vector>
#include <map>
#include <string>
#include <mutex>



#if defined(VIDEOSERVER_LIBRARY)
#  define VIDEOSERVER_EXPORT __declspec(dllexport)
#else
#  define VIDEOSERVER_EXPORT __declspec(dllimport)
#endif

#define SHE_BEGING __try{

#define SHE_END  std::cout<<"she try end"<<std::endl;}__except(EXCEPTION_EXECUTE_HANDLER) \
{ \
    addLog("unkonw error!", __LINE__);\
}

#define SHE_END_RETURN(returnValue) }__except(EXCEPTION_EXECUTE_HANDLER) \
{ \
    addLog("unkonw error!", __LINE__);\
   return returnValue; \
}

#define SHE_END_RETURN_FALSE SHE_END_RETURN(false)


typedef long long  download_handle_t;
typedef long long  play_handle_t;

//厂商ID
enum DeviceFactory 
{
    SISC_IPC_DH = 0,                                  // 大华
    SISC_IPC_GZLL = 1,                                // 广州利凌
    SISC_IPC_DIZHIPU = 2,							  // 迪智浦
    SISC_IPC_HAISHITAI = 3,							  // 海视泰
    SISC_IPC_XINDAGONGCHUANG = 4,					  // 新达共创
    SISC_IPC_XP = 5,								  // 鑫鹏安防
    SISC_IPC_BLUESKY = 6,							  // 蓝色星际
    SISC_IPC_DEJIALA = 7,							  // 德加拉
    SISC_IPC_JUNMINGSHI = 8,                          // 俊明视
    SISC_IPC_JIUAN = 9,							      // 九安光电
    SISC_IPC_LIANDA = 10,							  // 立安达
    SISC_IPC_SILANG = 11,							  // 深圳思浪
    SISC_IPC_JIRUI = 12,							  // 吉锐
    SISC_IPC_TAIKANGWEIYE = 13,					      // 泰康伟业
    SISC_IPC_XINWANGRUIJIE = 14,				      // 星网锐捷
    SISC_IPC_ZHONGTIANANBAO = 15,				      // 中田安保
    SISC_IPC_TIANSHIDA = 16,			    	      // 天视达
    SISC_IPC_DOANGYANG = 17,			    	      // 东阳国际
    SISC_IPC_YUNDIANSHI = 18,			    	      // 云典视
    SISC_IPC_KUANGSHIAN = 19,			    	      // 旷视安
    SISC_IPC_HONGKANGWEISHI = 20,			    	  // 宏康威视
    SISC_IPC_SHENZHENTONGWEI = 21,			    	  // 深圳同为
    SISC_IPC_DALIKEJI = 22,			    	          // 大立科技
    SISC_IPC_SHIXINGANFANG = 23,			    	  // 世星安防
    SISC_IPC_SHOUWEIZHE = 24,			         	  // 守卫者
    SISC_IPC_HUONIWEIER = 25,			         	  // 霍尼韦尔
    SISC_IPC_NANNINGGUANGTAI = 26,			          // 南宁冠泰
    SISC_IPC_GUANGDONGLINYU = 27,			          // 广东领域
    SISC_IPC_HANGJINGKEJI = 28,		     	          // 航景科技

    SISC_IPC_BAOXINGSHENG = 29,		     	          // 宝欣盛
    SISC_IPC_LANDAOEMHK = 30,		     	          // 立安达OEM海康
    SISC_IPC_XIANHAIFANG = 31,		     	          // 西安海方OEM海康
    SISC_IPC_BEIJINGZHENGFANG = 32,		     	      // 北京正方时代OEM海康
    SISC_IPC_TAIKANGWEIYEOEMHK = 33,		     	  // 泰康伟业OEM海康
    SISC_IPC_TIANMIN = 34,		     	              // 天敏
    SISC_IPC_KEEN = 35,		     	                  // 科恩
    SISC_IPC_MEIFANGWEIYE = 36,		     	          // 美防伟业
    SISC_IPC_GUANGZHOUBANGSHI = 37,		     	      // 广州邦世电子
    SISC_IPC_GUANGZHOUSHIAN = 38,		     	      // 广州视安安防
    SISC_IPC_QIAOAN = 39,		     	              // 乔安
    SISC_IPC_LANGSHIXIN = 40,		     	          // 朗视兴
    SISC_IPC_YAAN = 41,		     	                  // 亚安
    SISC_IPC_TAIWEIGAOKE = 42,		     	          // 泰威高科
    SISC_IPC_HUABANGHAISHI = 43,		     	      // 华邦海视
    SISC_IPC_GUANGZHOUYISHI = 44,		     	      // 广州亿视
    SISC_IPC_HONGKANGWEISHIOEMXM = 45,		     	  // 深圳宏康威视OEM雄迈
    SISC_IPC_ANJULIAO = 46,		     	              // 安居鸟
    SISC_IPC_YIRONGGUANSHI = 47,		     	      // 北京益融冠世
    SISC_IPC_DONGDAJIZHI = 48,		     	          // 东大金智平台
    SISC_IPC_GERECORDER = 49,		     	          // GE硬盘录像机

    SISC_IPC_MIKA61 = 50,		     	              // 广州米卡61系列
    SISC_IPC_MIKA8081 = 51,		     	              // 广州米卡8081系列NVR
    SISC_IPC_HENGYI = 52,		     	              // 恒忆
    SISC_IPC_BAAN = 53,		         	              // 深圳八安
    SISC_IPC_BAIHUI = 54,		         	          // 深圳百汇
    SISC_IPC_HAISHIAN = 55,		         	          // 深圳海视安
    SISC_IPC_JUFU = 56,		            	          // 深圳巨罦
    SISC_IPC_SAIQING = 57,		         	          // 深圳赛清
    SISC_IPC_XINGKEAN = 58,		         	          // 深圳兴科安
    SISC_IPC_TIANTIANYOU = 59,		         	      // 天天佑

    SISC_IPC_HB = 60,                                 // 汉邦
    SISC_IPC_TDWY = 61,                               // 天地伟业
    SISC_IPC_XM = 62,                                 // 雄迈
    SISC_IPC_GZHX = 63,   						      // 广州宏翔
    SISC_IPC_SNA = 64,   						      // 施耐安
    SISC_IPC_JF = 65,   						      // 巨峰
    SISC_IPC_SALX = 66,   						      // 狮安联讯
    SISC_IPC_WSD = 67,   						      // 沃仕达
    SISC_IPC_JXJ = 68,   						      // 佳信捷
    SISC_IPC_BOLI = 69,   						      // 波粒
    SISC_IPC_HIKVISION = 100,                         // 海康
    SISC_IPC_ZHONGWEI = 101,                          // 中维
    SISC_IPC_YUSHIKEJI = 102,                          // 宇视科技

    SISC_IPC_ZHENSHISHUN = 1001,                      // 臻视顺
    SISC_IPC_JINSHANPINKE,                            // 金山品科
    SISC_IPC_XUFENGWS,                                // 旭峰威视
    SISC_IPC_XINGSHIBAO,                              // 欣视宝

    SISC_IPC_ZSB,                                  // 佳视安
    SISC_IPC_KER,                            // 开尔瑞
    SISC_IPC_AN,                                // 安尼
    SISC_IPC_AWT,                              // 奥威拓

    SISC_IPC_JAA,                      // 君安安
    SISC_IPC_XJX,                            // 鑫捷讯
    SISC_IPC_HY,                                // 华洋
    SISC_IPC_QX,                              // 巧芯

    SISC_IPC_YK,                      // 邮科
    SISC_IPC_JAB,                            // 居安宝
    SISC_IPC_NZ,                                // 南中
    SISC_IPC_LZJ,                              // 龙之净

    SISC_IPC_SX,                      // 世星
    SISC_IPC_WTS,                            // 玮特视
    SISC_IPC_AZX,                                // 安之讯
    SISC_IPC_BKB,                              // 百科博

    SISC_IPC_DTHQ,                      // 大唐华强
    SISC_IPC_HAIY,                            // 海伊
    SISC_IPC_ALKJ,                                // 安联锐视
    SISC_IPC_XTKJ,                              // 旭霆科技

    SISC_IPC_HXGJ,                      // 华翔国际
    SISC_IPC_ERJIA,                            // 尔佳
    SISC_IPC_HWSX,                                // 宏为视讯
    SISC_IPC_XINGRQ,                              // 欣荣泉

    SISC_IPC_XMDZ,                      // 谐美电子
    SISC_IPC_AOKS,                            // 澳科森
    SISC_IPC_HRRS,                                // 恒润睿视
    SISC_IPC_AJKJ,                              // 安嘉科技

    SISC_IPC_NAKJ,                      // 尼安科技
    SISC_IPC_LTSX,                            // 蓝图视讯
    SISC_IPC_WEIDS,                                // 威迪斯
    SISC_IPC_SZSBD,                              // 深圳市本德

    SISC_IPC_YMKJ,                      // 印梦科技
    SISC_IPC_RUILT,                            // 锐郎特
    SISC_IPC_YINGFT,                                // 英飞拓
    SISC_IPC_LATX,                              // 龙安天下

    SISC_IPC_WBDZ,                      // 维邦电子
    SISC_IPC_XRHK,                            // 新锐华科
    SISC_IPC_DBDZ,                                // 点博电子
    SISC_IPC_FEIHX,                              // 飞鸿信

    SISC_IPC_CHAOBL,                      // 超本乐
    SISC_IPC_YSSM,                            // 宇烁商贸
    SISC_IPC_PANK,                                // 帕尼柯
    SISC_IPC_BAOJS,                              // 宝视佳

    SISC_IPC_KELAN,                      // 科兰
    SISC_IPC_BEIST,                            // 北视通
    SISC_IPC_KDKJ,                                // 科达科技
    SISC_IPC_SBZN,                              // 山本智能

    SISC_IPC_JINGDR,                      // 精达锐
    SISC_IPC_HZSS,                            // 杭州圣尚
    SISC_IPC_HTDF,                                // 汉和东方
    SISC_IPC_XBWS,                              // 星宝威视

    SISC_IPC_BJYKJY,                      // 北京英康嘉业
    SISC_IPC_GZSTH,                            // 广州市泰豪
    SISC_IPC_SZSAY,                                // 深圳市安裕
    SISC_IPC_GZZT,                              // 广州左田

    SISC_IPC_UNDEFINE = 9999,                         // 未定义
};

// 设备信息
struct DeviceInfo
{
    DeviceInfo()
    {
        Factory = SISC_IPC_UNDEFINE;
        szIP = "";
        nPort = 0;
    }

    DeviceInfo(const DeviceInfo &other)
    {
        Factory = other.Factory;
        szIP = other.szIP;
        nPort = other.nPort;
    }
    DeviceInfo& operator = (const DeviceInfo &other)
    {
        if (this != &other)
        {
            Factory = other.Factory;
            szIP = other.szIP;
            nPort = other.nPort;
         }

        return *this;
    }

    DeviceFactory Factory;
    std::string szIP;
    __int32 nPort;
};

struct IVideoServer;
struct IVideoServerFactory
{
    IVideoServerFactory() : m_init(false)
    {
        m_OemFlag = false;
    }
    virtual ~IVideoServerFactory()
    {
    }
    virtual void destroy()
    {
        delete this;
    }

    //初始化SDK
    virtual bool init() = 0;
    /*
     * 释放SDK
     */
    virtual void clean() = 0;
    //创建视频服务器
    virtual IVideoServer* create() = 0;
    //厂商名称
    virtual const char* name() = 0;
    //厂商ID
    virtual DeviceFactory factory() = 0;
    //视频后缀
    virtual void videoFileExterns(std::vector<std::string>& externs) = 0;
    virtual const char* getLastError()
    {
        return m_sLastError.c_str();
    }
    //默认端口
    virtual int defaultPort() = 0;
    //默认用户
    virtual const char* defaultUser() = 0;
    //默认密码
    virtual const char* defaultPasswords() = 0;

    virtual bool IsOEMFac()
    {
        return m_OemFlag;
    }
protected:
    bool m_init;
    std::string  m_sLastError;

protected:
    bool m_OemFlag;
};
//单个视频文件信息
struct RecordFile
{
    RecordFile()
    {
        channel = 0;
        size = 0;
        pPrivateData = nullptr;
        PrivateDataDataSize = 0;
    }

    ~RecordFile()
    {
        if (nullptr != pPrivateData)
        {
            delete pPrivateData;
            pPrivateData = nullptr;
            PrivateDataDataSize = 0;
        }
    }

    RecordFile(const RecordFile& other)
    {
        channel = other.channel;
        size = other.size;
        name = other.name;
        beginTime = other.beginTime;
        endTime = other.endTime;
        pPrivateData = nullptr;
        PrivateDataDataSize = 0;
        setPrivateData(other.pPrivateData, other.PrivateDataDataSize);
    }

    RecordFile& operator= (const RecordFile& other)
    {
        if (&other == this)
        {
            return *this;
        }
        channel = other.channel;
        size = other.size;
        name = other.name;
        beginTime = other.beginTime;
        endTime = other.endTime;
        setPrivateData(other.pPrivateData, other.PrivateDataDataSize);
        return *this;
    }

    void setPrivateData(void* pData, int size)
    {
        if (nullptr != pPrivateData)
        {
            delete pPrivateData;
            pPrivateData = nullptr;
            PrivateDataDataSize = 0;
        }


        if (size > 0)
        {
            pPrivateData = new char[size];
            PrivateDataDataSize = size;
            memcpy(pPrivateData, pData, size);
        }
    }
    //取得私有数据
    void* getPrivateData() const
    {
        return pPrivateData;
    }
    __int32 getPrivateDataSize() const
    {
        return PrivateDataDataSize;
    }

    __int32 channel;      //通道
    __int64 size;         //文件大小(byte)
    std::string  name;    //文件名称
    __time64_t beginTime; //本地时间
    __time64_t endTime;   //本地时间
    char* pPrivateData;   //私有数据
    __int32 PrivateDataDataSize;//私有数据大小
};

struct IVideoServer
{

    IVideoServer() : m_lLoginHandle(0)
    {

    }

    virtual void destroy()
    {
        delete this;
    }

    virtual ~IVideoServer()
    {
    }

    virtual const char* getLastError()
    {
        return m_sLastError.c_str();
    }
    //深度克隆一个服务器
    virtual IVideoServer* clone() = 0;
    /*
     *  登录函数
     *  @param [in] IP 登录的IP
     *  @param [in] port 端口号
     *  @param [in] user 用户名
     *  @param [in] password 密码
     *  @param [out] channels 通道号
     *  @return 是否成功
     */
    virtual bool login(const char* IP, __int32 port, const char* user,
        const char* password, std::map<__int32, std::string>& channels) = 0;
    //退出
    virtual bool logout() = 0;
    /*
     * 获取文件列表
     *  @param [out] files 文件列表
     *  @param [in] nChannelId 通道号
     *  @param [in] timeStart 开始时间
     *  @param [in] timeEnd 结束时间
     *  @return 是否成功
     */
    virtual bool GetRecordFileList(std::vector<RecordFile>& files, /*__int32 nChannelId*/const std::vector<int>& channelVec, __time64_t timeStart,
                                    __time64_t timeEnd) = 0;
    /*
     * 文件下载
     *  @param [in] saveFileName 保存文件名（全路径）
     *  @param [in] file 视频文件信息
     *  @param [out] hdl 返回下载ID
     *  @return 成功与否
     */
    virtual bool downLoadByRecordFile(const char* saveFileName, const RecordFile& file, download_handle_t& hdl) = 0;
    /*
     *  停止下载
     *  @param [in] h downLoadByRecordFile返回的下载ID
     *  @return 返回true/false
     */
    virtual bool stopDownload(download_handle_t h) = 0;
    /*
     *  视频预览
     *  @param [in] file 视频文件信息
     *  @param [in] hwnd 播放窗口句柄
     *  @param [out] playbackHandle 返回播放ID
     *  @return 返回true/false
     */
    virtual bool  PlayBackByRecordFile(const RecordFile& file, HWND hwnd, play_handle_t& playbackHandle) = 0;

    /*
     *  设置播放位置
     *  @param [in] playbackHandle PlayBackByRecordFile返回的播放ID
     *  @param [in] pos （0 - 100）
     *  @return 返回true/false
     */
    virtual bool SetPlayBack(__int64 playbackHandle, __int32 pos) = 0;
    /*
     *  停止视频预览
     *  @param [in] playbackHandle PlayBackByRecordFile返回的播放ID
     *  @param [in] mPause 是否是暂停 1是暂停 0不是
     *  @return 返回true/false
     */
    virtual bool StopPlayBack(__int64 playbackHandle, __int32 mPause) = 0;
    virtual bool getPlayBackPos(__int64 playbackHandle, __int32* pos)
    {
        return false;
    }

    virtual bool getDownloadPos(download_handle_t h, __int64* totalSize, __int64* currentSize, bool* failed)
    {
        return false;
    }

protected:
    std::string m_sLastError;
    long m_lLoginHandle;
    std::recursive_mutex m_mutexDownload;
};

#if defined(VIDEOSERVER_LIBRARY)
extern "C" VIDEOSERVER_EXPORT IVideoServerFactory* VideoServerFactory();
#else

typedef void* (*pfExternFun)(IVideoServer* server, void* externParams);

//是否能预览视频
//externParams 类型 忽略
//返回值类型 bool (默认true)
#define EXTERN_FUN_CAN_PLAY_BACK "canPlayBack"


typedef IVideoServerFactory* (*pfGetVideoServerFactory)();
#endif


#endif // IVIDEOSERVER_H
