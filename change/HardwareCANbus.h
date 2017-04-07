#ifndef HARDWARECANBUS_H
#define HARDWARECANBUS_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "HardwareAdapter.h"
#include "SoftBusMsg.h"
#include <cstring>

//接口卡类型定义
#define VCI_PCI5121		1
#define VCI_PCI9810		2
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_USBCAN2A	4
#define VCI_PCI9820		5
#define VCI_CAN232		6
#define VCI_PCI5110		7
#define VCI_CANLITE		8
#define VCI_ISA9620		9
#define VCI_ISA5420		10
#define VCI_PC104CAN	11
#define VCI_CANETE		12
#define VCI_DNP9810		13
#define VCI_PCI9840		14
#define VCI_PC104CAN2	15
#define VCI_PCI9820I	16
#define VCI_CANETTCP	17
#define VCI_PEC9920		18
#define VCI_PCI5010U	19
#define VCI_USBCAN_E_U	20
#define VCI_USBCAN_2E_U	21

//CAN错误码
#define	ERR_CAN_OVERFLOW			0x0001	//CAN控制器内部FIFO溢出
#define	ERR_CAN_ERRALARM			0x0002	//CAN控制器错误报警
#define	ERR_CAN_PASSIVE				0x0004	//CAN控制器消极错误
#define	ERR_CAN_LOSE				0x0008	//CAN控制器仲裁丢失
#define	ERR_CAN_BUSERR				0x0010	//CAN控制器总线错误
#define ERR_CAN_BUSOFF				0x0020 //总线关闭错误
//通用错误码
#define	ERR_DEVICEOPENED			0x0100	//设备已经打开
#define	ERR_DEVICEOPEN				0x0200	//打开设备错误
#define	ERR_DEVICENOTOPEN			0x0400	//设备没有打开
#define	ERR_BUFFEROVERFLOW			0x0800	//缓冲区溢出
#define	ERR_DEVICENOTEXIST			0x1000	//此设备不存在
#define	ERR_LOADKERNELDLL			0x2000	//装载动态库失败
#define ERR_CMDFAILED				0x4000	//执行命令失败错误码
#define	ERR_BUFFERCREATE			0x8000	//内存不足


//函数调用返回状态值
#define	STATUS_OK					1
#define STATUS_ERR					0

#define CMD_DESIP			0
#define CMD_DESPORT			1
#define CMD_CHGDESIPANDPORT		2

typedef char CHAR;
typedef void* PVOID;
typedef int INT;

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned long ULONG;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef UCHAR* PUCHAR;
typedef UCHAR BOOL;

#define TRUE 1
#define FALSE 0

//1.ZLGCAN系列接口卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO{
        USHORT	hw_Version;
        USHORT	fw_Version;
        USHORT	dr_Version;
        USHORT	in_Version;
        USHORT	irq_Num;
        BYTE	can_Num;
        CHAR	str_Serial_Num[20];
        CHAR	str_hw_Type[40];
        USHORT	Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO;

//2.定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJ{
    BYTE	ID[4];
    UINT	TimeStamp;
    BYTE	TimeFlag;
    BYTE	SendType;
    BYTE	RemoteFlag;//是否是远程帧
    BYTE	ExternFlag;//是否是扩展帧
    BYTE	DataLen;
    BYTE	Data[8];
    BYTE	Reserved[3];
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.定义CAN控制器状态的数据类型。
typedef struct _VCI_CAN_STATUS{
    UCHAR	ErrInterrupt;
    UCHAR	regMode;
    UCHAR	regStatus;
    UCHAR	regALCapture;
    UCHAR	regECCapture;
    UCHAR	regEWLimit;
    UCHAR	regRECounter;
    UCHAR	regTECounter;
    DWORD	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//4.定义错误信息的数据类型。
typedef struct _VCI_ERR_INFO{
        UINT	ErrCode;
        BYTE	Passive_ErrData[3];
        BYTE	ArLost_ErrData;
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//5.定义初始化CAN的数据类型
typedef struct _VCI_INIT_CONFIG{
    DWORD	AccCode;
    DWORD	AccMask;
    DWORD	Reserved;
    UCHAR	Filter;
    UCHAR	Timing0;
    UCHAR	Timing1;
    UCHAR	Mode;
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

typedef struct _tagChgDesIPAndPort
{
    char szpwd[10];
    char szdesip[20];
    int desport;
}CHGDESIPANDPORT;

///////// new add struct for filter /////////
typedef struct _VCI_FILTER_RECORD{
    DWORD ExtFrame;	//是否为扩展帧
    DWORD Start;
    DWORD End;
}VCI_FILTER_RECORD,*PVCI_FILTER_RECORD;

#ifdef __cplusplus
#define EXTERNC	extern "C"
#else
#define EXTERNC
#endif

EXTERNC DWORD VCI_OpenDevice(DWORD DeviceType,DWORD DeviceInd,DWORD Reserved);
EXTERNC DWORD VCI_CloseDevice(DWORD DeviceType,DWORD DeviceInd);
EXTERNC DWORD VCI_InitCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_INIT_CONFIG pInitConfig);

EXTERNC DWORD VCI_ReadBoardInfo(DWORD DeviceType,DWORD DeviceInd,PVCI_BOARD_INFO pInfo);
EXTERNC DWORD VCI_ReadErrInfo(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_ERR_INFO pErrInfo);
EXTERNC DWORD VCI_ReadCANStatus(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_STATUS pCANStatus);

EXTERNC DWORD VCI_GetReference(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,DWORD RefType,PVOID pData);
EXTERNC DWORD VCI_SetReference(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,DWORD RefType,PVOID pData);

EXTERNC ULONG VCI_GetReceiveNum(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
EXTERNC DWORD VCI_ClearBuffer(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);

EXTERNC DWORD VCI_StartCAN(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
EXTERNC DWORD VCI_ResetCAN(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);

EXTERNC ULONG VCI_Transmit(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pSend,ULONG Len);//发送数据
EXTERNC ULONG VCI_Receive(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pReceive,ULONG Len,INT WaitTime);//接收数据


class  CHardwareCANbus
{
public:
        CHardwareCANbus();
        ~CHardwareCANbus();
public:
        CHardwareAdapter *  m_pHardwareadapter_CAN;

        bool   canbus_initial(CHardwareAdapter * m_phardwareadapter);
        void   canbus_send(CSoftBusMsg *  msg);
};


#endif // HARDWARECANBUS_H
