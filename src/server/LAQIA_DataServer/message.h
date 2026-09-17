#ifndef __MESSAGE_H__
#define __MESSAGE_H__


///////////////////////////////////////////////////////////
#define dDATA_PACKET_RESERVED						0

#define dDATA_PACKET_GETSELECTINFO					1
#define dDATA_PACKET_SENDSELECTINFO					2
#define dDATA_PACKET_GETCHARINFO					3		// 선택한 플레이 데이터 요청
#define dDATA_PACKET_GETCHARINFO_RESULT				4		// 선택한 플레이 데이터 전송 
#define dDATA_PACKET_NULLCHAR						5		// 케릭터 존재하지 않음
#define dDATA_PACKET_MAKENEWCHAR					6		// 신규 케릭터 생성
#define dDATA_PACKET_MAKENEWCHAR_RESULT				7		// 신규 케릭터 생성 결과
#define dDATA_PACKET_CHAR_LOGOUT					8		// 로그아웃 ( 데이터 저장 )

#define dDATA_PACKET_UPDATELEVEL					9		// 레벨업 정보
#define dDATA_PACKET_UPDATESKILL					10		// 스킬업 정보
#define dDATA_PACKET_UPDATESKILL_EXP				11		// 스킬 경치 정보 

#define dDATA_PACKET_UPDATESTR						12		// STR
#define dDATA_PACKET_UPDATESTR_EXP					13		// STR EXP
#define dDATA_PACKET_UPDATEINT						14		// INT
#define dDATA_PACKET_UPDATEINT_EXP					15		// INT EXP
#define dDATA_PACKET_UPDATEDEX						16		// DEX
#define dDATA_PACKET_UPDATEDEX_EXP					17		// DEX EXP

#define dDATA_PACKET_UPDATEHP						18		// HP
#define dDATA_PACKET_UPDATEMP						19		// MP
#define dDATA_PACKET_UPDATEMAPINFO					20		// mapNum, posX, posY
#define dDATA_PACKET_UPDATEMONEY					21		// 소유 금액

#define dDATA_PACKET_DELCHAR						22		// 케릭터 삭제
#define dDATA_PACKET_DELCHAR_RESULT					23		// 케릭터 삭제 결과 

#define dDATA_PACKET_ISCONNECTED					24		// 이미 접속중인 계정

#define dDATA_PACKET_UPDATEEQUIP					25		// 장비

#define dDATA_PACKET_UPDATEMERC						26		// 용병 정보

#define dDATA_PACKET_UPDATECHA						27		// 카리스마

#define dDATA_PACKET_UPDATECOUNTRY					28		// 국가
#define dDATA_PACKET_UPDATEHOMETOWN					29		// 고향마을

#define dDATA_PACKET_UPDATEGUILD					30		// 길드정보
#define dDATA_PACKET_UPDATECOUNTRYLEVEL				31		// 국가 레벨

#define dDATA_PACKET_CRASHGUILD						32		// 길드 아작

#define dDATA_PACKET_UPDATECLASS					33		// 직업 업데이트
#define dDATA_PACKET_UPDATE_CASH					34		// 캐쉬 금액 
#define dDATA_PACKET_SAVE_CHA						35

#define dMAX_TAG									36	
///////////////////////////////////////////////////////////
#define dCHARINFO_EQUIP_NUM						5

#define dCHARINFO_EQUIP_HEAD					0
#define dCHARINFO_EQUIP_JACKET					1
#define dCHARINFO_EQUIP_PANTS					2
#define dCHARINFO_EQUIP_FOOT					3
#define dCHARINFO_EQUIP_HAND					4

typedef struct 
{
	char		name[dNAME_LEN];				// 케릭터 이름 

	int			skill[dMAX_SKILL];				// 스킬 4개

	BYTE		sex;							// 성별
	WORD		headStyle;						// 머리	스타일
	WORD		faceStyle;						// 얼굴 스타일

	WORD		equip[dCHARINFO_EQUIP_NUM];		// 착용 장비
	BYTE		charClass;						// 직업
	BYTE		charPos;						// 케릭터 번호
} sCHAR_SELECT_INFO, *sPCHAR_SELECT_INFO;




////////////////////////////////////////////////////////
// F U N C S
////////////////////////////////////////////////////////
BOOL RecvFromClient( sPDESC_DATA pClient );
BOOL ReadRecvBuff( sPDESC_DATA pClient );
void WriteLoginTime( char *id, BYTE charPos );
void WriteLogOutTime( char *id );
BOOL GetSelectInfo( sPDESC_DATA pClient );
BOOL FindAnotherData( char *where );
BOOL CrashGuild( sPDESC_DATA pClient );

#endif 


