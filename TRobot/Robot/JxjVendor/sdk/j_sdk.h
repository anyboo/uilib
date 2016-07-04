
#ifndef __J_SDK_H__
#define __J_SDK_H__

#include <stdio.h>
#include "stdint.h"

#define J_SDK_MAX_ID_LEN					32
#define J_SDK_MAX_IP_LEN					64
#define J_SDK_MAX_HOST_LEN					128

#define J_SDK_MAX_MANU_INFO_LEN				128
#define J_SDK_MAX_DATA_LEN					32
#define J_SDK_MAX_VERSION_LEN				12

#define J_SDK_MAX_NETMASK_LEN				32
#define J_SDK_MAX_GATEWAY_LEN				32
#define J_SDK_MAX_MAC_LEN					32
#define J_SDK_MAX_DNS_LEN					32

#define J_SDK_MAX_ACCOUNT_LEN				32
#define J_SDK_MAX_PASSWD_LEN				32
#define J_SDK_MAX_PATH_LEN					128
#define J_SDK_MAX_ADDR_LEN					64

#define J_SDK_MAX_ALARM_DATA_LEN			128

#define J_SDK_MAX_COMMAND_LEN				64
#define J_SDK_MAX_PAGE_SIZE					1024

#define J_SDK_MAX_SEG_SZIE					4
#define J_SDK_MAX_TIME_LEN					32

#define J_SDK_MAX_DISK_NUMBER				16
#define J_SDK_MAX_DECTION_AREA_SIEZ			16
#define J_SDK_MAX_URL_LEN					128

#define J_SDK_MAX_NAME_LEN					64
#define J_SDK_MAX_PWD_LEN					64

#define J_SDK_MAX_TEXT_DATA_LEN				128
#define J_SDK_MAX_UPGRADE_DATA_LEN			200

#define J_SDK_MAX_STORE_LOG_SIZE  			24
#define J_SDK_MAX_STREAM_SIZE				4
#define J_SDK_MAX_WIFI_AP_SIZE				16

#define J_MAX_CHANNEL_INFO_SIZE				32
#define J_FILE_PROPERTY_SIZE				64

#define MAX_IVS_AVD_RULES 					5

#define MAX_NET_DISK_NAME_LEN				256


#define J_SDK_MAX_CHN_SIZE  				32
#define J_SDK_MAX_ALARMIN_SIZE 				32
#define J_SDK_MAX_ALARMOUT_SIZE 			32

#define J_SDK_MAX_PRESET_PORT_SIZE 			255
#define J_SDK_MAX_CRUISE_PORT_SIZE 			32
#define J_SDK_MAX_CRUISE_WAY_SIZE 			16

#define J_SDK_MAX_NETCARD_SIZE              8
#define J_SDK_DEF_BASE_YEAR                 1900
#define J_SDK_MAX_LAYER_NUM					128

#ifndef _WIN32
# define DEF_PRAGMA_PACKED					__attribute__((packed))
#else
# define DEF_PRAGMA_PACKED					
#endif

#pragma pack(push, 1)

//************************* enmu *******************************
typedef enum __PuType
{
    J_SDK_DVR=0,
    J_SDK_DVS,
    J_SDK_IPC,
    J_SDK_IPNC,
    J_SDK_SDEC,                         //�������
    J_SDK_DEC,                          //Ӳ������
    J_SDK_NVR,
    J_SDK_HVR,
    
    J_SDK_OTHER_TYPE = 255
}JPuType;

typedef enum __IPCFactoryType
{
	JXJ_IPC=0,
	HIK_IPC,
	DAH_IPC
}JIPCFactoryType;

typedef enum __PuSubType
{
	J_SDK_RIFLE=0,						//ǹ��
	J_SDK_INFRARED,						//����
	J_SDK_HEMISPHERE,					//����
	J_SDK_HIGH_SPEED_DOME,				//������
}JPuSubType;

typedef enum __ProtocolType
{
	J_SDK_TCP=0,
	J_SDK_UDP
}JProtocolType;


typedef enum __NetworkType
{
	J_SDK_ETH0=0,
	J_SDK_WIFI,
	J_SDK_3G,
	J_SDK_MAX_NETWORK_TYPE
}JNetworkType;

typedef enum __PublicDomain
{
	J_SDK_OPTIONS=0,
	J_SDK_TEARDOWN
}JPublicDomain;

typedef enum __MediaType
{
	J_SDK_R_TIME_AV_S=0,				//ʵʱ����Ƶ��
	J_SDK_R_TIME_CAPTURE_I,				//ʵʱץ��ͼƬ
	J_SDK_HISTORY_AV_DEMAND,			//��ʷ����Ƶ���㲥��
	J_SDK_HISTORY_AV_DOWNLOAD,			//��ʷ����Ƶ�����أ�
	J_SDK_HISTORY_IMAGE					//��ʷͼƬ
}JMediaType;

typedef enum __VideoFormat
{
	J_SDK_PAL=0,
	J_SDK_NTSC
}JVideoFormat;

typedef enum __BitRate
{
	J_SDK_ABR=0,
	J_SDK_CBR,
	J_SDK_VBR,
	J_SDK_FIXQP,
	J_SDK_BUTT
}JBitRate;

typedef enum __DiskType
{
	J_SDK_SATA=0,
	J_SDK_USB,
	J_SDK_ISCSI,
	J_SDK_NFS,
	J_SDK_SD
}JDiskType;

typedef enum __DiskStatus
{
	J_SDK_USING=0,
	J_SDK_MOUNTED,
	J_SDK_UNMOUNT
}JDiskStatus;

typedef enum __WeekDay
{
	J_SDK_SUNDAY=0,
	J_SDK_MONDAY,
	J_SDK_TUESDAY,
	J_SDK_WEDNESDAY,
	J_SDK_THURSDAY,
	J_SDK_FRIDAY,
	J_SDK_SATURDAY,
	J_SDK_EVERYDAY,
	J_SDK_MAX_DAY_SIZE
}JWeekDay;

typedef enum __SysFileType
{
	J_SDK_FAT16=0,
	J_SDK_FAT32,
	J_SDK_NTFS,
	J_SDK_EXT,
	J_SDK_EXT2,
	J_SDK_EXT3
}JSysFileType;

typedef enum __AlarmType
{
	J_SDK_MOVE_ALARM=0,
	J_SDK_LOST_ALARM,
	J_SDK_HIDE_ALARM,
	J_SDK_IO_ALARM,
	J_SDK_AVD_ALARM,				    // ���ܷ�����Ƶ��ϱ���	

	//+++++++++++++IPC++++++++++++//
    J_SDK_NETWORK_FAULT_ALARM,          // ipc ������ϱ��� --> JNetCardAlarm
    J_SDK_PIR_ALARM,					// �������̽�ⱨ��
	J_SDK_IOT_ANALYSE_ALARM = 100,      // �п�Ժ�����������ܷ������� --> JAnalyseAlarm
    J_SDK_PEA_OSC_ALARM = 101,	        //�ܽ硢���ߡ���Ʒ�����ȱ��� --> JPeaOscAlarm
}JAlarmType;

typedef enum __NetCardAlarm
{
    NETWORK_DISCONNECT = 0x2,
    NETWORK_RECONNECT  = 0x4,
}JNetCardAlarm;

typedef enum __AnalyseAlarm
{
    SUBMIT_HERD_ANALYSE_DATA = 0x2,
}JAnalyseAlarm;

typedef enum __PeaOscAlarm
{
    UNKNOW_EVENT = 0x1,
    SIMPLE_LINE  = 0x2,
    DOUBLE_LINE  = 0x4,
    ALERT_AREA_ALARM = 0x8,
    HOVER            = 0x10,
    HAND_DOWN        = 0x20,
    LOSING_SOMETHING = 0x40,
}JPeaOscAlarm;

typedef enum __IOType
{
	NORMAL_CLOSE=0,
	NORMAL_OPEN
}JIOType;

typedef enum __Action
{
	ACTION_UNPACK,
	ACTION_PACK,
}JAction;

//�ֱ���
typedef enum __Resolution
{
	J_SDK_VIDEO_QCIF		= 0x01,		//320*192
	J_SDK_VIDEO_CIF			= 0x02,		//352*288	n��352*240
	J_SDK_VIDEO_HD1			= 0x03,		//704*288	n��704*240
	J_SDK_VIDEO_D1			= 0x04,		//704*576	n��704*480
	
	J_SDK_VIDEO_QQVGA	    = 0x20,		//160*112
	J_SDK_VIDEO_QVGA		= 0x21,		//320*240
	J_SDK_VIDEO_VGA			= 0x22,		//640*480
	J_SDK_VIDEO_SVGA		= 0x23,		//800*600
	J_SDK_VIDEO_UXGA		= 0x24,		//1600*1200
	J_SDK_VIDEO_XGA			= 0x25,		//1024*768
	J_SDK_VIDEO_WXGA		= 0x26,		//1280*800
	J_SDK_VIDEO_SXGA		= 0x27,		//1280*1024

	J_SDK_VIDEO_720P		= 0x40,		//1280*720
	J_SDK_VIDEO_960P		= 0x41,		//1280*960
	J_SDK_VIDEO_1080P		= 0x42,		//1920*1080	
	J_SDK_VIDEO_3Mega	  	= 0x43,		//2048*1536
	J_SDK_VIDEO_5Mega	  	= 0x44,		//2560*1920
	J_SDK_VIDEO_UltraHD   	= 0x45,		//3840*2160 4k ultra hd   
}JResolution;

//��Ƶ����Ƶ��������
typedef enum __AVCodeType
{
	J_SDK_AV_CODEC_UNKNOWN = 0,

	J_SDK_AV_VIDEO_CODEC_START = 10,
	J_SDK_AV_VIDEO_H264,
	J_SDK_AV_VIDEO_MPEG4,
	J_SDK_AV_VIDEO_MJPEG,
	J_SDK_AV_VIDEO_CODEC_END,

	J_SDK_AV_AUDIO_CODEC_START = 20,
	J_SDK_AV_AUDIO_G711A,
	J_SDK_AV_AUDIO_G711U,
	J_SDK_AV_AUDIO_CODEC_END
}JAVCodeType;

typedef enum __AudioInputType
{
	J_SDK_AUDIO_MIC_IN=0,
	J_SDK_AUDIO_LINE_IN
}JAudioInputType;


typedef enum __EncodeLevel
{
	J_SDK_BASELINE,
	J_SDK_MAIN,
	J_SDK_HIGH
}JEncodeLevel;

typedef enum __RecodeType
{
	TIMER_RECODE  = 0x00000001,
	ALARM_RECODE  = 0x00000010,
	MOVE_RECODE   = 0x00000100,
	MANUAL_RECODE = 0x00001000,
	LOST_RECODE   = 0x00010000,
	HIDE_RECODE   = 0x00100000,
	//+++++++++++IPC+++++++++++++++//
    NET_FAULT_RECODE = 0x00200000,
	PIR_RECODE 	  = 0x00400000,
	ALL_RECODE    = 0xFFFFFFFF,
}JRecodeType;

typedef enum __PTZAction
{
	PTZ_STOP        = 0,			//ֹͣ
	PTZ_AUTO        = 1,			//�Զ�
	
	PTZ_LEFT        = 2,			//��
	PTZ_RIGHT       = 3,			//��
	PTZ_UP          = 4,			//��
	PTZ_DOWN        = 5,			//��

	PTZ_LEFT_UP     = 6,			//����
	PTZ_LEFT_DOWN   = 7,			//����
	PTZ_RIGHT_UP    = 8,			//����
	PTZ_RIGHT_DOWN  = 9,			//����
	
	PTZ_SET_PP      = 10,			//����Ԥ�õ�
	PTZ_USE_PP      = 11,			//����Ԥ�õ�
	PTZ_DEL_PP      = 12,			//ɾ��Ԥ�õ�
	
	PTZ_ADD_ZOOM    = 13,			//����Ŵ�
	PTZ_SUB_ZOOM    = 14,			//������С
	PTZ_ADD_FOCUS   = 15,			//Զ����
	PTZ_SUB_FOCUS   = 16,			//������
	
	PTZ_TURN_ON     = 17,			//�����ƹ�
	PTZ_TURN_OFF    = 18,			//�رյƹ�
	
	PTZ_WIPERS_ON   = 19,			//������ˢ
	PTZ_WIPERS_OFF  = 20,			//�ر���ˢ

	PTZ_SET_CRZ		= 21,			//����Ѳ��
	PTZ_START_CRZ	= 22,			//��ʼѲ��
	PTZ_STOP_CRZ	= 23,			//ֹͣѲ��
	PTZ_DEL_CRZ		= 24,			//ɾ��Ѳ��
	PTZ_IRIS_OPEN	= 25,			//��Ȧ+
	PTZ_IRIS_CLOSE	= 26,			//��Ȧ-
}JPTZAction;

typedef enum __AvdType 
{
    AVD_BRIGHT         = 0,       /*�����쳣*/
    AVD_CLARITY        = 1,       /*�������쳣*/
    AVD_NOISE          = 2,       /*�����쳣*/
    AVD_COLOR          = 3,       /*ƫɫ*/  
    AVD_SCENE          = 4,       /*�����任*/
}JAvdType;
  
//++++++++++++++++++++ivs ���ܷ�����Ƶ����¼�����+++++++++++++++++++++++++++++++++++++++++++
typedef enum __AvdEventType /*avd event*/
{
    AVD_BRIGHT_ABMNL         = 0x2,       /*�����쳣����*/
    AVD_BRIGHT_NORMAL        = 0x4,       /*��������*/
    AVD_CLARITY_ABMNL        = 0x8,       /*�������쳣����*/
    AVD_CLARITY_NORMAL       = 0x10,      /*����������*/
    AVD_NOISE_ABMNL          = 0x20,      /*�����쳣����*/
    AVD_NOISE_NORMAL         = 0x40,
    AVD_COLOR_ABMNL          = 0x80,      /*ƫɫ�쳣����*/
    AVD_COLOR_NORMAL         = 0x100,
    AVD_SCENE_ABMNL          = 0x200,     /*�����任����*/
    AVD_SCENE_NORMAL         = 0x400,
    AVD_EVENT_ALL            = 0xffffffff,
}JAvdEventType;

//*************************************************************

typedef struct __Time
{
	uint8_t year;					//��1900��ʼ, J_SDK_DEF_BASE_YEAR
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t weekday;				//�����JWeekDay
}JTime;

typedef struct __TimeSeg
{
	uint32_t enable;
	JTime time_start;
	JTime time_end;
}JTimeSeg;
typedef struct __Day
{
	uint32_t count;
	uint32_t day_id;
	uint32_t all_day_enable;				//�Ƿ�ȫ��¼��
	JTimeSeg seg[J_SDK_MAX_SEG_SZIE];
}JDay;
typedef struct __Week
{
	uint32_t count;
	JDay days[J_SDK_MAX_DAY_SIZE];
}JWeek;

typedef struct __Network
{
	uint32_t type;							//����ӿ����ͣ������JNetworkType
	uint32_t dhcp_enable;					//�Ƿ�����DHCP
	uint8_t mac[J_SDK_MAX_MAC_LEN];			//MAC��ַ
	uint8_t ip[J_SDK_MAX_IP_LEN];			//�豸ip
	uint8_t netmask[J_SDK_MAX_NETMASK_LEN];	//��������
	uint8_t gateway[J_SDK_MAX_GATEWAY_LEN];	//����
}JNetwork;

typedef struct __Rectangle
{
	int16_t left;
	int16_t top;
	int16_t right;
	int16_t bottom;
}JRectangle;

typedef struct __Area
{
	uint32_t count;
	JRectangle rect[J_SDK_MAX_DECTION_AREA_SIEZ];
}JArea;

typedef struct __DiskInfo
{
	uint32_t disk_no;						//���̺�
	uint32_t total_size;					//��������
	uint32_t free_size;						//ʣ������
	uint32_t is_backup;						//�Ƿ񱸷���
	uint32_t status;						//����״̬�������JDiskStatus
	uint32_t disk_type;						//�������ͣ������JDiskType
	uint32_t sys_file_type;					//ϵͳ�ļ����ͣ������JSysFileType
}JDiskInfo;

typedef struct __Joint
{
	uint32_t joint_record_enable_channel;
	uint32_t joint_output_enable_channel;
	uint32_t joint_snap_enable_channel;
	uint32_t joint_record_second;
	uint32_t joint_beep_enable;
	uint32_t joint_beep_second;
	uint32_t joint_output_times;
	uint32_t joint_snap_interval;
	uint32_t joint_snap_times;
	uint32_t joint_email_enable;
}JJoint;


typedef struct __Store
{
	uint32_t  rec_type;
	uint32_t  file_size;
	JTime beg_time;
	JTime end_time;
	uint8_t property[J_FILE_PROPERTY_SIZE];
}Store;

typedef struct __Channel
{
	uint8_t pu_id[J_SDK_MAX_ID_LEN];
    uint8_t ip[J_SDK_MAX_IP_LEN];
    uint32_t  port;
    uint32_t media;
    uint32_t level;
    uint32_t channel;
    uint32_t link_num;
    uint32_t protocol;
}Channel;

//#######################################################

typedef struct __ResultCode
{
	uint32_t code;								//0:�ɹ���~0��ʧ��
}JResultCode;


///////////////////////////////////////////////////////////////////////////////
typedef struct __DeviceInfo
{
	uint8_t manu_info[J_SDK_MAX_MANU_INFO_LEN];	//������Ϣ
	uint8_t release_date[J_SDK_MAX_DATA_LEN];	//��������
	uint8_t dev_version[J_SDK_MAX_VERSION_LEN];	//�豸�汾
	uint8_t hw_version[J_SDK_MAX_VERSION_LEN];	//Ӳ���汾
	uint32_t pu_type;							//�豸���ͣ������JPuType
	uint32_t sub_type;							//�豸�Ļ������ͣ�ָ�豸��С�࣬�����JPuSubType
	uint32_t di_num;							//�豸����ӿ���Ŀ
	uint32_t do_num;							//�豸����ӿ���Ŀ
	uint32_t channel_num;						//ͨ����Ŀ
	uint32_t rs485_num;							//RS485�ӿ���Ŀ����0������
	uint32_t rs232_num;							//RS232�ӿ���Ŀ����0������
}JDeviceInfo;

typedef struct __DeviceNTPInfo
{
	uint8_t ntp_server_ip[J_SDK_MAX_IP_LEN];	//NTP������
	uint32_t time_zone;							//ʱ��
	uint32_t time_interval;						//��ʱ���
	uint32_t ntp_enable;						//ntp���أ�1��������0��������
	uint32_t dst_enable;						//����ʱ��1��������0��������
	uint32_t reserve;							//�����ֽ�
}JDeviceNTPInfo;

typedef struct __DeviceTime
{
	JTime time;									//ʱ��
	uint32_t zone;								//ʱ��
	uint32_t sync_enable;						//��ƽ̨������ʱ��ͬ���Ƿ���Ч
}JDeviceTime;
//��չ: ��ʱ����cms ͬ����Ƿ���
typedef struct __DeviceTimeExt
{
	JTime time;									//ʱ��
	uint32_t zone;								//ʱ��
}JDeviceTimeExt;

typedef struct __PlatformInfo
{
	uint8_t pu_id[J_SDK_MAX_ID_LEN];
	uint8_t cms_ip[J_SDK_MAX_IP_LEN];			//ƽ̨cms ip
	uint8_t mds_ip[J_SDK_MAX_IP_LEN];			//ƽ̨mds ip
	uint32_t cms_port;							//ƽ̨cms�˿ں�
	uint32_t mds_port;							//ƽ̨mds�˿ں�
	uint32_t protocol;							//�������Ĵ���Э�飬�����JProtocolType 
	uint32_t is_con_cms;						//�Ƿ�����ƽ̨��1-����  0-������
}JPlatformInfo;

typedef struct __NetworkInfo
{
	JNetwork network[J_SDK_MAX_NETWORK_TYPE];	//����ӿ���Ϣ
	uint8_t main_dns[J_SDK_MAX_DNS_LEN];		//��DNS��������ַ
	uint8_t backup_dns[J_SDK_MAX_DNS_LEN];		//����DNS��������ַ
	uint32_t auto_dns_enable;					//�Ƿ������Զ���ȡdns
	uint32_t cur_network;						//eth0/wifi/3G
	uint32_t cmd_port;							//�豸����˿�
	uint32_t data_port;							//�豸���ݶ˿�
	uint32_t web_port;							//�豸web�˿�
	uint32_t talk_port;                         //�豸�Խ��˿�
}JNetworkInfo;

typedef struct __PPPOEInfo
{
	uint32_t type;								//PPPOE���ŵ�����ӿڣ������JNetworkType
	uint32_t enable;							//
	uint8_t ip[J_SDK_MAX_IP_LEN];				//
	uint8_t account[J_SDK_MAX_ACCOUNT_LEN];		//PPPOE�����˺�
	uint8_t passwd[J_SDK_MAX_PASSWD_LEN];		//PPPOE��������
}JPPPOEInfo;

typedef struct __EncodeParameter
{
	uint32_t level;				//�������ͣ���(0)����(1)����(2)
	uint32_t frame_rate;		//֡��
	uint32_t i_frame_interval;	//I֡���
	uint32_t video_type;		//��Ƶ�������ͣ������JAVCodeType
	uint32_t audio_type; 		//��Ƶ�������ͣ������JAVCodeType
	uint32_t au_in_mode;		//��Ƶ�������ͣ������JAudioInputType
	uint32_t audio_enble;		//��Ƶ�Ƿ��
	uint32_t resolution;		//�ֱ��ʣ������JResolution
	uint32_t qp_value;			//����
	uint32_t code_rate;			//����
	uint32_t frame_priority;	//�Ƿ�֡������
	uint32_t format;			//��ʽ�������JVideoFormat 
	uint32_t bit_rate;			//λ�����ͣ������JBitRate
	uint32_t encode_level;		//�������ͣ������JEncodeLevel
}JEncodeParameter;

typedef struct __DisplayParameter
{
	uint32_t contrast;			//�Աȶ�
	uint32_t bright;			//����
	uint32_t hue;				//�Ҷ�
	uint32_t saturation;		//���Ͷ�
	uint32_t sharpness;			//���
}JDisplayParameter;

typedef struct __RecordParameter
{
	uint32_t level;				//�������ͣ���(0)����(1)����(2)
	uint32_t pre_record;		//ͨ��Ԥ¼ʱ������λ����
	uint32_t auto_cover;		//ͨ��¼���Ƿ��Զ�����

	JWeek week;
}JRecordParameter;

typedef struct __HideParameter
{
	uint32_t hide_enable;		//�Ƿ������ڵ�
	uint32_t hide_color;		//�ڵ�ʱʹ�õ���ɫ
	JArea hide_area;			//�ڵ�����,��ĿΪ0���ڵ���Ч
	uint32_t max_width;			//�����
	uint32_t max_height;		//���߶�
}JHideParameter;

typedef struct __SerialParameter
{
	uint32_t serial_no;			//���ں�
	uint32_t baud_rate;			//������
	uint32_t data_bit;			//����λ 
	uint32_t stop_bit;			//ֹͣλ
	uint32_t verify;			//У��
	uint8_t rs_protocal;		//����Э��: 0ΪRS485��1ΪRS232���˳�Աֻ����������Ч
	uint8_t purpose;			//������;:0 Ĭ�Ϲ��ܣ�485����PTZ���ƣ���1Ϊ���յ㳮���ַ��������ݡ�2Ϊ������ľ����ץ��
	uint8_t res[2];
}JSerialParameter;

typedef struct __OSDExtend
{
	uint32_t ext_text1_enable;					//�Ƿ���ʾ��չ�ı�1��1����ʾ��0������
	uint8_t  ext_text1_data[J_SDK_MAX_TEXT_DATA_LEN];//��չ�ı�1
	uint32_t ext_text1_display_x;				//��չ�ı�1����X
	uint32_t ext_text1_display_y;				//��չ�ı�1����Y
	uint32_t ext_text1_display_w;				//��չ�ı�1���(��ȡ��Ч)
	uint32_t ext_text1_display_h;				//��չ�ı�1�߶�(��ȡ��Ч)
	uint32_t ext_text2_enable;					//�Ƿ���ʾ��չ�ı�2��1����ʾ��0������
	uint8_t  ext_text2_data[J_SDK_MAX_TEXT_DATA_LEN];//��չ�ı�2
	uint32_t ext_text2_display_x;				//��չ�ı�2����X
	uint32_t ext_text2_display_y;				//��չ�ı�2����Y
	uint32_t ext_text2_display_w;				//��չ�ı�2���(��ȡ��Ч)
	uint32_t ext_text2_display_h;				//��չ�ı�2�߶�(��ȡ��Ч)
}JOSDExtend;

typedef struct __OSDParameter
{
	uint32_t time_enable;					//�Ƿ���ʾʱ�䣻1����ʾ��0������
	uint32_t time_display_x;				//ʱ������X
	uint32_t time_display_y;				//ʱ������Y
	uint32_t time_display_color;			//ʱ����ʾ��ɫ
	uint32_t text_enable;					//�Ƿ���ʾ�ı���1����ʾ��0������
	uint32_t text_display_x;				//�ı�����X
	uint32_t text_display_y;				//�ı�����Y
	uint32_t text_display_color;			//�ı���ʾ��ɫ
	uint32_t max_width;						//�����
	uint32_t max_height;					//���߶�
	uint8_t text_data[J_SDK_MAX_TEXT_DATA_LEN];
	uint32_t stream_enable;					//�Ƿ���ʾ������Ϣ��1����ʾ��0������
	
	uint32_t time_display_w;				//ʱ����(��ȡ��Ч)
	uint32_t time_display_h;				//ʱ��߶�(��ȡ��Ч)
	uint32_t text_display_w;				//�ı����(��ȡ��Ч)
	uint32_t text_display_h;				//�ı��߶�(��ȡ��Ч)
	uint32_t osd_invert_color;				//osd ��ɫ��1��������0���ر�
	JOSDExtend ext_osd;
	uint32_t osd_week_day_show;				//�Ƿ���ʾ ����  1:��ʾ��0:����ʾ
}JOSDParameter;

typedef struct __PTZParameter
{
	uint32_t serial_no;						//���ں�
	uint32_t ptz_addr;						//��̨��ַ
	uint32_t protocol;						//0:PelcoD, 1:PelcoP
	uint32_t baud_rate;						//������
	uint32_t data_bit;						//����λ 
	uint32_t stop_bit;						//ֹͣλ
	uint32_t verify;						//У��
}JPTZParameter;

typedef struct __FTPParameter
{
	uint8_t ftp_ip[J_SDK_MAX_IP_LEN];		//ftp������ip
	uint8_t ftp_usr[J_SDK_MAX_ACCOUNT_LEN];	//ftp������port
	uint8_t ftp_pwd[J_SDK_MAX_PASSWD_LEN];	//��½�û���
	uint8_t ftp_path[J_SDK_MAX_PATH_LEN];	//��½����
	uint32_t ftp_port;						//Ftp�ϴ�·��
}JFTPParameter;

typedef struct __SMTPParameter
{
	uint8_t mail_ip[J_SDK_MAX_IP_LEN];		//smtp ��������ַ
	uint8_t mail_addr[J_SDK_MAX_ADDR_LEN];	//�����ʼ���ַ
	uint8_t mail_usr[J_SDK_MAX_ACCOUNT_LEN];//�����ʼ��˺�
	uint8_t mail_pwd[J_SDK_MAX_PASSWD_LEN];	//�����ʼ�����
	uint8_t mail_rctp1[J_SDK_MAX_ADDR_LEN];	//�����ʼ���ַ1
	uint8_t mail_rctp2[J_SDK_MAX_ADDR_LEN];	//�����ʼ���ַ2
	uint8_t mail_rctp3[J_SDK_MAX_ADDR_LEN];	//�����ʼ���ַ3
	uint32_t mail_port;						//smtp �������˿�
	uint32_t ssl_enable;					//�Ƿ�����SSL
}JSMTPParameter;

typedef struct __UPNPParameter
{
	uint8_t  upnp_ip[J_SDK_MAX_IP_LEN];		//upnp ������IP
	uint32_t upnp_enable;					//upnp ����
	uint32_t upnp_eth_no;					//0ʹ�ñ�������ӳ�� 1 ʹ����������ӳ��
	uint32_t upnp_model;					//0�̶�ӳ�� 1�Զ�ӳ��
	uint32_t upnp_refresh_time;				//upnp ˢ��ʱ��(��λСʱ)
	uint32_t upnp_data_port;				//�̶�ӳ��ģʽ��, ���ݶ˿ڵ������˿�
	uint32_t upnp_web_port;					//�̶�ӳ��ģʽ��, Web�˿ڵ������˿�
	uint32_t upnp_data_port_result;			//���ݶ˿ڵ������˿� 0����û��ӳ��򲻳ɹ�
	uint32_t upnp_web_port_result;			//Web�˿ڵ������˿� 0����û��ӳ��򲻳ɹ�
    uint32_t upnp_cmd_port;                 // ����˿ڵ������˿� 
    uint32_t upnp_talk_port;                // �Խ��˿ڵ������˿� 
    uint32_t upnp_cmd_port_result;          //����˿ڵ������˿�ӳ��״̬ 0����û��ӳ��򲻳ɹ�
    uint32_t upnp_talk_port_result;         //�Խ��˿ڵ������˿�ӳ��״̬ 0����û��ӳ��򲻳ɹ�
}JUPNPParameter;

typedef struct __DeviceDiskInfo
{
	uint32_t disk_num;
	JDiskInfo disk[J_SDK_MAX_DISK_NUMBER];
}JDeviceDiskInfo;

typedef struct __FormatDisk
{
	uint32_t disk_no;						//���̺�
	uint32_t progress;						//��ʽ������(0 - 100)
}JFormatDisk;

typedef struct __MoveAlarm
{
	uint32_t move_enable;
	uint32_t sensitive_level;
	uint32_t detect_interval;
	uint32_t max_width;
	uint32_t max_height;
	JArea detect_area;
	JWeek week;
}JMoveAlarm;

typedef struct __LostAlarm
{
	uint32_t lost_enable;
	uint32_t detect_interval;
	JWeek week;
}JLostAlarm;

typedef struct __HideAlarm
{
	uint32_t hide_enable;
	uint32_t detect_interval;
	uint32_t sensitive_level;				//	�������ֶΣ���Χ 0-4��ֵԽ�ͣ�Խ����
	uint32_t max_width;
	uint32_t max_height;
	JArea detect_area;
	JWeek week;
}JHideAlarm;

typedef struct __IoAlarm
{
	uint32_t io_type;						//�����JIOType
	uint32_t alarm_enable;					//
	uint32_t detect_interval;				//
	JWeek week;								//
}JIoAlarm;

typedef enum __JProcessType
{
	JOINT_SNAP_FTP = 0x1,
	JOINT_SNAP_SMTP = 0x2,
}JJProcessType;
typedef struct __JProcess
{
	uint32_t joint_post_process[J_SDK_MAX_CHN_SIZE];	// �����JJProcessType
}JJProcess;

typedef struct __JointAction
{
	uint32_t alarm_type;					//�����JAlarmType
	JJoint joint;							//
	JJProcess process;
}JJointAction;

typedef struct __PTZControl
{
	uint32_t action;						//��̨�����������JPTZAction
	uint32_t param;							//���ֲ�������ֵ
}JPTZControl;

typedef struct __SubmitAlarm
{
	uint8_t data[J_SDK_MAX_ALARM_DATA_LEN];
	uint32_t channel;
	uint32_t alarm_type;					//��� JAlarmType
	uint32_t action_type;	//���alarm_type ΪJ_SDK_AVD_ALARM������JAvdEventType ���尴λ����
							//���alarm_type ΪJ_SDK_NETWORK_FAULT_ALARM������JNetWorkFaultEventType��λ����
							//���alarm_type ΪJ_SDK_IOT_ANALYSE_ALARM������JIotAnalyseEventType��λ����
							//���alarm_type ΪJ_SDK_PEA_OSC_ALARM������JPeaOscEventTypeE��λ����
							//����action_type Ϊ0 : ��ʼ�澯��1: �����澯
	JTime alarm_time;
}JSubmitAlarm;

typedef struct __MediaUrl
{
	uint32_t level;							//��������
	uint32_t media_type;					//��� JMediaType
	uint8_t ip[J_SDK_MAX_IP_LEN];			//
	uint8_t url[J_SDK_MAX_URL_LEN];			//
	uint8_t cu_ip[J_SDK_MAX_IP_LEN];		//�ͻ���IP
}JMediaUrl;

typedef struct __StoreLog
{
	uint32_t rec_type;						//��� JRecodeType
	uint32_t beg_node;
	uint32_t end_node;
	JTime beg_time;
	JTime end_time;
	
	uint32_t node_count;
	uint32_t total_count;
	uint32_t sess_id;
	Store store[J_SDK_MAX_STORE_LOG_SIZE];
}JStoreLog;

typedef struct __UserInfo
{
	uint8_t username[J_SDK_MAX_NAME_LEN];	//�û���
	uint8_t password[J_SDK_MAX_PWD_LEN];	//����	
}JUserInfo;

typedef struct __UserHeart
{
	JTime server_time;
}JUserHeart;

typedef struct __FirmwareUpgrade
{
	uint32_t percent;
	uint32_t data_len;
	uint32_t file_len;
	uint32_t sess_id;
	uint8_t addr[J_SDK_MAX_IP_LEN];
	uint8_t data[J_SDK_MAX_UPGRADE_DATA_LEN];
}JFirmwareUpgrade;

typedef struct __UpgradeProgress
{
	uint32_t percent;
}JUpgradeProgress;

typedef struct __ClientSimulatorConfig
{
    uint32_t count;
    Channel channel[J_MAX_CHANNEL_INFO_SIZE];
}ClientSimulatorConfig;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/***************************** Capability start ********************************/
typedef enum __PlatformType
{
	PF_JXJ		= 0x1,	//���ڼ��Ž�
	PF_ZSYH		= 0x2,	//��ʢ�滪
	PF_HXHT     = 0x4,	//���Ż�ͨƽ̨
	PF_XL_NVR   = 0x8,  //Ѷ��NVR	
	PF_HW		= 0x10, // ��Ϊƽ̨
	PF_P2P		= 0x20,
	PF_MK		= 0x40, // �ֻ�����(mobile kandian)
	PF_ZTE      = 0x80,  // zte 	
	PF_SZY		= 0x0100,	// ����ӥƽ̨
}JPlatformType;

// CpuType
typedef enum __HWEncodeType
{
	TI_DM365 = 0,
	TI_DM368,
	TI_DM8127,
	TI_DM8168,
	
	HI_3507	 = 0x100,
	HI_3510,
	HI_3511,
	HI_3512,
	HI_3515,
	HI_3516,
	HI_3517,
	HI_3518,
	HI_3520,
	HI_3521,
	HI_3531,
	HI_3532,
	HI_3531_3532,
}JHWEncodeType;

// EncodeType
typedef enum __VideoEncodeType
{
	VIDEO_ENCODE_H264_E	  = 0x1,
	VIDEO_ENCODE_MJPEG_E  = 0x2,
	VIDEO_ENCODE_JPEG_E   = 0x4,
	VIDEO_ENCODE_MPEG4_E  = 0x8
}JVideoEncodeType;


// NetSpeed
typedef enum __NetSpeed
{
	NET_SPEED_NONE = 0,
	NET_SPEED_10M,
	NET_SPEED_100M,
	NET_SPEED_1000M,
}JNetSpeed;

typedef enum __NetProtocolType
{
	PROTOCOL_CUSTOM	 = 0x1,
	PROTOCOL_RTSP	 = 0x2,
	PROTOCOL_ONVIF	 = 0x4,
	PROTOCOL_G28181  = 0x8,     //g28181
	PROTOCOL_PISA	 = 0x10,
}JNetProtocolType;

typedef enum __SysCap
{
	DEV_CAP_VIDEO_IN 	= 0x1,				// ��Ƶ����
	DEV_CAP_AUDIO		= 0x2,				// ��Ƶ
	DEV_CAP_IRIS		= 0x4,				// ��Ȧ
	DEV_CAP_PTZ			= 0x8,				// ��̨
	DEV_CAP_ALARM_IN	= 0x10,				// ��������
	DEV_CAP_ALARM_OUT	= 0x20,				// �������
	DEV_CAP_WIFI		= 0x40,				// WIFI
	DEV_CAP_3G			= 0x80,				// 3G
	DEV_CAP_STORAGE		= 0x100,			// �洢
	DEV_CAP_WEB			= 0x200,			// WEB
	DEV_CAP_PLATFORM	= 0x400,			// ƽ̨
	DEV_CAP_INTELLIGENT_ANALYSIS = 0x800,	// ���ܷ���
	DEV_CAP_UPDATE		= 0x1000,			// ����
	DEV_CAP_VIDEO_OUT	= 0x2000,			// ��Ƶ���
	DEV_CAP_IR			= 0x4000,			// ����
	DEV_CAP_NET_STOR	= 0x8000,			// ����洢
	//+++++++++++IPC++++++++++++++//
    DEV_CAP_NETWORK_FAULT = 0x10000,        //������ϼ��
    DEV_CAP_PIR        	= 0x20000,			// ���������
    DEV_CAP_RTSP_SVR_CFG= 0x40000,			// RTSP����
	DEV_CAP_ELEVATOR_CFG= 0x80000,			//¥����ʾ
}JSysCap;

typedef enum __IvsCap
{
	IVS_BRIGHT_ABMNL_CAP        = 0x1,		//�����쳣
    IVS_CLARITY_ABMNL_CAP       = 0x2,		//�������쳣
    IVS_NOISE_ABMNL_CAP         = 0x4,		//�����쳣
	IVS_ABMNL_CAP				= 0x8,		//��ɫ�쳣
    IVS_SCENE_CHANGE_CAP        = 0x10,		//�����任����Ϊ����

    IVS_TRIP_WIRE_CAP           = 0x20,		//������
    IVS_DOUBLE_TRIP_WIRE_CAP    = 0x40,		//˫����
    IVS_PERI_METER_CAP          = 0x80,		//������
    IVS_LOITER_CAP              = 0x100,	//�ǻ����
    IVS_LEFT_CAP                = 0x200,	//��Ʒ����
    IVS_TAKE_CAP                = 0x400,	//��Ʒ��ʧ
	//+++++++++++IPC++++++++++++//
	IVS_HERD_CAP                = 0x800,	//��ҵ����
	IVS_CPC_CAP                 = 0x1000,   //Խ������ͳ��
}JIvsCap;

typedef enum __ImaCap
{
    IMA_BRIGHTNESS 	= 0x00000001,			//����
    IMA_CONTRAST    = 0x00000002,			//�Աȶ�
    IMA_SATURATION 	= 0x00000004,			//���Ͷ�
    IMA_HUE		  	= 0x00000008,			//ɫ��
    IMA_GAMMA		= 0x00000010,			//GAMMAֵ
    IMA_SHARPNESS	= 0x00000020,			//���
                                    
    IMA_MIRROR		= 0X00000040,			//����
    IMA_FLIP		= 0X00000080,			//��ת
    IMA_PWDFREQ	    = 0X00000100,			//����Ƶ��1   
    
	IMA_AWB			= 0x00000200,			//�Զ���ƽ��
	IMA_WDR			= 0x00000400,			//��̬
	IMA_IRIS		= 0x00000800,			//��Ȧ����
	IMA_AEX_CMP     = 0x00001000,			//�عⲹ��
	IMA_AEX_MODE    = 0x00002000,			//�ع�ģʽ
	
	IMA_LDC 	    = 0x00004000,			//����У��
	IMA_ANTI_FOG    = 0x00008000,			//ȥ��
	IMA_RATATE	    = 0x00010000,			//��ת
	IMA_SHUTTER     = 0X00020000,           //���ӿ���
}JImaCap;

typedef enum __ResCap
{
    CAP_VIDEO_QCIF      = 0x1,				//176*144  n��176*120
    CAP_VIDEO_CIF		= 0x2,				//352*288  n��352*240
    CAP_VIDEO_HD1		= 0x4,				//704*288  n��704*240
    CAP_VIDEO_D1		= 0x8,				//704*576  n��704*480
    CAP_VIDEO_QQVGA		= 0x10,				//160*112
    CAP_VIDEO_QVGA		= 0x20,				//320*240
    CAP_VIDEO_VGA		= 0x40,				//640*480
    CAP_VIDEO_SVGA		= 0x80,				//800*600
    CAP_VIDEO_UXGA		= 0x100,			//1600*1200
    CAP_VIDEO_720P		= 0x200,			//1280*720
    CAP_VIDEO_960		= 0x400,			//1280*960
    CAP_VIDEO_1080P		= 0x800,			//1920*1080
    CAP_VIDEO_XGA		= 0x1000, 			//1024*768
	CAP_VIDEO_WXGA		= 0x2000, 			//1280*800
	CAP_VIDEO_SXGA		= 0x4000, 			//1280*1024
	CAP_VIDEO_3Mega		= 0x8000, 			//2048*1536
	CAP_VIDEO_5Mega		= 0x10000, 			//2560*1920
	CAP_VIDEO_UltraHD	= 0x20000, 			//3840*2160 4k ultra hd 
}JResCap;


// ͨ��������
typedef struct __ChnCap
{
	uint32_t	size;						//�ṹ���С
	uint8_t		encode;						//�����JVideoEncodeType	
	uint8_t		supp_mask;					//�Ƿ�֧�������ڵ�	0 ��֧��  1֧��
	uint8_t		mask_count;					//�ڵ��������
	uint8_t		supp_hide_alarm;			//�Ƿ�֧���ڵ�����  
	uint8_t		hide_alarm_count;			//�ڵ��澯����������
	uint8_t		supp_move_alarm;			//�Ƿ�֧���ƶ���ⱨ��
	uint8_t		move_alarm_count;			//�ƶ���ⱨ���������
	uint8_t		supp_video_lost_alarm;		//�Ƿ�֧����Ƶ��ʧ����
	uint8_t		te_reserve[2];				//����
	uint8_t		osd_count;					//OSD����
	uint8_t		stream_count;				//������
	uint32_t	stream_supp_resolution[J_SDK_MAX_STREAM_SIZE];	//�����JResCap
	uint32_t	stream_max_frate_rate[J_SDK_MAX_STREAM_SIZE];	//�������֧��֡��
	uint32_t	img_cap;										//ͼ�������� �������JImaCap
}JChnCap;

typedef struct __DevCap
{
	uint32_t	size;
	uint32_t	cap0;						//�������������JSysCap
	uint32_t	ivs_cap;					//���ܷ����������������JIvsCap
	uint32_t	res_cap;					//
	uint32_t	eth_speed;					//�����JNetSpeed
	uint32_t	hw_encoder_type;			//�����JHWEncodeType
	uint32_t	video_encode_type;			//�����JVideoEncodeType
	uint32_t	platform_type;				//�����JPlatformType
	uint32_t	protocal_type;				//�����JNetProtocolType
	uint32_t	nvr_pu_proto;				//�ɽ����ipc Э������(pu_type = NVR)��JIPCFactoryType ��λȡֵ
	uint32_t	pu_type;					//�����JPuType
	uint32_t	herd_enable;				//��ҵ���
	uint8_t		reserve[16];				//����
	uint8_t		ftp_enable;					//�Ƿ�֧�� ftp��0 ��֧��  1֧��
	uint8_t		upnp_enable;				//�Ƿ�֧�� upnp��0 ��֧��  1֧��
	uint8_t 	eth_num;					//���ڸ���
	uint8_t		chn_num;					//ͨ����
	JChnCap		chn_cap;					//ͨ�������� �������JChnCap
}JDevCap;
/***************************** Capability end ********************************/

typedef enum __ChnType
{
	CH_LOCAL,								//ͨ������: ����
	CH_NETWORK								//ͨ������: ����
}JChnType;
typedef enum __ChnStatusType
{
	CH_FORBID,								//ͨ��״̬: ��ֹ
	CH_NORMAL,								//ͨ��״̬: ����
	CH_CONNECTING,							//ͨ��״̬: ��������
}JChnStatusType;

typedef struct __RemoteChannelInfo
{
	uint32_t	ch_no;							//ͨ���ţ�0-255��
	uint32_t	protocol;						//����Ƶ����Э��
	uint32_t	audio_enable;					//�Ƿ�����Ƶ
	uint8_t		user_name[J_SDK_MAX_NAME_LEN];	//�û�����
	uint8_t		user_pwd[J_SDK_MAX_PWD_LEN];	//�û�����
	uint32_t	win_mode;						//�����������ģʽ: 0-�Զ�, 1-�ֶ�
	uint32_t 	win_max_strm;					//�󴰿����ʱ��Ƶ��������
	uint32_t 	win_min_strm;					//С�������ʱ��Ƶ��������
}JRemoteChannelInfo;

typedef struct __RemoteDeviceInfo
{
	uint8_t 	ip[J_SDK_MAX_IP_LEN];		//IP��ַ
	uint32_t 	port;						//�˿�
	uint32_t 	dev_type;					//�豸����
	uint32_t 	ch_sum;						//��ͨ����
	uint32_t 	dns_enable;					//�Ƿ�ʹ��DNS����
	uint8_t 	dns[J_SDK_MAX_DNS_LEN];		//DNS·��
}JRemoteDeviceInfo;

typedef struct __ChannelInfo
{
	uint32_t ch_no;							//ͨ���ţ�0-255��
	uint32_t ch_type;						//ͨ�����ͣ������JChnType
	uint32_t ch_status;						//ͨ��״̬����ֹ���������������ӣ�
	uint8_t ch_name[J_SDK_MAX_NAME_LEN];	//ͨ������
	JRemoteChannelInfo rmt_ch_info;			//Զ���豸ͨ����Ϣ
	JRemoteDeviceInfo rmt_dev_info;			//Զ���豸��Ϣ
}JChannelInfo;

typedef struct __PictureInfo
{
	uint32_t mirror;						//����(ͼ�����ҵ���) 1 ���� 0 ������
	uint32_t flip;							//��ת(ͼ�����µ���) 1 ��ת 0 ����ת
	uint32_t hz;							//����˸ 0-�ر� 1-50Hz 2-60 Hz 
	uint32_t awb_mode;						//��ƽ��ģʽ 0 �Զ���ƽ�� 1�ֶ���ƽ��
	uint32_t awb_red;						//��ƽ���ɫǿ�� 0-255
	uint32_t awb_blue;						//��ƽ����ɫǿ�� 0-255
	uint32_t wdr;							//��̬0 �ر� 1-255Ϊ��̬ǿ��
	uint32_t iris_type;						//��Ȧ���� 0 �ֶ���Ȧ 1 �Զ���Ȧ
	uint32_t exp_compensation;				//�عⲹ�� 0-255
	uint32_t ae_mode;	 					//�Զ��ع�ģʽ��0-2����0 ��������	  1 ֡������	2 ��������
	uint32_t ldc_value;						//����У��ǿ��(0-255)��0 ������    1-255 ����У��ǿ��ֵ
	uint32_t anti_fog;	 					//ȥ��0 ������    1 ����
	uint32_t rotate;	 					//��ת��0 ��    1 ��ת90��    2 ��ת270�� 
	uint32_t shutter;						//���ӿ��� 0-9  0���Զ����� 1��1/25S�� 2��1/50S�� 3��1/100S�� 4��1/500S�� 5��1/1000S�� 6��1/2000S�� 7��1/5000S�� 8��1/10000S, 9: 1/200s
}JPictureInfo;

typedef struct __WifiConfig
{
	uint8_t essid[J_SDK_MAX_ID_LEN];		//���������
	uint8_t pwd[J_SDK_MAX_PWD_LEN];			//����
	uint32_t wifi_enable;					//0���Ͽ���1������
	uint32_t encrypt_type;					//��������0:NONE  1:WPA  2:WPA2 3:WEP
	uint32_t auth_mode;						//��֤��ʽ0:NONE  1:EAP 2:PSK 3:OPEN 4:SHARED 
	uint32_t secret_key_type;				//��Կ����ʽ 0:NONE  1:AES 2:TKIP ֻ��Ӧ�ڼ�������ΪWPA/WPA2�����
	uint32_t wifi_st;						//������������״̬ 0 ������ 1 ��������2 �ɹ�
}JWifiConfig;

typedef struct __WifiApInfo
{
	uint8_t essid[J_SDK_MAX_ID_LEN];		//���������
	uint32_t encrypt_type;					//��������0:NONE  1:WPA  2:WPA2 3:WEP
	uint32_t auth_mode;						//��֤��ʽ0:NONE  1:EAP 2:PSK 3:OPEN 4:SHARED 
	uint32_t secret_key_type;				//��Կ����ʽ 0:NONE  1:AES 2:TKIP ֻ��Ӧ�ڼ�������ΪWPA/WPA2�����
	uint32_t quality;						//�ź����� 0-100
	uint32_t bit_rate;						//��������
}JWifiApInfo;

typedef struct __WifiSearchRes
{
	uint32_t count;							//��������wifi ����
	JWifiApInfo wifi_ap[J_SDK_MAX_WIFI_AP_SIZE];//
}JWifiSearchRes;

typedef struct __NetworkStatus
{
	uint32_t eth_st;						//������������״̬ 0 ������ 1�������� 2 �ɹ�
	uint32_t wifi_st;						//������������״̬ 0 ������ 1 �������� 2 �ɹ�
	uint32_t pppoe_st;						//pppoe����״̬ 0 ������ 1�������� 2 �ɹ�
}JNetworkStatus;

typedef enum __DevCntlCmd
{
	SHUTDOWN_DEVICE  = 0x01,				//�豸�ػ�
	RESTART_DEVICE   = 0x02,				//�豸����
	RESTORE_DEFAULT  = 0x03,				//�ָ�����Ĭ��
	DETECT_DEAL_PIX  = 0x04,				//������
	DETECT_IRIS      = 0x05,				//��ȦУ��
}JDevCntlCmd;

typedef enum __DetectCmd
{
	DETECT_DEAL_PIX_START   = 0x10,			//�����⿪ʼ
	DETECT_DEAL_PIX_END     = 0x11,			//���������(�ɹ�)
	DETECT_DEAL_PIX_FAIL    = 0x12,			//������ʧ��
	DETECT_DEAL_PIX_TIMEOUT = 0x13,			//�����ⳬʱ
}DetectCmd;

typedef enum __RegulateCmd
{
	REGULATE_IRIS_START   = 0x20,			//��ȦУ����ʼ
	REGULATE_IRIS_END	  = 0x21,			//��ȦУ������(�ɹ�)
	REGULATE_IRIS_FAIL    = 0x22,			//��ȦУ��ʧ��
	REGULATE_IRIS_TIMEOUT = 0x23,			//��ȦУ����ʱ
}JRegulateCmd;

typedef struct __ControlDevice
{
	uint32_t command;						//������������
}JControlDevice;

typedef struct __SubmitDeviceStatus
{
	uint32_t ddp_status;					//������״̬
}JSubmitDeviceStatus;

typedef struct __UserConfig
{
	uint8_t username[J_SDK_MAX_NAME_LEN];	//�û���
	uint8_t password[J_SDK_MAX_PWD_LEN];	//����
	uint32_t local_right;					//���ز���Ȩ��
	uint32_t remote_right;					//Զ�̲���Ȩ��
	uint32_t reserve;
}JUserConfig;

typedef struct __RegionConfig
{
	uint32_t enable;						//1:����	0: ������
	uint32_t abs_qp;						//0 ������� 1 ��������
	uint32_t qp_val;						//����ֵ 0-51
	uint32_t max_width;						//�����
	uint32_t max_height;					//���߶�
	JArea region;							//����
}JRegionConfig;//roi

typedef struct __DdnsConfig
{
	uint8_t ddns_account[J_SDK_MAX_NAME_LEN];	//DDNS ע����
	uint8_t ddns_usr[J_SDK_MAX_NAME_LEN];		//�û�����
	uint8_t ddns_pwd[J_SDK_MAX_PWD_LEN];		//�û�����
	uint32_t ddns_open;							//DDNS����
	uint32_t ddns_type;							//DDNS���ͣ�0-dyndns 1-3322 2 withcctv.com��
	uint32_t ddns_port;							//DDNS�������˿�
	uint32_t ddns_times;						//����ʱ��
	int32_t check_account;						// Ŀǰ����ddns_type = withcctv.com ������, ��ȡ������Ч�����ע�����������Ƿ�ռ�ã� ����������Ч
												//  ��ȡ����ʱ���ͻ���--> �豸check_account = 0 ��ȡ�豸DDNS ���ò������豸���ֶ�Ҳ��Ӧ0
												// �ͻ���--> �豸ddns_type = withcctv.com && check_account = 1 , ���withcctv.com ע�����������Ƿ�ռ��, 
												// �豸--> �ͻ��˻�Ӧcheck_account ֵ����:0  �����ɱ�ӳ�䣬1 �����ѱ�ռ�ã�-1 ���ʧ��(�豸δ�������¼�ⳬʱ�����)
}JDdnsConfig;

typedef struct __HxhtPfConfig
{
	uint8_t  dev_id[J_SDK_MAX_ID_LEN/*32*/];	//�豸ID
	uint8_t  svr_ip[J_SDK_MAX_ID_LEN/*32*/];	//�����������ַ
	uint16_t svr_port;							//����������˿�
	
	uint16_t  video_port;						//������Ƶ�˿�
	uint16_t  audio_port;						//������Ƶ�˿�
	uint16_t  msg_port;							//������Ϣ�˿�
	uint16_t  pb_port;							//���ػطŶ˿�
	
	uint8_t  max_video_conn;					//�����Ƶ������
	uint8_t  pic_upload;						//������Ƭ�ϴ�����

	uint8_t  dev_after_nat;						//0ֱ������ 1ת������(�Ƿ���NAT ���棬�ϱ�0 ����Ƶ������ֱ��������ת�����ȣ�Ĭ���߷ַ�������)
	uint8_t  port_map;							//�Զ�ӳ�����˿� (Ϊ֧���豸����ʱֱ��������Ƶ\�Խ�\�ط�)
	uint8_t  video_trans_mode; 					// ��Ƶ���ģʽ:1 �����ֻ����ģʽ��0 һ��ģʽ,Ĭ��Ϊ0
	uint8_t  res1;
	
	uint32_t upnp_ip;							//upnp ��������ַ (�����ֽ���)
	uint8_t  res2[28];
}JHxhtPfConfig;

typedef struct __UserModConfig
{
	uint8_t old_usrname[J_SDK_MAX_NAME_LEN];	//���û���
	uint8_t old_password[J_SDK_MAX_PWD_LEN];	//���û���
	uint8_t new_username[J_SDK_MAX_NAME_LEN];	//���û���
	uint8_t new_password[J_SDK_MAX_PWD_LEN];	//������
	uint32_t local_right;						//���ز���Ȩ��
	uint32_t remote_right;						//Զ�̲���Ȩ��
	uint32_t reserve;
}JUserModConfig;

typedef struct __Segment
{
	uint32_t 	open     :8;					//����
	uint32_t 	begin_sec:24;					//��ʼʱ������ 3600*hour+60*min+sec
	
	uint32_t 	reserve:8;						//����
	uint32_t 	end_sec:24;						//����ʱ������ 3600*hour+60*min+sec
}JSegment;

typedef struct __AvdRule
{
    uint8_t		enable;          				//0 ����� 1 ���
    uint8_t		level;            				//������0-4 ֵԽ��������Խ��
    uint8_t		alarm_times;        			// �쳣����ʱ�䣬��0-20�룩����ֵԽ����Ҫ�ı���ʱ��Խ��( Ԥ���������ݲ�֧��)
    uint8_t		reserve[29];          
}JAvdRule;

typedef struct __AvdConfig
{
    uint32_t	enable;                			//avd ʹ��
    JSegment	sched_time[J_SDK_MAX_SEG_SZIE]; //ʱ���
    JAvdRule	avd_rule[MAX_IVS_AVD_RULES];  	//��Ƶ��Ϲ���index �±�ȡֵ�� JAvdType
}JAvdConfig;

typedef struct __GB28181Proto
{
	uint8_t device_id[J_SDK_MAX_ID_LEN];		// �豸ID
	uint8_t device_pwd[J_SDK_MAX_PASSWD_LEN];	// �豸���� 
	uint8_t server_id[J_SDK_MAX_ID_LEN];		// ������ID
	uint8_t server_ip[J_SDK_MAX_IP_LEN];		// ��������ַ
	uint16_t server_port;						// �������˿�
	uint16_t msg_port;							// ������Ϣ�˿�    
	uint8_t res[32];
}JGB28181Proto;

typedef struct __GB28181Proto_nvr_chann_cfg
{
	uint8_t channel_id[J_SDK_MAX_CHN_SIZE][J_SDK_MAX_ID_LEN];		
	uint8_t res[32][32];
}JGB28181Proto_nvr_chann_cfg;


typedef struct __IscsiDiskCfg
{
	uint16_t disk_port;							//������̷���˿ڣ�Ĭ��ֵΪ3260 
	uint16_t res1;								// Ԥ��
	uint8_t  cli_name[MAX_NET_DISK_NAME_LEN];	// ���������� [ ipsan ������SAN�ͻ���Ӧ��Initiator���ƣ��豸����iscsi ����ʹ��]   ---����������
	uint8_t  tg_name[MAX_NET_DISK_NAME_LEN];	// �������Target���� (Ŀ������������ RFC 3720 �Ĺ涨��iscsi �ڵ��������󳤶Ȳ��ܳ��� 223 bytes)
	uint32_t discovey_auth;						// discovey  Targetʱ�Ƿ�������֤��0: NONE, ����Ҫ��֤ 1. CHAP ��֤   ---����������
	uint8_t  dis_chap_usr[MAX_NET_DISK_NAME_LEN];// discovey  ʹ�õ�CHAP��֤�û���   ---����������
	uint8_t  dis_chap_pwd[J_SDK_MAX_PWD_LEN];	// discovey  CHAP ��֤����(ʵ��Ϊ 12-16 �ֽ��ַ���)   ---����������
	uint32_t tg_chap_auth;						// Target ������֤��ʽ 0: NONE, ����Ҫ��֤1. CHAP������ʱ����ʹ�� CHAP ��֤ 
	uint8_t  tg_chap_usr[MAX_NET_DISK_NAME_LEN];// ��½Target CHAP ��֤�û���
	uint8_t  tg_chap_pwd[J_SDK_MAX_PWD_LEN];	// ��½Target CHAP ��֤����(ʵ��Ϊ 12-16 �ֽ��ַ���)
	uint8_t  res2[32];
}JIscsiDiskCfg;

typedef struct __NetworkDisk
{
	uint32_t enable;							// �Ƿ�����������
	uint32_t proto;								// ������Э������ (Ŀǰ��֧��iscsi��ֵΪ0�����������)
	uint8_t  disk_ip[J_SDK_MAX_IP_LEN];			// �������Ip��ַ
	JIscsiDiskCfg iscsi_disk;					// iscsi ��������
}JNetworkDisk;

typedef struct __DistState
{
	uint32_t volume;							//Ӳ������  M
	uint32_t free_space;						//Ӳ��ʣ��ռ� M
	uint32_t hd_static;							//Ӳ��״̬ 0������ʹ�ã�1��δ���أ�2���ѹ���δʹ�� 
}JDistState;

typedef struct __ChnState
{
	uint8_t  record_static;						//ͨ���Ƿ���¼��0����¼��1��¼�� 
	uint8_t  signal_static;						//���ӵ��ź�״̬��0��������  1���źŶ�ʧ  
	uint8_t  hardware_static;					//ͨ��Ӳ��״̬��  0��������  1���쳣������DSP�쳣�� 
	uint8_t  res1;
	uint32_t bit_rate[J_SDK_MAX_STREAM_SIZE];	//ʵ������ 
	uint8_t  res[12];
}JChnState;

typedef struct __DevWorkState
{  
	uint32_t   device_static;								//dwDeviceStatic �豸��״̬��0��������1��CPUռ����̫�ߣ�����85%��
	JDistState hd_static[J_SDK_MAX_DISK_NUMBER];			//Ӳ��״̬
	JChnState  chn_static[J_SDK_MAX_CHN_SIZE];				//ͨ��״̬ 
	uint8_t    alarm_in_static[J_SDK_MAX_ALARMIN_SIZE];		//��������ڵ�״̬��0-û�б�����1-�б���
	uint8_t    alarm_out_static[J_SDK_MAX_ALARMOUT_SIZE];	//��������ڵ�״̬��0-û�������1-�б������ 
	uint32_t   local_display;								//������ʾ״̬��0-������1-������
	uint8_t    audio_chn_status[J_SDK_MAX_CHN_SIZE];		//��ʾ����ͨ����״̬��0-δʹ�ã�1-ʹ���У�0xff��Ч 
	uint8_t    res[12];
}JDevWorkState;


//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

typedef struct __BinaryStream
{
	uint32_t type;							//��������������
	uint32_t flag;							//���ı�־
	uint32_t seq;							//�������
	uint32_t size;							//�����ṹ��� size
	uint8_t data[0];						//���Ӷ�����������ʼλ��
}JBinaryStream;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#ifdef _USE_DECODER_PROTO_

#define J_SDK_MAX_MODE_SIZE					32
#define J_SDK_MAX_DIVISION_SIZE				64
#define J_SDK_MAX_DESCRIBE_SIZE				256


typedef struct __DivMode
{
	uint32_t div_id;
	uint8_t mode_name[J_SDK_MAX_NAME_LEN];
	uint8_t description[J_SDK_MAX_DESCRIBE_SIZE];
}JDivMode;

typedef struct __DivisionMode
{
	uint32_t page_size;						//�������
	uint32_t start_row;						//��ʼ��ѯ������
	
	uint32_t total;							//
	uint32_t count;							//
	JDivMode mode[J_SDK_MAX_MODE_SIZE];		//
}JDivisionMode;

typedef struct __DivisionInfo
{
	uint32_t div_no;						//�ָ����
	uint32_t action;						//��������: 1-STEP, 2-TOUR, 3-GROUP
	uint32_t result;						//0:�ɹ�, !0:ʧ��
	uint8_t encoder[J_SDK_MAX_NAME_LEN];	//����������
	uint8_t enc_chn;						//������ͨ��
	uint8_t level;							//��������
	uint8_t url[J_SDK_MAX_URL_LEN];			//������URL��ΪNULL���ڵ�ǰ�ָ����ʾ������
	uint32_t flag;							//��ָ��־��0��������ָ�1������ָ
}JDivisionInfo;

typedef struct __FullScreenMode
{
	uint8_t encoder[J_SDK_MAX_NAME_LEN];	//����������
	uint8_t enc_chn;						//������ͨ��
	uint8_t level;							//��������
	uint32_t action;						//��������: 1-STEP, 2-TOUR, 3-GROUP
	uint32_t result;						//0:�ɹ�, !0:ʧ��
}JFullScreenMode;

typedef struct __ScreenState
{
	uint32_t div_id;						//����ģʽID
	uint32_t count;							//�ָ����
	JDivisionInfo division[J_SDK_MAX_DIVISION_SIZE];
	uint32_t fs_mode;						//�Ƿ�ȫ��
	JFullScreenMode full_screen;
}JScreenState;

typedef struct __ChangeDMode
{
	uint32_t div_id;						//����ģʽID
	uint32_t dis_chn;						//��ʾͨ����
}JChangeDMode;

typedef struct __Screen
{
	uint32_t div_no;						//�ָ����
	uint32_t dis_chn;						//��ʾͨ����
}JScreen;

typedef struct __TVWallPlay
{
	uint32_t action;						//��������: 0-STEP, 1-TOUR, 2-GROUP
	uint8_t name[J_SDK_MAX_NAME_LEN];		//Ѳ�ػ�Ⱥ������ƣ�����û�����ƣ�
	uint32_t step;							//����ţ�������1��
	uint32_t dis_chn;						//��ʾͨ�����
	uint32_t div_id;						//����ģʽID
	uint32_t k_other;						//�Ƿ񱣳������ָ������״̬, 0-������, ����, 1-����
	uint32_t count;							//�����·��ı���������
	JDivisionInfo division[J_SDK_MAX_DIVISION_SIZE];
}JTVWallPlay;

# endif //_USE_DECODER_PROTO_

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define J_SDK_MAX_DEV_LOG_SIZE 32

//��־��������
typedef enum __SDK_LOG_OP
{
	J_SDK_LOG_QUERY = 0x01,  // ��ѯ sdk_log_item_t, sdk_log_cond_t
	J_SDK_LOG_EMPTY,         // ���(ȫ��)
	J_SDK_LOG_BACKUP,        // ����
	J_SDK_LOG_DEL,           // ɾ��(��ʱ���)
	J_SDK_LOG_RESUME,        // �ָ�
}JSDK_LOG_OP;


//��־���Ͷ��壨�����ͣ����֧��32�֣�
typedef enum __LOG_MAJOR_TYPE
{
	J_LOG_MAJOR_SYSTEM 		= 0x00000001,  //ϵͳ��־
	J_LOG_MAJOR_ALARM  		= 0x00000002,  //�澯��־
	J_LOG_MAJOR_OPERATE		= 0x00000004,  //������־
	J_LOG_MAJOR_NETWORK		= 0x00000008,  //������־
	J_LOG_MAJOR_PARAM  		= 0x00000010,  //������־
	J_LOG_MAJOR_EXCEPTION	= 0x00000020,  //�쳣��־
	    
	J_LOG_MAJOR_ALL    		= 0xffffffff,  //������־
}JLOG_MAJOR_TYPE;


//ϵͳ��־������
typedef enum __SYSTEM_MINOR_TYPE
{
	J_SYSTEM_MINOR_STARTUP = 0x0001,  //����
	J_SYSTEM_MINOR_SHUTDOWN,          //�ػ�
	J_SYSTEM_MINOR_REBOOT,			  //����

	J_SYSTEM_MINOR_ALL     = 0xffff   //����ϵͳ��־
}JSYSTEM_MINOR_TYPE;


//�澯��־������
typedef enum __ALARM_MINOR_TYPE
{
	J_ALARM_MINOR_DI_START = 0x0001,   //����澯����
	J_ALARM_MINOR_DI_STOP,             //����澯ֹͣ
	J_ALARM_MINOR_MD_START,            //�ƶ����澯����
	J_ALARM_MINOR_MD_STOP,             //�ƶ����澯ֹͣ
	J_ALARM_MINOR_VL_START,			   //��Ƶ��ʧ�澯����
	J_ALARM_MINOR_VL_STOP,			   //��Ƶ��ʧ�澯ֹͣ
	J_ALARM_MINOR_OD_START,			   //��Ƶ�ڵ���������
	J_ALARM_MINOR_OD_STOP,			   //��Ƶ�ڵ�����ֹͣ
	//+++++++++++++++++IPC+++++++++++++++++//
    J_ALARM_MINOR_NETCARD_DISCONN,     //��������
    J_ALARM_MINOR_NETCARD_RECONN,      //�����ָ�����
	J_ALARM_MINOR_HERD_START,     	   //��ҵ�����澯����
	J_ALARM_MINOR_HERD_STOP,           //��ҵ�����澯ֹͣ
	J_ALARM_MINOR_PIR_START,     	   //�������̽��澯����
	J_ALARM_MINOR_PIR_STOP,            //�������̽��澯ֹͣ

	J_ALARM_MINOR_ALL		= 0xffff   //���и澯��־
}JALARM_MINOR_TYPE;


//������־������
typedef enum __OPERATE_MINOR_TYPE
{
	J_OPERATE_MINOR_LOGIN = 0x0001,		// ��½	
	J_OPERATE_MINOR_LOGOUT, 			// ע��
	J_OPERATE_MINOR_USER_ADD,			// �û�����-����
	J_OPERATE_MINOR_USER_DEL,			// �û�����-ɾ��
	J_OPERATE_MINOR_USER_MODI,			// �û�����-�޸�
	J_OPERATE_MINOR_SETTIME, 			// ����ϵͳʱ��
	J_OPERATE_MINOR_FORMAT_DISK, 		// ��ʽ��Ӳ��
	J_OPERATE_MINOR_DEFAULT, 			// �ָ�Ĭ��
	J_OPERATE_MINOR_UPGRADE, 			// ����
	J_OPERATE_MINOR_PLAYBACK, 			// �ط�
	J_OPERATE_MINOR_PTZ, 				// ��̨����
	J_OPERATE_MINOR_BACKUP, 			// ����
	J_OPERATE_MINOR_RECORD_START, 		// ����¼��
	J_OPERATE_MINOR_RECORD_STOP, 		// ֹͣ¼��
	J_OPERATE_MINOR_CLR_ALARM, 			// �������
	J_OPERATE_MINOR_TALKBACK_START, 	// �Խ���ʼ
	J_OPERATE_MINOR_TALKBACK_STOP, 		// �Խ�����
	J_OPERATE_MINOR_LOG_DEL,			// ɾ����־
	J_OPERATE_MINOR_LOG_EMPTY,			// �����־
	J_OPERATE_MINOR_LOG_BACKUP,			// ������־
	J_OPERATE_MINOR_MANUAL_RECORD_START,// �ֶ�����¼��
	J_OPERATE_MINOR_MANUAL_RECORD_STOP, // �ֶ�ֹͣ¼��	
	J_OPERATE_MINOR_FORMAT_DISK_U, 		// ��ʽ��U��
	J_OPERATE_MINOR_CTRL_ALARM_OUT,		// �ֶ����Ʊ������
	J_OPERATE_MINOR_DETECT_DEAL_PIX,	//������
	J_OPERATE_MINOR_DETECT_IRIS,		//��ȦУ��
	
	J_OPERATE_MINOR_ALL	= 0xffff		// ���в�����־
}JOPERATE_MINOR_TYPE;


//������־������
typedef enum __PARAM_MINOR_TYPE
{
	J_PARAM_MINOR_PARAM_NETWORK = 0x0001, 	// �����������
	J_PARAM_MINOR_PARAM_UART, 				// ���ڲ�������
	J_PARAM_MINOR_PARAM_PTZ, 				// ��̨��������
	J_PARAM_MINOR_PARAM_CHAN, 				// ͨ����������
	J_PARAM_MINOR_PARAM_VENC, 				// �����������
	J_PARAM_MINOR_PARAM_TV, 				// TV��������
	J_PARAM_MINOR_PARAM_PIC_ATTR,			// ͼ����������
	J_PARAM_MINOR_PARAM_RECORD,				// ¼���������
	J_PARAM_MINOR_PARAM_MOTION,				// �ƶ��������
	J_PARAM_MINOR_PARAM_VIDEOLOST,			// ��Ƶ��ʧ����
	J_PARAM_MINOR_PARAM_ALARMIN,			// ����澯����
	J_PARAM_MINOR_PARAM_POLL,				// ��Ѳ����
	J_PARAM_MINOR_PARAM_PREVIEW,			// Ԥ������
	J_PARAM_MINOR_PARAM_OVERLAY,			// �ڵ���������
	J_PARAM_MINOR_PARAM_OSD,				// OSD����
	J_PARAM_MINOR_PARAM_COMM,				// DVR ��������
	J_PARAM_MINOR_PARAM_OD,					// ��Ƶ�ڵ�����
	J_PARAM_MINOR_PARAM_ROI,				// ����Ȥ��������
	J_PARAM_MINOR_PARAM_FTP,				// FTP ����
	J_PARAM_MINOR_PARAM_UPNP,				// UPNP ����
	J_PARAM_MINOR_PARAM_SMTP,				// SMTP ����
	J_PARAM_MINOR_PARAM_NTP,				// NTP  ����
	J_PARAM_MINOR_PARAM_DDNS,				// DDNS ����
	J_PARAM_MINOR_PARAM_ALARMUP,			// �����ϴ�����
	J_PARAM_MINOR_PARAM_NETFAULT,			// ������ϼ������
	J_PARAM_MINOR_PARAM_HERD,	            // ��ҵϵͳ����
	J_PARAM_MINOR_PARAM_PIR,				// �������̽������
	J_PARAM_MINOR_PARAM_RTSP,				// RTSP��������
	J_PARAM_MINOR_PARAM_ELEVATOR,           //����¥������
	J_PRARM_MINOR_ALL	= 0xffff			// ���в�����־
}JPARAM_MINOR_TYPE;


//�쳣��־������
typedef enum __EXCEPTION_MINOR_TYPE
{
	J_EXCEPTION_HD_FULL 		= 0x0001,  	//Ӳ����
	J_EXCEPTION_HD_ERROR,           		//Ӳ�̴���
	J_EXCEPTION_NET_DISCONNECT,				//����Ͽ�
	J_EXCEPTION_IP_CONFLICT,				//IP��ͻ
	
	J_EXCEPTION_MINOR_ALL     = 0xffff   	//�����쳣��־
}JEXCEPTION_MINOR_TYPE;


//��־��¼����
typedef struct _OperationLogItem
{
	uint32_t times;						//Unix epoch 
	uint8_t  major_type;                //����LOG_MAJOR_TYPE_E 
	uint8_t  minor_type;                //0x0000:ֱ����ʾdesc���ݣ�����ͨ��JXXX_MINOR_TYPE_E��������.
	uint8_t  args;                      //�����Ͳ���(args=0ʱ���ý���,�������minor_type�������ݡ�
										//���minor_type��ͨ������أ�args��ʾͨ����;���minor_type�͸澯��أ�args��ʾ�澯�����)
	uint8_t	 res;
	uint8_t  user[J_SDK_MAX_NAME_LEN];  //�����û�
	uint32_t ip;
}JOperationLogItem;

//��־��ѯ����
typedef struct _OperationLogCond
{
	uint32_t beg_time;                   //��ʼʱ��
	uint32_t end_time;                   //����ʱ��
	uint32_t type;                       //���� 
	uint8_t  user[J_SDK_MAX_NAME_LEN];   //�����û� ��ָ��Ϊ��
	uint32_t ip;						 //�û�IP  ��ָ��Ϊ0
	uint32_t beg_node;
	uint32_t end_node;
	uint32_t sess_id;
}JOperationLogCond;

typedef struct _OperationLog
{
	JOperationLogCond cond;						//in 
	uint32_t total_count;						//out
	uint32_t node_count;						//out
	JOperationLogItem item[J_SDK_MAX_DEV_LOG_SIZE];// out
}JOperationLog;

typedef struct __AlarmUploadCfg
{
	uint32_t enable;							//�����ϴ�ʹ�ܿ���
	uint8_t  host[J_SDK_MAX_HOST_LEN];			//��������IP
	uint16_t port;								//���������˿�
	uint16_t type;								//����Э������
}JAlarmUploadCfg;

typedef struct __PresetPoint
{
	uint8_t  name[J_SDK_MAX_NAME_LEN];			//Ԥ�õ�����
	uint32_t preset;							//Ԥ�õ�ţ�
}JPresetPoint;

typedef struct __PPConfig
{
	uint32_t action;							//Ԥ�õ�����������JPTZAction
	JPresetPoint pp;
}JPPConfig;

typedef struct __PPSet
{
	uint32_t pp_count;							//Ԥ�õ�����
	JPresetPoint pp[J_SDK_MAX_PRESET_PORT_SIZE];//Ԥ�õ㼯��
}JPPSet;

typedef struct __CruiseInfo
{
	uint8_t  crz_name[J_SDK_MAX_NAME_LEN];		//Ѳ��·����
	uint32_t crz_no;							//Ѳ��·����
}JCruiseInfo;

typedef struct __CruisePoint
{
    uint8_t preset;								//Ԥ�õ�
    uint8_t speed;								//�ƶ��ٶ�
    uint8_t dwell;								//ͣ��ʱ��
    uint8_t res;								//����
}JCruisePoint;

typedef struct __CruiseWay
{
	JCruiseInfo crz_info;
	uint32_t pp_count;									//Ѳ��������
	JCruisePoint crz_pp[J_SDK_MAX_CRUISE_PORT_SIZE];	//Ѳ���㼯��(·��)
}JCruiseWay;

typedef struct __CruiseConfig
{
	uint32_t action;							//Ѳ��·�������������JPTZAction
	uint32_t crz_no;							//Ѳ��·����
}JCruiseConfig;

typedef struct __CruiseWaySet
{
	uint32_t crz_count;									//Ѳ��·������
	JCruiseInfo crz_info[J_SDK_MAX_CRUISE_WAY_SIZE];	//Ѳ��·������
}JCruiseWaySet;

// ͨ�������Զ��л�����
typedef struct __VencBit
{
	uint8_t enable;				//ʹ�������Զ��л�����
	uint8_t type;				//�л�����,ֵΪ0 �����õ�ʱ������л����ʣ�1 Ϊ��icut��״̬�л�����
	uint8_t sub_type;			//type = 0,��ʱ����л�ʱ��0 Ϊ��ʱ������л��ɵ����ʣ�1 Ϊ��ʱ������л�Ϊ������
								//type = 1, ������ģʽ�л�ʱ��0 ��ʾ�Ǻ���ģʽ����Ϊ������ֵ��1 ��ʾ�Ǻ���ģʽ����Ϊ������ֵ
	uint8_t res;				//Ԥ��
	uint32_t high_er[J_SDK_MAX_STREAM_SIZE];	//�������л�������ֵ��ֵΪ0 ʱ�����л�
	uint32_t low_er[J_SDK_MAX_STREAM_SIZE];		//�������л�������ֵ��ֵΪ0 ʱ�����л�
	JSegment time_seg[J_SDK_MAX_SEG_SZIE];		//�л���ʱ���, type = 0 ʱ��Ч
}JVencBit;

typedef struct __VencBitAutoSwitch
{
	JVencBit venc[J_SDK_MAX_CHN_SIZE];
}JVencBitAutoSwitch;

typedef struct __3DControl
{
	int32_t x_offset;
	int32_t y_offset;
	int32_t amplify;
}J3DControl;

// ir-cut �Զ��л�����
typedef struct __IrcutAutoSwitch
{
    int      sensitive;     //ir switch sensitive 0:low sensitive 1 high sensitive
    uint8_t  res[24];   // Ԥ��
}JIrcutAutoSwitch;

// ir-cut ��ʱ����л�����
typedef struct __IrcutTimerSwitch
{ 
    JSegment seg_time[J_SDK_MAX_SEG_SZIE]; //ʱ���  in segment time open ir light, otherwise close it
    uint8_t  res[12]; // Ԥ��
}JIrcutTimerSwitch;

//============================ ��˼IPC ˽��[start]===========================================
// �ֶ�����ir-cut��/�رգ����ٽ����л�
typedef struct __IrcutManuSwitch
{
    int open;       // 1 open 0 close
    uint8_t res[12];    // Ԥ��
}JIrcutManuSwitch;

// IR-CUT�л�ģʽ����
typedef struct _IrcutCtrl
{
    int switch_type;    //0 auto switch 1 manual switch 2 timer switch
    int auto_c2b;       //0 auto change color 1 fix color 2  fix black white, 0 �Զ���ת��, 1 �̶�Ϊ��ɫ 2 �̶�Ϊ�ڰ�
    uint8_t res[8];
    JIrcutAutoSwitch auto_switch;   // nSwitchType = 0���������ȵȲ����Զ��л�ir cut
    JIrcutManuSwitch manu_switch;   // nSwitchType = 1���ֶ��趨ir cut��/�ر�
    JIrcutTimerSwitch timer_switch; // nSwitchType = 2����ʱ����ir cut��/�ر�
}_JIrcutCtrl;

typedef struct __IrcutCtrl
{
    uint32_t   count;
    _JIrcutCtrl ircut_ctrl[J_SDK_MAX_CHN_SIZE];
}JIrcutCtrl;
//============================ ��˼IPC ˽��[end]===========================================

/* ����IO */
typedef struct __LinkIO
{
	uint32_t time_len;
	uint8_t  data[J_SDK_MAX_ALARM_DATA_LEN];
    uint8_t  res[12];
}JLinkIO;

/* ����Ԥ��λ */
typedef struct __LinkPreset
{
	uint32_t preset;
}JLinkPreset;

/*��������Ļ����(�ֱ���)
0: 	800   X  600
1:	1024 X  768
2:	1152 X  864
3:	1280 X  600
4:	1280 X  720
5:	1280 X  768
6:	1280 X  800
7:	1280 X  960
8:	1280 X  1024
9:	1360 X  768
10:	1366 X  768
11:	1440 X  900
12:	1680 X  900
13:	1680 X  1050
14:	1920 X  1080
15:	1920 X  1200
16:	2560 X  1600
*/
typedef struct __ResolutionInfo
{
    uint32_t resolution;
    uint8_t  res[12];
}JResolutionInfo;

//RTC:round-the-clock
typedef struct __IrcutRTCSwitch
{
    uint32_t rtc;       //0:��ҹ,1:����
    uint8_t  res[12];   // Ԥ��
}JIrcutRTCSwitch;

enum
{
    IRCUT_AUTO = 0,
    IRCUT_RTC  = 1,
    IRCUT_TIMER= 2,
};

typedef struct __Ircut
{
    uint32_t ircut_mode;    //����ģʽ��0:�Զ���1:��ҹ,2:��ʱģʽ
    uint32_t auto_c2b;      //0 auto change color 1 fix color 2  fix black white, 0 �Զ���ת��, 1 �̶�Ϊ��ɫ 2 �̶�Ϊ�ڰ�
    uint8_t  res[12];       // Ԥ��
    union {
        JIrcutAutoSwitch autos;
        JIrcutRTCSwitch  rtcs;
        JIrcutTimerSwitch timers[J_SDK_MAX_DAY_SIZE];
    };
}JIrcut;

typedef struct __IrcutControl
{
    uint32_t count;
    JIrcut   ircut[J_SDK_MAX_CHN_SIZE];
}JIrcutControl;

//�����˿� 
typedef struct __ExtranetPort
{
    uint16_t cmd_port;
    uint16_t data_port;   //Ŀǰ�����ݶ˿ڱ�ʹ��
    uint16_t http_port;
    uint16_t talk_port;
}JExtranetPort;


/*===========================��ҵ��������=======================*/
#define MAX_TROUGH_SIZE             16

typedef struct __Point
{
    uint16_t x;
    uint16_t y;
}JPoint;

typedef struct __QuadrAngle
{
    JPoint angle[4];
}JQuadrAngle;

typedef struct __Trough
{
    uint32_t id;                        //�۶�Ӧ��id, ��Χ0~65535
    uint8_t  name[J_SDK_MAX_NAME_LEN];  //�۵�����
    JQuadrAngle inner_quadr;            //�ڲ��ı���
    JQuadrAngle outer_quadr;			//����ı���
    uint32_t inner_percent;             //�ڲ۲��ϵİٷֱ�
    uint32_t outer_percent;             //��۲��ϵİٷֱ�
}JTrough;

typedef struct __Field
{
    int trough_count;               //��Ч�۸���
    JTrough trough[MAX_TROUGH_SIZE];
}JField;

typedef struct __Herd_Smart_Params_
{
	uint8_t ucEdgeParam; 		// Ĭ��ֵ: 200
	uint8_t ucBinaryParam; 		// Ĭ��ֵ: 80
	uint8_t ucBlurParam; 		// Ĭ��ֵ: 25
	uint8_t ucContourParam; 	// Ĭ��ֵ: 25
	uint8_t ucDilateParam; 		// Ĭ��ֵ: 75
	uint8_t ucErodeParam; 		// Ĭ��ֵ: 180
	uint8_t ucHierarchyParam; 	// Ĭ��ֵ: 35
	uint8_t ucMaskParam; 		// Ĭ��ֵ: 40
}JHerdSmart;

typedef struct __HerdAnalyse
{
    int8_t fodder_eable;            //�Ƿ����ò��Ϸ���, 1: ����, 0������
    int8_t report_intv;             //�ϱ������λ����, 1 ~ 10
    int16_t res;                    //����
    uint32_t max_width;			    //�ο�������
    uint32_t max_height;		    //�ο�����߶�
    JWeek week;                     //����ʱ���
    JField field;                   //������
	JHerdSmart smart; 				// �����궨��������
}JHerdAnalyse;

typedef struct __GrassPercent
{
    JField field;                   //������
}JGrassPercent;

#if 0
// ����������
#define J_SDK_MAX_BALL_ONTIME_NUM 8

// ��������
typedef struct __BallAlarm
{
	uint32_t action;                // ��������
	uint32_t duration_sec;			// ��������ʱ��
	uint8_t res[32];
}JBallAlarm;

// ��ʱ����
typedef struct __BallOntimeAction
{
	uint8_t begin_hour;             // ��ʼСʱ
	uint8_t begin_min;              // ��ʼ����
	uint8_t end_hour;               // ����Сʱ
	uint8_t end_min;                // ��������
	uint32_t action;
	uint8_t res[12];
}JBallOntimeAction;

// ��ʱ����
typedef struct __BallOntimeParam
{
	JBallOntimeAction ontime_action[J_SDK_MAX_BALL_ONTIME_NUM];
	uint8_t res[32];
}JBallOntimeParam;

// Ѳ����
typedef struct __BallCruisePoint
{
	uint8_t preset_enable;		// �Ƿ�ʹ�����Ԥ�õ� 
    uint8_t speed;								//�ƶ��ٶ�
    uint8_t dwell;								//ͣ��ʱ��
	uint8_t res[8];
}JBallCruisePoint;

// Ѳ������
#define J_SDK_MAX_BALL_CRUISE_NUM 16
typedef struct __BallCruiseParam
{
	JBallCruisePoint cruise_point[J_SDK_MAX_BALL_CRUISE_NUM];
	JBallCruisePoint res_cruise_point[8];
	uint8_t res[64];
}JBallCruiseParam;
#endif

//��Ϊ����������
typedef enum __HW_SERVER_AREA
{ 
    J_AREA_AH = 1,   // ����
    J_AREA_BJ = 2,   // ����
    J_AREA_CQ = 3,   // ����
    J_AREA_FJ = 4,   // ����
    J_AREA_GS = 5,   // ����
    J_AREA_GD = 6,   // �㶫
    J_AREA_GX = 7,   // ����
    J_AREA_GZ = 8,   // ����
    J_AREA_HAINAN = 9,   // ����
    J_AREA_HEBEI = 10,  // �ӱ� 
    J_AREA_HENAN = 11,  // ����
    J_AREA_HLJ = 12,  // ������
    J_AREA_HN = 13,  // ����
    J_AREA_HB = 14,  // ���� 
    J_AREA_JL = 15,  // ����
    J_AREA_JS = 16,  // ���� 
    J_AREA_JX = 17,  // ����
    J_AREA_LN = 18,  // ���� 
    J_AREA_NMG = 19,  // ���ɹ�
    J_AREA_NX = 20,  // ����
    J_AREA_QH = 21,  // �ຣ
    J_AREA_SD = 22,  // ɽ��
    J_AREA_SX = 23,  // ɽ�� 
    J_AREA_SHANXI = 24,  // ����
    J_AREA_SH = 25,  // �Ϻ� 
    J_AREA_SC = 26,  // �Ĵ�
    J_AREA_TJ = 27,  // ���
    J_AREA_XZ = 28,  // ����
    J_AREA_XJ = 29,  // �½�
    J_AREA_YN = 30,  // ����
    J_AREA_ZJ = 31,  // �㽭
    J_AREA_HK = 32,  // ���
    J_AREA_MO = 33,  // ����
    J_AREA_TW = 34,  // ̨��
}JHW_SERVER_AREA;

typedef struct __HWPfCfg
{
    uint8_t  device_id[J_SDK_MAX_ID_LEN]; // �豸ID��15λ
    uint8_t  device_pwd[J_SDK_MAX_PASSWD_LEN]; // �豸���� 
    uint16_t msg_port; // ������Ϣ�˿�   
    uint8_t  server_area; // ����������JHW_SERVER_AREA
    uint8_t  res1; // Ԥ��
    uint8_t  server_id[J_SDK_MAX_ID_LEN]; // ������ID��15λ
    uint8_t  server_ip[J_SDK_MAX_IP_LEN]; // ��������ַ 
    uint16_t server_port; // �������˿�
    uint8_t  res2[30]; // Ԥ��
}JHWPfCfg;

typedef struct __ZtePfCfg
{
	uint8_t  device_id[J_SDK_MAX_ID_LEN]; //ע��ID, 18λ
	uint8_t  username[J_SDK_MAX_NAME_LEN];
	uint8_t  password[J_SDK_MAX_PASSWD_LEN];
	uint8_t  server_ip[J_SDK_MAX_IP_LEN];   // ƽ̨ע��IP
	uint16_t server_port;					//ƽ̨ע��˿�
	uint8_t  platformswitch;					//  ƽ̨��������
	uint8_t  reserved; //Ԥ������
		
}JZtePfCfg;

typedef struct _NetWorkFaultCheck
{
    uint8_t   check_enable;     // �Ƿ����������
    uint8_t   res1[3];          // Ԥ���ֶ�
    uint8_t   check_net_cards[J_SDK_MAX_NETCARD_SIZE]; // ����������ã���ʾ�ļ��������������,���������ߡ�wifi��3G�������򣬱����豸����������������ͬʱ����1��wifi��3Gģ�飬��check_net_cards[0]��ʾ��һ������������check_net_cards[1]��ʾ�ڶ�������������check_net_cards[2]��ʾWifi������check_net_cards[3]��ʾ3G����
}JNetWorkFaultCheck;

typedef struct __PirAlarm               //pir alarm
{
	uint32_t	alarm_enable; //
	uint32_t	detect_interval; //
	JWeek 		week; //
}JPirAlarm;

typedef struct __RtspSvrCfg     // rtsp��������
{
	uint8_t   enable; 			// �Ƿ���rtsp����
	uint8_t   res1[3];			// Ԥ���ֶ�
	uint16_t  svr_port; 		// RTSP����˿�
	uint8_t   res2[6];			// Ԥ���ֶ�
}JRtspSvrCfg;

typedef struct __P2PId
{
    char pu_id[J_SDK_MAX_ID_LEN];
    char p2p_id[J_SDK_MAX_ID_LEN];
}JP2PId;

typedef struct __ElevatorCfg     			//����¥������
{
	uint32_t	thetop; 					// ���¥��
	uint32_t	thecurrent;					// ��ǰ¥��
	uint8_t		layername[J_SDK_MAX_LAYER_NUM][20]; 	// ¥�����
	uint32_t	osd_display_x;				//�ı�����X
	uint32_t 	osd_display_y;				//�ı�����Y
	uint32_t 	osd_display_color;			//�ı���ʾ��ɫ
	uint32_t 	osd_display_w;				//�ı����(��ȡ��Ч)
	uint32_t 	osd_display_h;				//�ı��߶�(��ȡ��Ч)
	uint32_t 	max_width; 					//osd �����(�ο����)
	uint32_t 	max_height;					//osd ���߶�(�ο��߶�)
}JElevatorCfg;


typedef struct __MKPfcfg
{
	char device_id[J_SDK_MAX_NAME_LEN];
	char device_key[J_SDK_MAX_NAME_LEN];
	char group_addr[J_SDK_MAX_HOST_LEN];
	char lcms_url[J_SDK_MAX_HOST_LEN];
	char dev_mode[J_SDK_MAX_NAME_LEN];
}MKPfcfg;

// �����ƽ������
typedef struct __SpecialWBConfi
{
	uint8_t		enable;				// ���������ƽ�⿪�أ�0-�����ã�1-����
	uint8_t		res[3];
	uint16_t	curr_color_temp;	// ��ǰɫ��ֵ[0x0, 0xFFFF](ֻ��)
	uint16_t	high_color_temp;	// ��ƽ���ɫ�����ޣ��ο���Χ:[0x0, 0xFFFF]
	uint16_t	low_color_temp;		// ��ƽ���ɫ�����ޣ��ο���Χ:[0x0, 0xFFFF]
	uint16_t	wb_r_gain;			// ��ƽ��Rͨ��У��ϵ��, �ο���Χ:[0x0, 0xFFFF]
	uint16_t	wb_gr_gain;			// ��ƽ��Grͨ��У��ϵ�����ο���Χ:[0x0, 0xFFFF]
	uint16_t	wb_gb_gain;			// ��ƽ��Gbͨ��У��ϵ�����ο���Χ:[0x0, 0xFFFF]
	uint16_t	wb_b_gain;			// ��ƽ��Bͨ��У��ϵ�����ο���Χ:[0x0, 0xFFFF]
}JSpecialWBConfig;

typedef struct __XMcfg
{
	char host_type[J_SDK_MAX_ID_LEN];
	char dev_id[J_SDK_MAX_ID_LEN];
	char vgs_ip[J_SDK_MAX_IP_LEN];
	int32_t vgs_port;
	char user_name[J_SDK_MAX_ACCOUNT_LEN];
	char password[J_SDK_MAX_PASSWD_LEN];
}XMcfg;
//=====================================================================================

typedef enum __ParamId
{
	PARAM_START               = -1,
	PARAM_SYNC_TIME           = 3,			//ͬ��������ʱ��
	PARAM_CHANGE_DISPATCH     = 4,			//������ý�������
	PARAM_DEVICE_INFO         = 5,			//�豸������Ϣ
	PARAM_DEVICE_NTP_INFO     = 6,			//NTP ��Ϣ
	PARAM_DEVICE_TIME         = 7,			//����ʱ����Ϣ(����)-->��չ: 60,61
	PARAM_PLATFORM_INFO       = 8,			//ƽ̨��Ϣ
	PARAM_NETWORK_INFO        = 9,			//������Ϣ
	PARAM_PPPOE_INFOE         = 10,			//PPPOE������Ϣ
	PARAM_ENCODE_INFO         = 11,			//�������
	PARAM_DISPLAY_INFO        = 12,			//��ʾ����
	PARAM_RECORD_INFO         = 13,			//¼�����
	PARAM_HIDE_INFO           = 14,			//�ڵ�����
	PARAM_SERIAL_INFO         = 15,			//���ڲ���
	PARAM_OSD_INFO            = 16,			//OSD����
	PARAM_PTZ_INFO            = 17,			//��̨��Ϣ
	PARAM_FTP_INFO            = 18,			//ftp ����
	PARAM_SMTP_INFO           = 19,			//smtp ����
	PARAM_UPNP_INFO           = 20,			//upnp ����
	PARAM_DISK_INFO           = 21,			//������Ϣ
	PARAM_FORMAT_DISK         = 22,			//��ʽ����
	PARAM_MOVE_ALARM          = 23,			//�ƶ��澯
	PARAM_LOST_ALARM          = 24,			//��ʧ�澯
	PARAM_HIDE_ALARM          = 25,			//�ڵ��澯
	PARAM_IO_ALARM            = 26,			//IO �澯
	PARAM_JOINT_INFO          = 27,			//��������
	PARAM_CONTROL_PTZ         = 28,			//�ƾ�����
	PARAM_SUBMINT_ALARM       = 29,			//�ϱ��澯
	PARAM_MEDIAL_URL          = 30,			//��ý�� URL
	PARAM_STORE_LOG           = 31,			//��Ƶ����
	PARAM_USER_LOGIN          = 32,			//�û���¼����
	PARAM_FIRMWARE_UPGRADE    = 33,			//�̼�����
	PARAM_DEVICE_CAP          = 34,			//�豸������
	PARAM_SEARCH_DEV          = 35,			//�������豸����
	PARAM_CHANNEL_INFO        = 36,			//�豸ͨ������
	PARAM_PIC_INFO            = 37,			//ͼ�����
	PARAM_WIFI_INFO           = 38,			//wifi ����
	PARAM_WIFI_SEARCH         = 39,			//wifi ����
	PARAM_USER_CONFIG         = 40,			//�û���Ϣ����JUserConfig
	PARAM_CONTROL_DEV         = 41,			//�����豸
	PARAM_NETWORK_STATUS      = 42,			//��������״̬
	PARAM_INTEREST_CODING     = 43,			//����Ȥ�������
	PARAM_DDNS_CODING         = 44,			//��̬��������
	
	PARAM_DEF_DISPLAY_INFO    = 45,			//Ĭ����ʾ����
	PARAM_DEF_PICTURE_INFO    = 46,			//Ĭ��ͼ�����
	PARAM_HXHT_CONFIG         = 47,			//���Ż�ͨƽ̨JHxhxPfConfig
	PARAM_MODIFY_USER_CFG     = 48,			//�޸��û���ϢJUserModConfig
	PARAM_AVD_CFG             = 49,			//AVD ����
	PARAM_GB28181_PROTO_CFG   = 50,			//
	PARAM_NETWORK_DISK_CFG    = 51,			//������̹���
	PARAM_DEV_WORK_STATE      = 52,			//�豸״̬��Ϣ

	PARAM_OPERATION_LOG       = 53,			//��ȡ�û�������¼
	PARAM_ALARM_UPLOAD_CFG    = 54,			//�����ϴ�����

	PARAM_PRESET_POINT_CFG    = 55,			//Ԥ�õ�����
	PARAM_CRUISE_POINT_CFG    = 56,			//Ѳ��·��(��ѯѲ��·���ż��ϣ�������ֹͣ��ɾ��ĳһѲ����)
	PARAM_CRUISE_WAY_CFG      = 57,			//Ѳ��·��(��ѯ����ӡ��޸�ĳһѲ��·����Ϣ)
	PARAM_VENC_AUTO_SWITCH    = 58,			//ͨ�������Զ��л�����
	PARAM_3D_CONTROL          = 59,			//3D ����
	
	PARAM_SET_DEVICE_TIME     = 60,			//�ֶ�����ʱ�� JDeviceTimeExt
	PARAM_SYNC_CMS_TIME       = 61,			//����ͬ��cms ʱ����
	PARAM_IR_CUT_CFG          = 62,			//IR-CUT�л�ģʽ����������:JIrcutCtrl(��˼IPC ˽��)
	PARAM_NETWORKFAULTCHECK   = 63,

    //63,64�ѱ�ռ��!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	PARAM_ALARM_LINK_IO       = 65,			//�����豸IO �澯
	PARAM_ALARM_LINK_PRESET   = 66,         //����Ԥ��λ
	PARAM_RESOLUTION_CFG      = 67,			//�������ֱ�����Ϣ
	PARAM_IRCUT_CONTROL       = 68,			//�������:JIrcutControl
	PARAM_3D_GOBACK           = 69,         //3D ���Ʒ���ԭ��
	PARAM_EXTRANET_PORT       = 70,         //�����˿�
    PARAM_HW_PF_CFG			  = 71,			//��Ϊƽ̨    
    PARAM_PIR_ALARM			  = 72,			//�������̽�� JPirAlarm
    PARAM_RTSP_SVR_CFG	  	  = 73,			//RTSP�������� JRtspSvrCfg
	PARAM_P2P_ID              = 74,         //P2P ID
	PARAM_ZTE_PF_CFG          = 75,         //����ƽ̨
	PARAM_GB28181_PROTO_NVR_CH_CFG = 76,    //28181 nvr ͨ��ID JGB28181Proto_nvr_chann_cfg

# ifdef _USE_DECODER_PROTO_
	PARAM_NOTIFY_DIVISION_MODE= 200,		//֪ͨ����ģʽ
	PARAM_GET_SCREEN_STATE    = 201,		//��ȡ��Ļ״̬
	PARAM_CHANGE_DIVISION_MODE= 202,		//�ı����ģʽ
	PARAM_SET_FULL_SCREEN     = 203,		//����ȫ����ʾ
	PARAM_EXIT_FULL_SCREEN    = 204,		//�˳�ȫ����ʾ
	PARAM_TV_WALL_PLAY        = 205,		//����ǽ����
	PARAM_CLEAR_DIVISION      = 206,		//����ָ���ʾ
# endif //_USE_DECODER_PROTO_

	/////////////////ipc start 300////////////////////
	PARAM_PEA_OSC_INFO        = 300,		//�ܽ硢���ߡ���Ʒ����-->JPeaOscCfg
    PARAM_HERD_ANALYSE        = 301,        //��ҵ��������
    PARAM_GRASS_PERCENT       = 302,        //���ϰٷֱ�
	PARAM_CPC_INFO			  = 303,		//Խ������ͳ�� -->JCpcCfg
	PARAM_ELEVATOR_CFG		  = 304,		//����¥������ -->JElevatorCfg
	PARAM_SPECIAL_WB_CFG	  = 305,		//�����ƽ������ HI_DEV_SPE_WB_CFG_S

	PARAM_MK_CFG              = 370,		//mobail kandian
	PARAM_XM_CFG              = 371,		//Ѷ��	

	/////////////////ipc end  ////////////////////
	PARAM_DOME_CTRL 		  = 411,		// �������

	PARAM_MAX_SIZE,
}JParamId;


//******************************************************
#pragma pack(pop)


// **********************���������*******************
typedef enum __JDomeCtrlAction
{
	PTZ_DOME_UP				=	0,	//��
	PTZ_DOME_SENSE			=	1,	//

	PTZ_DOME_DOWN			=	2,	//��
	PTZ_DOME_LEFT			=	3,	//��
	PTZ_DOME_RIGHT			=	4,	//��
	PTZ_DOME_LEFT_UP		=	5,	//����
	PTZ_DOME_LEFT_DOWN		=	6,	//����
	PTZ_DOME_RIGHT_UP		=	7,	//����
	PTZ_DOME_RIGHT_DOWN		=	8,	//����
	PTZ_DOME_AUTOSCAN		=	9,	//�Զ�ɨ��
	PTZ_DOME_MANUALSCAN		=	10,	//�ֶ�ɨ��
	PTZ_DOME_FOCUS_NEAR		=	11, //�۽�+
	PTZ_DOME_FOCUS_FAR		=	12, //�۽�-
	PTZ_DOME_IRIS_CLOSE		=	13, //��Ȧ-
	PTZ_DOME_IRIS_OPEN		=	14,	//��Ȧ+
	PTZ_DOME_ZOOM_WIDE		=	15,	//�䱶+
	PTZ_DOME_ZOOM_TELE		=	16, //�䱶-
	PTZ_DOME_STOP			=	17,	//ֹͣ

	PTZ_DOME_SET_PRESET		=	18, //Ԥ��λ����
	PTZ_DOME_CLR_PRESET		=	19,	//Ԥ��λ���
	PTZ_DOME_GOTO_PRESET		=	20, //Ԥ��λ����
	PTZ_DOME_FLIP			=	21, //��ת
	PTZ_DOME_GOTO_ZEROPAN	=	22, //��λ����
	PTZ_DOME_SET_AUX			=	23, //���ø�������
	PTZ_DOME_CLR_AUX			=	24, //�����������
	PTZ_DOME_REMOTE_RESET	=	25, //Զ�ָ̻�
	PTZ_DOME_ZONE_START		=	26, //���û���ɨ�迪ʼ
	PTZ_DOME_ZONE_END		=	27, //���û���ɨ�����
	PTZ_DOME_WR_CHAR			=	28, //д�ַ�
	PTZ_DOME_CLR_SCR			=	29, //����ַ�
	PTZ_DOME_ALM_ACK			=	30, //����ȷ��
	PTZ_DOME_ZONE_SCAN_ON	=	31, //��������ɨ��
	PTZ_DOME_ZONE_SCAN_OFF	=	32, //ֹͣ����ɨ��
	PTZ_DOME_PTN_START		=	33, //
	PTZ_DOME_PTN_STOP		=	34, //
	PTZ_DOME_RUN_PTN			=	35, //
	PTZ_DOME_ZOOM_SPEED		=	36, //�䱶�ٶ�
	PTZ_DOME_FOCUS_SPEED		=	37, //�۽��ٶ�
	PTZ_DOME_RESET_CAMERA	=	38, //�����λ
	PTZ_DOME_AUTO_FOCUS		=	39, //�Զ��۽�
	PTZ_DOME_AUTO_IRIS		=	40,	//�Զ���Ȧ 
	PTZ_DOME_AUTO_AGC		=	41, //�Զ�����
	PTZ_DOME_BACKLIGHT_COMP	=	42,	//
	PTZ_DOME_AUTO_WB		    =	43,	//�Զ���ƽ��
	PTZ_DOME_DEV_PHASE_DELAY	=	44,	//
	PTZ_DOME_SET_SHTTER_SPEED=	45,	//���ÿ����ٶ�
	PTZ_DOME_ADJ_LL_PHASE_DELAY=	46,
	PTZ_DOME_ADJ_WB_RB		=	47,	//�����Զ���ƽ�����
	PTZ_DOME_ADJ_WB_MG		=	48,	//�����Զ���ƽ��
	PTZ_DOME_ADJ_GAIN		=	49, //�����Զ�����
	PTZ_DOME_AUTO_IRIS_LV	=	50,	//�����Զ���Ȧlevel
	PTZ_DOME_AUTO_IRIS_PEAK	=	51,	//�����Զ���Ȧpeak
	PTZ_DOME_QUERY			=	52, //��ѯ
	
	PTZ_DOME_INVALID			=	53,


	// ********************** dome **************************************
	PTZ_DOME_SET_CRUISE				=	1000,	// ����Ѳ��ɨ��
	PTZ_DOME_SET_CRUISE_START		=	1001,	// �Զ�Ѳ��ɨ�� 
	PTZ_DOME_SET_GUARD_POS			=	1002,	// ���ÿ���λ
	PTZ_DOME_GOTO_GUARD_POS			=	1003,	// ���ÿ���λ
	PTZ_DOME_SET_AUTOSCAN_SETTING	=	1004,	// �����Զ�ɨ��
	PTZ_DOME_SET_AUTOSCAN_START		=	1005,	// �����Զ�ɨ�� 
	PTZ_DOME_SET_ABSCAN_SETTING		=	1006,	// ����AB����ɨ��
	PTZ_DOME_SET_ABSCAN_LEFT_POS		=	1007,	// ����AB��������λ 
	PTZ_DOME_SET_ABSCAN_RIGHT_POS	=	1008,	// ����AB��������λ
	PTZ_DOME_SET_ABSCAN_START		=	1009,	// ����AB����ɨ��
	PTZ_DOME_SET_IDLE_SETTING		=	1010,	// �������ö���
	PTZ_DOME_SET_ONTIME_SETTING		=	1011,	// ���ö�ʱ����
	PTZ_DOME_SET_ALARM_SETTING		=	1012,	// ���ñ�������ʱ�Ķ���
	PTZ_DOME_TRIG_ALARM				=	1013,	// ��������
	PTZ_DOME_CLR_ALARM				=	1014,
	PTZ_DOME_SET_IR_MODE				=	1015,	// ����ƹ���
	PTZ_DOME_SET_DAY_NIGHT_MODE		=	1016,	// ��ҹģʽ
	PTZ_DOME_SET_180_AUTO_FLIP		=	1017,	// 180�Զ���ת
	PTZ_DOME_SET_IRIS_MODE			=	1018,	// ��Ȧ����ģʽ
	PTZ_DOME_SET_BLC_MODE			=	1019,	// ���ⲹ��
	PTZ_DOME_SET_ZOOM_DISPLAY		=	1020,	// ��λ��ʾ
	PTZ_DOME_SET_FRAME_NOISE			=	1021,	// ֡����
	PTZ_DOME_SET_WB_MODE				=	1022,	// ��ƽ��ģʽ
	PTZ_DOME_SET_WDR_MODE			=	1023,	// ��̬
	PTZ_DOME_SET_AUTO_ZOOM			=	1024,	// �Զ��䱶
	PTZ_DOME_SET_3D_CONTROL			=	1025,	// �������3D����
	PTZ_DOME_SET_3D_RECT_CENTER		= 	1026,	// 3D��λ��ѡ����
	PTZ_DOME_SET_3D_POINT_CENTER		=	1027,	// 3D��λ�������
}JDomeCtrlAction;
#pragma pack(push, 2)
typedef struct __JDomeCtrlPtz
{
	uint16_t	u16PtzCmd;		//��̨��������
	union
	{
		struct
		{
			uint8_t	u8Chn;			//��̨ͨ����
			uint8_t	u8Speed;		//�ٶ�
			uint8_t	u8PresetNo;
			uint8_t	u8Res[3];
		};
		struct 
		{
			uint8_t	u8CruisePresetNo;
			uint8_t 	u8CruisePresetEnable;
			uint8_t	u8CruisePresetSec;
			uint8_t	u8CruisePresetSpeed;
		}struCruise;
		struct
		{
			uint8_t 	u8SetVerticalPos;	// �Ƿ����ô�ֱλ��
			uint8_t	u8Speed;
		}struAutoScan;
		struct
		{
			uint8_t 	u8SetVerticalPos;	// �Ƿ����ô�ֱλ��
			uint8_t	u8Speed;
		}struABScan;
		struct
		{
			uint8_t	u8IdleActionIndex;// ���ж���������JDomeAction
			uint8_t	u8IdleSec;
		}struIdleAction;
		struct
		{
			uint8_t	u8OntimeIndex;			// 1-8��ĳ����ʱ��
			uint8_t	u8OntimeActionIndex;	// ��ʱ����������,JDomeAction
			uint8_t	u8BeginHour;
			uint8_t	u8BeginMin;			
			uint8_t	u8EndHour;
			uint8_t	u8EndMin;
		}struOntimeAction;
		struct
		{
			uint8_t	u8AlarmTypeIndex;		// ��һ�ֱ���
			uint8_t	u8AlarmActionIndex;		// ��������������,JDomeAction
			uint8_t 	u8AlarmDurationSec;
		}struAlarm;
		struct
		{
			uint16_t res;
			int32_t s32XCoordinate;		//(x(fact)-x(total))*10000/x(total)
			int32_t s32YCoordinate;		//(y(total)-y(fact))*10000/y(total)
			int32_t s32Amplify;			//��Ϊ�Ŵ󣬸�Ϊ��С(xwide(total)/xwide(fact)>(ywide(total)/ywide(fact)))?xwide(total)/xwide(fact)*10:ywide(total)/ywide(fact)*10
		}stru3DControl;
		struct
		{
			uint8_t	u8Left;
			uint8_t 	u8Top;
			uint8_t	u8Right;
			uint8_t 	u8Bottom;
		}stru3DRectCenter;			//left,top,right,bottomΪ�ٷֱ����꣬��Χ0-100�� �� left>=right ��bottom<=topʱ��Ϊ��ѡ��С�����������Ϊ��ѡ�Ŵ�
		struct
		{
			uint8_t	u8XCoordinate;
			uint8_t 	u8YCoordinate;
		}stru3DPointCenter;					//x,yΪ�ٷֱ����꣬��Χ0-100 
		uint8_t u8OpenOrClose;		// 0:close, 1:open
		uint8_t u8WorkMode;			// 0:�ֶ�,1:�Զ�
		uint8_t u8IRMode;				// 0:Զ�����Զ��л�,1:Զ����ͬʱ����
		uint8_t u8DayNightMode;		// 0:����ģʽ,1:ҹ��ģʽ,2:�Զ�ģʽ
		uint8_t u8WBMode;				// 0 ����,1 ���� ,2 �Զ�,3 �ֶ�
		uint8_t u8Bright;				// ����
		uint8_t u8Contrast;			// �Աȶ�
		uint8_t u8Saturation;			// ���Ͷ�
		uint8_t u8Sharpness;			// ���
		uint8_t u8Red;				// ��ɫ
		uint8_t u8Blue;				// ��ɫ
		uint8_t u8Shutter;			// ���ӿ����ٶ�
	};
}JDomeCtrlPtz;
#pragma pack(pop)
#endif //__J_SDK_H__


