#pragma once
#include <list>
#include "publiclib_global.h"
#include <QString>

#include "IniEx.h"
#include <QMessageBox>
PUBLICLIB_EXPORT int showMessageBox(QMessageBox::Icon type, QString strMsg);

//��ɫ���Ͷ���
typedef enum _tagRoleTypeEnum
{
	ROLE_TYPE_HOST = 1,  //������
	ROLE_TYPE_DOC,  //ҽ��
	ROLE_TYPE_ASSISTANT, //����
	ROLE_TYPE_TRANSLATION, //����
	ROLE_TYPE_PATIENT = 5  //����
}RoleTypeEnum;

//�������Ͷ���
typedef enum _tagRoomTypeEnum
{
	ROOM_TYPE_1VS1 = 1, //1��1
	ROOM_TYPE_SMALL,     //С���
	ROOM_TYPE_BIG,     //����
	ROOM_TYPE_MAX
}RoomTypeEnum;

typedef enum tag_ENUM_MESSAGE_TYPE
{
	MESSAGE_TYPE_CHAT = 1,
	MESSAGE_TYPE_WHITEBOARD,
	MESSAGE_TYPE_SYSCTL,
	MESSAGE_TYPE_MAX
}ENUM_MESSAGE_TYPE;

typedef struct tag_UserInfo
{
	QString userExID;
	QString userName;
	int userID;
	int userType;
	int utype_;
	bool current;
	tag_UserInfo() {
		userExID = "";
		userName = "";
		userID = 0;
		userType = 0;
		utype_ = 0;
		current = false;
	}
}USERINFO, *PUSERINFO;
typedef std::list<PUSERINFO> PLIST_USERINFO;

//eclassЭ��

typedef struct tag_EclassProtocol
{
	int RoomID;
	int RoomType;
	int VideoProfileType;
	int im_port;
	long Timestamp;

	QString Command;
	QString strExtKey;
	QString IMToken;
	QString RoomExID;	
	QString RoomName;	
	QString Tunnel;
	QString im_ip;
	QString api;
	QString protocol;
	PLIST_USERINFO UserInfo;
	
	tag_EclassProtocol() {
		RoomID = 0;
		RoomType = 0;
		im_port = 0;
		VideoProfileType = -1;
		Timestamp = 0;
		Command = "";
		strExtKey = "";
		IMToken = "";
		RoomExID = "";
		RoomName = "";
		Tunnel = "";
		im_ip = "";
		api = "";
		protocol = "";
	}
}ECLASSPROTOCOL, *PECLASSPROTOCOL;


//������Ϣ
typedef struct tag_MessageInfo
{
	int uid;
	int utype;
	QString strUserName;
	QString strMsg;
	QString strTime;

}MESSAGEINFO, *PMESSAGEINFO;


typedef struct tag_IMServerInfo
{
	int nPort;//�˿ں�
	int nType;//��������
	int nPriority;//���ȼ�
	bool bDomestic;//���ڹ��ڷ��������ǹ��������
	QString strIP;//������IP��ַ
}IMSERVERINFO, *PIM_SERVERINFO;
typedef std::list<PIM_SERVERINFO> PLIST_IMSERVERINFO;



typedef enum tag_ROOMUSERACCESS
{
	RECEVE_SEND = 0,
	RECEVE_ONLY,
	SEND_ONLY,
	NONE
}ROOMUSERACCESS;

typedef enum tag_DEVICE
{
	MIC = 1,
	PLAYOUT,
	CAMERA
}DEVICE;

#define PUBLICLIB_API  PUBLICLIB_EXPORT extern

PUBLICLIB_API QString g_strResPath;
PUBLICLIB_API QString m_glanguage;
PUBLICLIB_API QString m_gRunConfig;

