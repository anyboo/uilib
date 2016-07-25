/*
 * Clinet Network SDK Interface.
 * Copyright (c) Shenzhen JXJ Electronic Co. Ltd, 2012
 */

/*
 *  ע��:
 *  1, ���: �����µ� J_MB_ParmId_E ����Ӧ�ṹ��,����Ҫ���±��뱾ģ��;
 *  2, dst_id����: general_dst, ��ʾ�������豸��������;
 *  3, msg_id����: J_MB_ParmId_E ����;
 *  4, cu �ӿ�   : ʹ��mb_cu_notify_t���ز�����, ֧��ͬ�����첽;
 *  5, pu �ӿ�   : ʹ��pu_process_cb_t�ص������豸;
 *
 */


#ifndef __mb_api__
#define __mb_api__

#include "j_sdk.h"


#ifdef __cplusplus
extern "C" {
#endif

/*----------------- comm ----------------*/
#define MB_NAME             "mb_lib"
#define MB_VERSION          "v0.0.6"
#define GROUP_IP            "224.0.0.99"


#define GROUP_PORT          40086
#define general_dst         "general_dst"

/*
 * �豸���Ͷ���
 */
typedef enum _J_DevType {
    /* --------- IPC ---------- */
      J_DEV_IPC_6001        = 0x0001   //3516 ��ͨǹ��
    , J_DEV_IPC_6002        = 0x0002   //3516 ���⻤��
    , J_DEV_IPC_6003        = 0x0003   //3516 �������
    , J_DEV_IPC_6004        = 0x0004   //3516 ��������    
    , J_DEV_IPC_6005        = 0x0005   //3516 ����

	, J_DEV_IPC_8001        = 0x0101   //3518A ��ͨǹ��
	, J_DEV_IPC_8002        = 0x0102   //3518A ���⻤��
	, J_DEV_IPC_8003        = 0x0103   //3518A �������
	, J_DEV_IPC_8004        = 0x0104   //3518A ��������    
	, J_DEV_IPC_8005        = 0x0105   //3518A ����
	, J_DEV_IPC_8101        = 0x0201   //3518C ��ͨǹ��
	, J_DEV_IPC_8102        = 0x0202   //3518C ���⻤��
	, J_DEV_IPC_8103        = 0x0203   //3518C �������
	, J_DEV_IPC_8104        = 0x0204   //3518C ��������    
	, J_DEV_IPC_8105        = 0x0205   //3518C ����
    /* --------- IPNC ---------- */
    , J_Dev_IPNC_           = 0x2000   
    /* --------- DVR ---------- */
    , J_Dev_DVR_2_3520      = 0x4000    //DVR 3520 16-D1
    , J_Dev_DVR_16_3531     = 0x4001    //DVR 3531 16-D1(960)
    , J_Dev_DVR_4_3531      = 0x4002    //DVR 3531 04-HD-SDI
    , J_Dev_DVR_8_3531      = 0x4003    //DVR 3531 08-HD-SDI
    /* --------- NVR 1 ethx ---------- */
    , J_Dev_NVR_1_3520      = 0x5000    //NVR 3520 08-720P
    , J_Dev_NVR_16_3531     = 0x5001    //NVR 3531 16-720P
    , J_Dev_NVR_9_3531      = 0x5002    //NVR 3531 09-1080P
    , J_Dev_NVR_25_3531     = 0x5003    //NVR 3531 25-720P
    , J_Dev_NVR_32_3531     = 0x5004    //NVR 3531 32-720P
    
    /* --------- NVR 2 ethx ---------- */
    , J_Dev_NVR_16_3531_e2  = 0x5011    //NVR 3531 16-720P  2 ethx
    , J_Dev_NVR_9_3531_e2   = 0x5012    //NVR 3531 09-1080P 2 ethx
    , J_Dev_NVR_25_3531_e2  = 0x5013    //NVR 3531 25-720P  2 ethx
    , J_Dev_NVR_32_3531_e2  = 0x5014    //NVR 3531 32-720P  2 ethx
    
    /* --------- NVR for 3521 ---------- */
    , J_Dev_NVR_4_3521      = 0x5020    //NVR 3521 04-1080P
    , J_Dev_NVR_9_3521      = 0x5021    //NVR 3521 09-720P 
    , J_Dev_NVR_16_3521     = 0x5022    //NVR 3521 16-720P 
    
    /* --------- DEC ---------- */    
    , J_Dev_DECC_10_3531    = 0x6000    //DEC CARD 720P 10in, 2out;
    , J_Dev_DECB_13_3531    = 0x6001    //DEC BOX  720P 13in, 2out;
    
}J_DevType_E;
/*-----------------------------------*/

/*
 * �鲥���ò���ID����
 */
typedef enum _J_MB_ParmId {
      J_MB_Device_Id     = 0x0001       /*  �豸 ����&����  J_Device_T    */
    , J_MB_SysCfg_Id                    /*  �豸ϵͳ����    J_SysCfg_T    */
    , J_MB_NetCfg_Id                    /*  �豸��������    JNetworkInfo  */
    , J_MB_DvrCommCfg_Id                /*  DVRͨ������     J_DvrCommCfg_T*/
    , J_MB_ManufCfg_Id                  /*  �豸��������    J_ManufCfg_T  */
    , J_MB_DHCPCtl_Id                   /*  �豸DHCP����    J_DhcpCtl_T   */
    , J_MB_SysStatus_Id                 /*  �豸����״̬     J_SysStatus_T */
    , J_MB_Decoder_Id    = 0x0008       /*  �������豸��Ϣ	J_Decoder_Info */
    , J_MB_Dec_Test_Id                  /*  ������ �������   J_Decoder_Test */

    , J_MB_Jpf_Search_Id = 0x0020       /*  ƽ̨����Jpf_Search*/
    , J_MB_Jpf_Redirect_Id              /*  �ض����豸��½ƽ̨ Jpf_Redirect*/
}J_MB_ParmId_E;


#define J_VER_STR_LEN       32          //�汾��Ϣ����
#define J_SERIAL_NO_LEN     32          //���кų���
#define J_DVR_NAME_LEN      32          //�豸���Ƴ���


typedef struct J_SysCfg_S {

    unsigned char serial_no[J_SERIAL_NO_LEN];       //���к�
    unsigned char device_type[J_VER_STR_LEN];       //�豸�ͺ�
    unsigned char software_ver[J_VER_STR_LEN];      //����汾��
    unsigned char software_date[J_VER_STR_LEN];     //�����������
    unsigned char panel_ver[J_VER_STR_LEN];         //ǰ���汾
    unsigned char hardware_ver[J_VER_STR_LEN];      //Ӳ���汾

    unsigned int  dev_type;                         //�豸����  J_DevType_E
    
    unsigned char ana_chan_num;                     //ģ��ͨ������
    unsigned char ip_chan_num;                      //����ͨ����
    unsigned char dec_chan_num;                     //����·��
    unsigned char stream_num;                       //ͨ��֧�ֵ���������
 
    unsigned char audio_in_num;                     //��������ڵĸ���
    unsigned char audio_out_num;                    //��������ڵĸ���
    unsigned char alarm_in_num;                     //�����������
    unsigned char alarm_out_num;                    //�����������

    unsigned char net_port_num;                     //����ڸ���
    unsigned char rs232_num;                        //232���ڸ���
    unsigned char rs485_num;                        //485���ڸ���
    unsigned char usb_num;                          //USB�ڵĸ���
    
    unsigned char hdmi_num;                         //HDMI�ڵĸ���
    unsigned char vga_num;                          //VGA�ڵĸ���
    unsigned char cvbs_num;                         //cvbs�ڵĸ���
    unsigned char aux_out_num;                      //���ڵĸ���
    
    unsigned char disk_ctrl_num;                    //Ӳ�̿���������
    unsigned char disk_num;                         //Ӳ�̸���
    unsigned char res2[2];                           //����

    unsigned int max_encode_res;                   //������ֱ���
    unsigned int max_display_res;                  //�����ʾ�ֱ���
    
    unsigned int   dvr_bit;                         //DVR����
    unsigned char  dvr_byte[4];                     //DVR����
    
    unsigned int   ipc_ircut:1;                     //IPC �Ƿ�֧��IR_CUT
    unsigned int   ipc_bit:31;                      //IPC ����
    unsigned char  ipc_sensor_type;                 //IPC ͼ�񴫸�������
    unsigned char  ipc_byte[3];                     //IPC ����
}J_SysCfg_T;

/*
 * DVR�������ò���
 */
typedef struct J_DvrCommCfg_S
{
    unsigned char 	dvr_name[J_DVR_NAME_LEN];   //DVR ����
    unsigned int 	dvr_id;                     //DVR ID,(ң����)
    unsigned int 	recycle_record;             //�Ƿ�ѭ��¼��,0:����; 1:��
    unsigned char 	language;                   //����0: ����  1 :Ӣ��
    unsigned char  	video_mode;                 //��Ƶ��ʽ0:PAL  1 NTSC
    unsigned char  	out_device;                 //����豸  0: VGA; 1: HDMI; 2:CVBS
    unsigned char  	resolution;                 //�ֱ���0 :1024 * 768    1 : 1280 * 720   2: 1280*1024  3: 1920*1080
    unsigned char 	standby_time;               //�˵�����ʱ�� 1~60����  0: ��ʾ������
    unsigned char  	boot_guide;                 //������ 0 : ��������  1 : ������
    unsigned char  	resolution2; 				//(����)�ֱ���0 :1024 * 768    1 : 1280 * 720   2: 1280*1024  3: 1920*1080
    unsigned char  	reserve[1];
} J_DvrCommCfg_T;


#define J_MAC_ADDR_LEN        20      //MAC��ַ����

/*
 * ������������
 */
typedef struct J_ManufCfg_S
{
	unsigned char serial_no[J_SERIAL_NO_LEN];       //���к�
	unsigned char device_type[J_VER_STR_LEN];       //�豸�ͺ�
	unsigned int  dev_type;                         //�豸����  J_DevType_E

	unsigned char ana_chan_num;                     //ģ��ͨ������
	unsigned char ip_chan_num;                      //����ͨ����
	unsigned char dec_chan_num;                     //����·��
	unsigned char stream_num;                       //ͨ��֧�ֵ���������

	unsigned char mac[J_MAC_ADDR_LEN];              //MAC��ַ(�ַ���,00:0c:29:00:37:6b)
	unsigned int  platform_type;                    //ƽ̨���ͣ��μ� j_sdk.h => JPlatformType    
	unsigned int  ivs_type;                    		//3516IPC�豸֧�ֵ����ܷ������ͣ����豸��ʹ��
	unsigned int  net_protocol;                    //����Э�飬��j_sdk.h => JNetProtocolType
	unsigned char mac2[J_MAC_ADDR_LEN];            // �豸֧��˫����ʱ,����MAC��ַ2
	unsigned char netport_num;						// ����ڸ���
    unsigned char res2[3+128];
}J_ManufCfg_T;

/* 
 * �豸DHCP����(��������Ч)
 */
typedef struct J_DhcpCtl_S
{
    unsigned short if_type; /* JNetworkType */
    unsigned short dhcp_en; /* ����&�ر�DHCP���� 0���رգ� 1������ */
}J_DhcpCtl_T;


/*
 * �豸����״̬
 */
typedef struct J_SysStatus_S 
{
    unsigned long run_time; //��λs
    unsigned char res[60];
}J_SysStatus_T;
/*
 * �豸��Ϣ����
 */
typedef struct J_Device_S {
    unsigned int    SysSize;
    J_SysCfg_T      SysCfg;
    unsigned int    NetSize;
    JNetworkInfo    NetworkInfo;
}J_Device_T;
typedef J_Device_T j_Device_T;

typedef struct J_Decoder_Info
{
	unsigned long		dwDecoderId;
	unsigned int		nType;
	unsigned char		szName[32];
	unsigned long		lChannelCount;
	unsigned char		szRegPlatformId[32];
	unsigned long		lCmsPort;
	unsigned char		szCmsIp[64];
	/* ------- Ӳ���������� -------- */ 
	JNetwork			NetCfg;		/*��������*/
	unsigned int		dec_no;	/*�������������*/
}J_Decoder_Info;


typedef struct J_Decoder_Test
{
	unsigned int out_en;	/* 0: �ر�������� 1: ��������� */
}J_Decoder_Test;


#define JPF_SEARCH_ID_LEN               32
#define JPF_SEARCH_IP_LEN               16
#define JPF_SEARCH_MNFCT_LEN            12

typedef struct Device_Info
{
    int pu_type;                        //�豸����
    int av_mun;                         //����Ƶ�����
    char mnfct[JPF_SEARCH_MNFCT_LEN];   //�豸����
    char pu_id[JPF_SEARCH_ID_LEN];      //�豸��ʶ
    char dev_ip[JPF_SEARCH_IP_LEN];     //�豸ip
    char res[32];                       //����
}Device_Info;
typedef struct Jpf_Platform
{
    char cms_ip[JPF_SEARCH_IP_LEN];     //ƽ̨cms ip
    char mds_ip[JPF_SEARCH_IP_LEN];     //ƽ̨mds ip
    int cms_port;                       //ƽ̨cms�˿ں�
    int mds_port;                       //ƽ̨mds�˿ں�
    int conn_cms;                       //����ƽ̨���أ�1-����  0-������
    char res[32];                       //����
}Jpf_Platform;
typedef struct Jpf_Search
{
    Device_Info  dev_info;
    Jpf_Platform jpf_plt;
}Jpf_Search;


typedef struct Jpf_Redirect
{
    char pu_id[JPF_SEARCH_ID_LEN];      //�豸��ʶ
    char cms_ip[JPF_SEARCH_IP_LEN];     //ƽ̨cms ip
    int cms_port;                       //ƽ̨cms�˿ں�
    int conn_cms;                       //����ƽ̨���أ�1-����  0-������
}Jpf_Redirect;


/*-----------------------------------*/


/*
 * MB_NOTIFY_ERR_E
 * ������
 * cu����Ҫ���errno��ȷ���Ƿ��pu�˷��ص��Ƿ���  ȷ
 * pu�˻ص�pu_process_cb_t.callback  �ķ���ֵ��ΪMB_NOTIFY_ERR_0 ����MB_NOTIFY_ERR_FAILED
 *
 */


typedef enum _MB_NOTIFY_ERR_E {
      MB_NOTIFY_ERR_0         = 0x0000     //none error;
    , MB_NOTIFY_ERR_TIMEOUT                //timeout;
    , MB_NOTIFY_ERR_FAILED                 //cfg set/get failed;
}MB_NOTIFY_ERR_E;




#define MB_SERIAL_LEN (32)                 //�鲥���кų���
#define MB_USER_LEN   (32)                 //�鲥�û�������
#define MB_PASS_LEN   (32)                 //�鲥���볤��


/*-------------- cu -------------------*/
/*
 * mb_cu_parm_t
 * cu �˻ص�����
 *
 * id:��ϢID
 * error:�豸���ص���Ϣ�����룬MB_NOTIFY_ERR_0����ɹ�
 * size:�豸���صĽṹ���С
 * data:���صĽṹ���ַ
 * dst_id:���ص��鲥����ID��
 */
typedef struct _mb_cu_parm_s {
    int  id;                                //id;
    int  error;                             //error;
    int  size;                              //data size;
    char *data;                             //data;
    char dst_id[MB_SERIAL_LEN];             //dst_id  ���ַ������ִ��ĸ�pu  ���ص���Ϣ
}mb_cu_parm_t;

/* 
  * mb_cu_notify_t 
  * cu �˻ص��ṹ��
  *
  */
typedef struct _mb_cu_notify_s {
    void *user_arg;
    int(*callback)(struct _mb_cu_notify_s *handle
            , mb_cu_parm_t *parm);        
}mb_cu_notify_t;


/*
  *   MB_MSG_DIR_E  ������cu������Ϣ�����û��ǻ�ȡ
  */
typedef enum _MB_MSG_DIR{
      MB_MSG_SET = 0X0001                  //����
    , MB_MSG_GET                           //��ȡ
}MB_MSG_DIR_E;

typedef struct mb_msg_s {
 
    char    src[MB_SERIAL_LEN];     //ԴID
    char    dst[MB_SERIAL_LEN];     //Ŀ��ID
    int     msg_id;                 //����ID
    int     req_no;                 //�������
    int     args;                   //�����û��ǻ�ȡMB_MSG_DIR_E
    int     port;                   //ͨ���ĸ��˿ڷ���ȥ��

    int     error;                  //������

    char    user[MB_USER_LEN];      //�û���cfgʱ�õ�
    char    pass[MB_PASS_LEN];      //����      cfgʱ��

    int     size;
}mb_msg_t;

#ifdef __cplusplus
}
#endif

#endif //__jcu_net_api__
