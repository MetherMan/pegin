/**
 *	$Workfile: dataserver.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 9 $
 *	$Date: 04-07-13 7:26a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/dataserver.h $
 * 
 * 9     04-07-13 7:26a Sk8snow
 * 
 * 8     04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __DATA_SERVER_H__
#define __DATA_SERVER_H__

///////////////////////////////////////////////
// D E F I N E S
///////////////////////////////////////////////
#define dDATA_PACKET_GETSELECTINFO					1		// 전체 케릭 기본 정보 요청 
#define dDATA_PACKET_SENDSELECTINFO				2		// 전체 케릭 기본 정보 전송 
#define dDATA_PACKET_GETCHARINFO					3		// 선택한 플레이 데이터 요청
#define dDATA_PACKET_GETCHARINFO_RESULT				4		// 선택한 플레이 데이터 전송 
#define dDATA_PACKET_NULLCHAR					5		// 케릭터 존재하지 않음
#define dDATA_PACKET_MAKENEWCHAR					6		// 신규 케릭터 생성
#define dDATA_PACKET_MAKENEWCHAR_RESULT				7		// 신규 케릭터 생성 결과
#define dDATA_PACKET_CHAR_LOGOUT					8		// 로그아웃 ( 데이터 저장 )

#define dDATA_PACKET_UPDATELEVEL					9		// 레벨업 정보
#define dDATA_PACKET_UPDATESKILL					10		// 스킬업 정보
#define dDATA_PACKET_UPDATESKILL_EXP				11		// 스킬 경치 정보 

#define dDATA_PACKET_UPDATESTR					12		// STR 업데이트 
#define dDATA_PACKET_UPDATESTR_EXP					13		// STR 경험치 업데이트 
#define dDATA_PACKET_UPDATEINT					14		// INT 업데이트
#define dDATA_PACKET_UPDATEINT_EXP					15		// INT 경험치 업데이트 
#define dDATA_PACKET_UPDATEDEX					16		// DEX 업데이트 
#define dDATA_PACKET_UPDATEDEX_EXP					17		// DEX 경험치 업데이트 

#define dDATA_PACKET_UPDATEHP					18		// HP
#define dDATA_PACKET_UPDATEMP					19		// MP
#define dDATA_PACKET_UPDATEMAPINFO					20		// mapNum, posX, posY
#define dDATA_PACKET_UPDATEMONEY					21		// 소유 금액

#define dDATA_PACKET_DELCHAR					22		// 케릭터 삭제
#define dDATA_PACKET_DELCHAR_RESULT				23		// 케릭터 삭제 결과 

#define dDATA_PACKET_ISCONNECTED					24		// 이미 접속중인 계정

#define dDATA_PACKET_UPDATEEQUIP					25		// 장비

#define dDATA_PACKET_UPDATEMERC					26		// 용병 정보

#define dDATA_PACKET_UPDATECHA					27		// 카리스마

#define dDATA_PACKET_UPDATECOUNTRY					28		// 국가
#define dDATA_PACKET_UPDATEHOMETOWN				29		// 고향마을

#define dDATA_PACKET_UPDATEGUILD					30		// 길드정보
#define dDATA_PACKET_UPDATECOUNTRYLEVEL				31		// 국가 레벨
#define dDATA_PACKET_CRASHGUILD					32		// 길드 아작

#define dDATA_PACKET_UPDATECLASS					33		// 직업 업데이트
#define dDATA_PACKET_UPDATE_CASH					34
#define dDATA_PACKET_SAVE_CHA					35

#define dMAX_DATA_TAG						35
//////////////////////////////////////////////////////////////////////


#define dCHARINFO_EQUIP_NUM						5

#define dCHARINFO_EQUIP_HEAD					0			// 머리
#define dCHARINFO_EQUIP_JACKET					1			// 상의
#define dCHARINFO_EQUIP_PANTS					2			// 하의
#define dCHARINFO_EQUIP_FOOT					3			// 다리 
#define dCHARINFO_EQUIP_HAND					4			// 손

typedef struct 
{
	char		name[dNAME_LEN];				// 케릭터 이름 

	int			skill[dMAX_PLAYER_SKILL];		// 스킬 4개

	BYTE		sex;						// 성별
	WORD		headStyle;					// 머리	스타일
	WORD		faceStyle;					// 얼굴 스타일

	WORD		equip[dCHARINFO_EQUIP_NUM];			// 착용 장비
	BYTE		charClass;					// 직업
	BYTE		charPos;					// 케릭터 번호
	BYTE		guildLevel;					// 길드 등급
} sCHAR_SELECT_INFO, *sPCHAR_SELECT_INFO;


///////////////////////////////////////////////
// V A R I A B L E S 
///////////////////////////////////////////////
extern SOCKET g_DataSock;
extern char g_recvBuff[dMAX_DATA_SOCK_BUFF];			// recv 데이터 버퍼 
extern int g_recvSize;						// recv 버퍼 상의 총 데이터 량
extern int	g_recvPos;						

extern char g_sendBuff[dMAX_DATA_SOCK_BUFF];			// send 대기중인 데이터 
extern int g_sendSize;		

BOOL RecvFromDataServer();
BOOL ReadDataRecvBuff();

void SendToDataServer( char *data , int size );
int CheckCashMoney( char *szID );
void UpdateCashMoney( char *szID );

int FlushDataSendBuff();

void InitDataServer();


#endif



