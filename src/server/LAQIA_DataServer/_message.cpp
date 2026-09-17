/*
	Copyright (c) 2003 팀메이, All rights reserved.

	Author	sk8snow

	File	message.cpp

	Build log...
*/

#include "def.h"

#include "structs.h"

#include "extern.h"
#include "message.h"

#include "db_rows.h"

char g_Packet[dMAX_SOCK_BUFF];
int g_nPos;

void WriteLoginTime( char *id, BYTE charPos );

//
// Function Name: RecvFromClient
// Date: 2002-02-12
// Description: 클라이언트에서 전송한 데이터를 버퍼에 담아온다
//
BOOL RecvFromClient( sPDESC_DATA pClient )
{
	int recvSize;
	char recvBuff[2048];

	recvSize = recv( pClient->sock, recvBuff, 1024, 0 );

	if( recvSize == 0 )
		return 0;

	if( recvSize < 0 )
	{
#if defined(WIN32)
		if( WSAGetLastError() != WSAEWOULDBLOCK )
		{
			log( "RecvFromClient : [%s] WSAGetLastError: %d\r\n", pClient->ip, WSAGetLastError() );
			return 0;
		}
#else
		if( errno != EWOULDBLOCK )
		{
			log( "RecvFromClient : [%s] errno: %d\r\n", pClient->ip, errno );
			return 0;
		}
#endif
		else
			return 1;
	}

	if( ( pClient->recvSize + recvSize ) >= dMAX_SOCK_BUFF )
	{
		log( "%s's recvBuff Overflow\r\n", pClient->ip );
		return 0;
	}
	
	memcpy( &pClient->recvBuff[pClient->recvSize], recvBuff, recvSize );
	pClient->recvSize += recvSize;

	return 1;
}


//
// Function Name: NullFunc
// Date: 2002-02-12
// Description: 
//
BOOL NullFunc( sPDESC_DATA pClient )
{
	return 1;
}


//
// Function Name: GetSelectInfo
// Date:
// Description: 
//
BOOL GetSelectInfo( sPDESC_DATA pClient )
{
	char id[128];

	GetString( pClient->recvBuff, id, pClient->recvPos );

	char query[1024];
	sCHAR_SELECT_INFO charInfo[3];

	int j;

	for( int i = 0; i < 3; i ++ )
	{
		strcpy( charInfo[i].name, "UNUSE" );

		for( j = 0; j < 4; j++ )
			charInfo[i].skill[j] = 0;

		charInfo[i].sex = 0;							// 성별
		charInfo[i].headStyle = 0;						// 머리	스타일
		charInfo[i].faceStyle = 0;						// 얼굴 스타일
	

		for( j = 0; j < dCHARINFO_EQUIP_NUM; j++ )
			charInfo[i].equip[j] = 0;					// 착용 장비
		
		charInfo[i].charClass = 0;						// 직업 
		charInfo[i].charPos = i;						// 케릭터 번호
	}

	
	BYTE isConnected = 0;

	char escID[128];

	mysql_escape_string( escID, id, strlen( id ) );

	sprintf( query, "select * from UserTable where id = '%s' order by charPos limit 3", escID );

	// 
	if( !mysql_query( g_USER_DB, query ) )
	{
		MYSQL_RES *result = mysql_store_result( g_USER_DB );
		
		if( result )
		{
			MYSQL_ROW row;

			int charPos;

			while( row = mysql_fetch_row( result ) )
			{
				charPos = atoi( row[dDB_CHAR_CHARPOS] );

				if( charPos < 0 || charPos > 2 )
					continue;

				strcpy( charInfo[charPos].name, row[dDB_CHAR_NAME] );			// 이름 
				
				charInfo[charPos].skill[0] = atoi( row[dDB_CHAR_SKILL_LEVEL0] );
				charInfo[charPos].skill[1] = atoi( row[dDB_CHAR_SKILL_LEVEL1] );
				charInfo[charPos].skill[2] = atoi( row[dDB_CHAR_SKILL_LEVEL2] );
				charInfo[charPos].skill[3] = atoi( row[dDB_CHAR_SKILL_LEVEL3] );

				charInfo[charPos].sex = atoi( row[dDB_CHAR_SEX] );				// 성별 
				charInfo[charPos].headStyle = atoi( row[dDB_CHAR_HEADSTYLE] );		// 머리 스타일 
				charInfo[charPos].faceStyle = atoi( row[dDB_CHAR_FACESTYLE] );		// 얼굴 스타일 
				
				charInfo[charPos].equip[0] = atoi( row[dDB_CHAR_EQUIPHEAD] );		// 투구 
				charInfo[charPos].equip[1] = atoi( row[dDB_CHAR_EQUIPJACKET] );		// 상의 
				charInfo[charPos].equip[2] = atoi( row[dDB_CHAR_EQUIPPANTS] );		// 하의 
				charInfo[charPos].equip[3] = atoi( row[dDB_CHAR_EQUIPFOOT] );		// 신발 
				charInfo[charPos].equip[4] = atoi( row[dDB_CHAR_EQUIPHAND] );		// 장갑
			
				charInfo[charPos].charClass = atoi( row[dDB_CHAR_CHARCLASS] );		// 직업 

				charInfo[charPos].charPos = charPos;						// 케릭터 순서 번호

				// 최후 로그아웃 시간이 최후 로그인 시간보다 작으면 접속중
/*				if( atol( row[dDB_CHAR_LASTLOGOUT] ) < atol( row[dDB_CHAR_LASTLOGIN] ) )
						isConnected = 1;*/
			}
			mysql_free_result( result );
		}
	}

//	if( !isConnected )
//	{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_SENDSELECTINFO, g_nPos );
	PutString( g_Packet, id, g_nPos );
	memcpy( &g_Packet[g_nPos], &charInfo[0], sizeof( sCHAR_SELECT_INFO ) );
	g_nPos += sizeof( sCHAR_SELECT_INFO );
	memcpy( &g_Packet[g_nPos], &charInfo[1], sizeof( sCHAR_SELECT_INFO ) );
	g_nPos += sizeof( sCHAR_SELECT_INFO );
	memcpy( &g_Packet[g_nPos], &charInfo[2], sizeof( sCHAR_SELECT_INFO ) );
	g_nPos += sizeof( sCHAR_SELECT_INFO );
	PutSize( g_Packet, g_nPos );
	
	SendData( pClient, g_Packet, g_nPos );
/*	}
	else
	{
		g_nPos = 2;

		PutWord( g_Packet, dDATA_PACKET_ISCONNECTED, g_nPos );
		PutString( g_Packet, id, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pClient, g_Packet, g_nPos );
	}*/
	return 1;
}

//
// Function Name: GetDataFromDataList
// Date:
// Description: 
//
sPCHAR_DATA GetDataFromDataList( sPDESC_DATA pClient, char *id )
{
	sPLIST_BLOCK block;
	sPCHAR_DATA tmpCh;

	int hashNum = GetStrHashIndex( id );

	for( block = pClient->DataList[hashNum].GetFirstBlock(); block; block = block->next )
	{
		tmpCh = (sPCHAR_DATA)block->data;

		if( !strcmp( tmpCh->id, id ) )
			return tmpCh;
	}
	return NULL;
}


//
// Function Name: WriteLoginTime
// Date:
// Description: 
//
void WriteLoginTime( char *id, BYTE charPos )
{
	char query[256];

	DWORD currTime = time(NULL);

	char escID[128];

	mysql_escape_string( escID, id, strlen( id ) );
	
	sprintf( query, "update UserTable set last_login = %d where id = '%s' and charPos = %d",
		currTime, escID, charPos );
	mysql_query( g_USER_DB, query );
}


//
// Function Name: GetCharInfo
// Date:
// Description: 
//
BOOL GetCharInfo( sPDESC_DATA pClient )
{
	char id[128];
	int charPos;

	GetString( pClient->recvBuff, id, pClient->recvPos );
	charPos = GetByte( pClient->recvBuff, pClient->recvPos );

	sPCHAR_DATA charInfo = GetDataFromDataList( pClient, id );
	
	// DataList 에 데이터가 존재하지 않을경우 
	// DB 에서 읽어오기
	if( !charInfo )
	{	
		char query[1024];
		char escID[128];

		mysql_escape_string( escID, id, strlen( id ) );
		
		sprintf( query, "select * from UserTable where id = '%s' and charPos = %d limit 1",
			escID, charPos );
		
		if( !mysql_query( g_USER_DB, query ) )
		{
			MYSQL_RES *result = mysql_store_result( g_USER_DB );
			
			if( result )
			{
				MYSQL_ROW row;
				
				if( row = mysql_fetch_row( result ) )
				{			

					charInfo = (sPCHAR_DATA)malloc( sizeof( sCHAR_DATA ) );
					
					if( !charInfo )
					{
						log( "GetCharInfo : malloc error\r\n" );
						return 0;
					}
					
					strcpy( charInfo->name, row[dDB_CHAR_NAME] );						// 케릭터 이름 
					strcpy( charInfo->id, row[dDB_CHAR_ID] );							// ID
					
					charInfo->charClass = atoi( row[dDB_CHAR_CHARCLASS] );				// 직업

					charInfo->sex = atoi( row[dDB_CHAR_SEX] );							// 성별
					charInfo->mapNum = atoi( row[dDB_CHAR_MAPNUM] );					// 맵번호
					
					charInfo->posX = atoi( row[dDB_CHAR_POSX] );
					charInfo->posY = atoi( row[dDB_CHAR_POSY] );						// 좌표
					charInfo->posLayer = atoi( row[dDB_CHAR_POSLAYER] );				// 레이어
					charInfo->posMapKind = atoi( row[dDB_CHAR_MAPKIND] );				// 맵 종류

					charInfo->prevMapNum = atoi( row[dDB_CHAR_PREVMAPNUM] );
					charInfo->prevPosX = atoi( row[dDB_CHAR_PREVPOSX] );
					charInfo->prevPosY = atoi( row[dDB_CHAR_PREVPOSY] );

					charInfo->hp = atoi( row[dDB_CHAR_HP] );
					charInfo->max_hp = atoi( row[dDB_CHAR_MAX_HP] );					// 체력
					charInfo->mp = atoi( row[dDB_CHAR_MP] );
					charInfo->max_mp = atoi( row[dDB_CHAR_MAX_MP] );					// 마나력
					
					charInfo->money = atoi( row[dDB_CHAR_MONEY] );						// 소유 금액	
					
					charInfo->skill[0] = atoi( row[dDB_CHAR_SKILL_LEVEL0] );			// 4 개의 스킬 레벨 
					charInfo->skill[1] = atoi( row[dDB_CHAR_SKILL_LEVEL1] );			// 4 개의 스킬 레벨 
					charInfo->skill[2] = atoi( row[dDB_CHAR_SKILL_LEVEL2] );			// 4 개의 스킬 레벨 
					charInfo->skill[3] = atoi( row[dDB_CHAR_SKILL_LEVEL3] );			// 4 개의 스킬 레벨 
					
					charInfo->skill_exp[0] = atoi( row[dDB_CHAR_SKILLEXP0] );			// 4 개의 스킬 경험치
					charInfo->skill_exp[1] = atoi( row[dDB_CHAR_SKILLEXP1] );			// 4 개의 스킬 경험치
					charInfo->skill_exp[2] = atoi( row[dDB_CHAR_SKILLEXP2] );			// 4 개의 스킬 경험치
					charInfo->skill_exp[3] = atoi( row[dDB_CHAR_SKILLEXP3] );			// 4 개의 스킬 경험치
					
					charInfo->str = atoi( row[dDB_CHAR_STR] );							// 힘
					charInfo->str_add = atoi( row[dDB_CHAR_STRADD] );					// 힘+
					charInfo->str_exp = atoi( row[dDB_CHAR_STREXP] );					// 힘 경험치
					charInfo->intelli = atoi( row[dDB_CHAR_INT] );						// 지력
					charInfo->intelli_add = atoi( row[dDB_CHAR_INTADD] );				// 지력 +
					charInfo->intelli_exp = atoi( row[dDB_CHAR_INTEXP] );				// 지력 경험치
					charInfo->dex = atoi( row[dDB_CHAR_DEX] );							// 민첩
					charInfo->dex_add = atoi( row[dDB_CHAR_DEXADD] );					// 민첩 +
					charInfo->dex_exp = atoi( row[dDB_CHAR_DEXEXP] );					// 민첩 경험치
					charInfo->charPos = charPos;										// 몇번째 케릭터?

					charInfo->headStyle = atoi( row[dDB_CHAR_HEADSTYLE] );				// 머리 스타일 
					charInfo->faceStyle = atoi( row[dDB_CHAR_FACESTYLE] );				// 얼굴 스타일

					charInfo->cha = atoi( row[dDB_CHAR_CHA] );							// 카리스마 

					charInfo->guildNo = atoi( row[dDB_CHAR_GUILDNO] );
					charInfo->guildLevel = atoi( row[dDB_CHAR_GUILDLEVEL] );
					charInfo->tmpGuildNum = atoi( row[dDB_CHAR_TEMP_GUILD] );
					
					charInfo->useSkill = atoi( row[dDB_CHAR_LASTSKILL] );				// 최후 사용 기술
					charInfo->isNewCharacter = atoi( row[dDB_CHAR_ISNEWCHARACTER] );	// 신규 케릭터 ?

					/*
					#define dCH_EQ_HEAD						0
					#define dCH_EQ_JACKET					1
					#define dCH_EQ_PANTS					2
					#define dCH_EQ_FOOT						3
					#define dCH_EQ_HAND						4

					#define dCH_EQ_MAX						5
					*/
					charInfo->chEquip[dCH_EQ_HEAD] = atoi( row[dDB_CHAR_EQUIPHEAD] );
					charInfo->chEquip[dCH_EQ_JACKET] = atoi( row[dDB_CHAR_EQUIPJACKET] );
					charInfo->chEquip[dCH_EQ_PANTS] = atoi( row[dDB_CHAR_EQUIPPANTS] );
					charInfo->chEquip[dCH_EQ_FOOT] = atoi( row[dDB_CHAR_EQUIPFOOT] );
					charInfo->chEquip[dCH_EQ_HAND] = atoi( row[dDB_CHAR_EQUIPHAND] );
					
					// 용병
					charInfo->mercType = atoi( row[dDB_CHAR_MERCTYPE] );
					charInfo->mercLevel = atoi( row[dDB_CHAR_MERCLEVEL] );
					charInfo->mercExp = atoi( row[dDB_CHAR_MERCEXP] );
					//여기서 캐쉬를 읽어드린다...

					g_nPos = 2;
					PutWord( g_Packet, dDATA_PACKET_GETCHARINFO_RESULT, g_nPos );
					PutString( g_Packet, id, g_nPos );
					memcpy( &g_Packet[g_nPos], charInfo, sizeof( sCHAR_DATA ) );
					g_nPos += sizeof( sCHAR_DATA );
					PutSize( g_Packet, g_nPos );
					
					SendData( pClient, g_Packet, g_nPos );
					
					int hashNum = GetStrHashIndex( id );
					pClient->DataList[hashNum].push( charInfo );					
				
					WriteLoginTime( id, charPos );
				}
				else
				{
					g_nPos = 2;
					PutWord( g_Packet, dDATA_PACKET_NULLCHAR, g_nPos );
					PutString( g_Packet, id, g_nPos );
					PutSize( g_Packet, g_nPos );
					SendData( pClient, g_Packet, g_nPos );
				}
				
				mysql_free_result( result );
			}
		}
	}
	else
	{
		// DataList 에 데이터가 존재할 경우
		g_nPos = 2;
		PutWord( g_Packet, dDATA_PACKET_GETCHARINFO_RESULT, g_nPos );
		PutString( g_Packet, id, g_nPos );
		memcpy( &g_Packet[g_nPos], charInfo, sizeof( sCHAR_DATA ) );
		g_nPos += sizeof( sCHAR_DATA );
		PutSize( g_Packet, g_nPos );
		
		SendData( pClient, g_Packet, g_nPos );
	}
	return 1;	
}

//
// Function Name: FindAnotherData
// Date:
// Description: 
//
BOOL FindAnotherData( char *where )
{
	char query[1024];
	
	sprintf( query, "select id from UserTable where %s", where );
	
	if( !mysql_query( g_USER_DB, query ) )
	{
		MYSQL_RES *result = mysql_store_result( g_USER_DB );
		
		if( result )
		{
			MYSQL_ROW row;
			
			// 이미 존재하는 케릭터 데이터
			if( row = mysql_fetch_row( result ) )
			{
				mysql_free_result( result );
				return 1;
			}
			mysql_free_result( result );
		}
	}
	else
		log( mysql_error( g_USER_DB ) );
	return 0;
}

//
// Function Name: MakeNewChar
// Date:
// Description: 
//
BOOL MakeNewChar( sPDESC_DATA pClient )
{
	char id[128];
	char name[128];
	BYTE sex;
	WORD hairStyle, faceStyle;
	WORD jacket, pants, glove, shoes;
	BYTE charPos;
	BYTE str, intelli, dex;
	BYTE str_add, int_add, dex_add;

	GetString( pClient->recvBuff, id, pClient->recvPos );
	GetString( pClient->recvBuff, name, pClient->recvPos );
	sex = GetByte( pClient->recvBuff, pClient->recvPos );
	hairStyle = GetWord( pClient->recvBuff, pClient->recvPos );
	faceStyle = GetWord( pClient->recvBuff, pClient->recvPos );
	jacket = GetWord( pClient->recvBuff, pClient->recvPos );
	pants = GetWord( pClient->recvBuff, pClient->recvPos );
	glove = GetWord( pClient->recvBuff, pClient->recvPos );
	shoes = GetWord( pClient->recvBuff, pClient->recvPos );

	str = GetByte( pClient->recvBuff, pClient->recvPos );
	intelli = GetByte( pClient->recvBuff, pClient->recvPos );
	dex = GetByte( pClient->recvBuff, pClient->recvPos );
	
	charPos = GetByte( pClient->recvBuff, pClient->recvPos );	

	str_add = MIN( str - 10, 0 );
	int_add = MIN( intelli - 10, 0 );
	dex_add = MIN( dex - 10, 0 );
	
	char query[1024];

	char escID[128];

	mysql_escape_string( escID, id, strlen( id ) );
	
	sprintf( query, "id = '%s' and charPos = %d limit 1", escID, charPos );

	if( FindAnotherData( query ) )
	{
		g_nPos = 2;
		PutWord( g_Packet, dDATA_PACKET_MAKENEWCHAR_RESULT, g_nPos );
		PutString( g_Packet, id, g_nPos );
		PutByte( g_Packet, 0, g_nPos );  // 실패
		PutSize( g_Packet, g_nPos );
		
		SendData( pClient, g_Packet, g_nPos );
		return 1;
	}

	char escName[128];

	mysql_escape_string( escName, name, strlen( name ) );

	sprintf( query, "name = '%s' limit 1", escName );

	if( FindAnotherData( query ) )
	{
		g_nPos = 2;
		PutWord( g_Packet, dDATA_PACKET_MAKENEWCHAR_RESULT, g_nPos );
		PutString( g_Packet, id, g_nPos );
		PutByte( g_Packet, 1, g_nPos );  // 이미 존재하는 케릭터명 
		PutSize( g_Packet, g_nPos );
		
		SendData( pClient, g_Packet, g_nPos );
		return 1;
	}
	
	DWORD currTime = time( NULL );

	sprintf( query, "insert into UserTable( "
		"idx, "
		"id, "
		"name, "
		"sex, "
		"headStyle, "
		"faceStyle, "
		"charPos , "
		"equipJacket, "
		"equipPants, "
		"equipHand, "
		"equipFoot, "
		"last_login, "
		"last_logout, "
		"str_point, "
		"int_point, "
		"dex_point, "
		"str_add,"
		"int_add,"
		"dex_add,"
		"isNewChar )"
		" values( "
		"'',"
		"'%s',"
		"'%s',"
		"%d,"
		"%d,"
		"%d,"
		"%d,"
		"%d,"
		"%d,"
		"%d,"
		"%d,"
		"%ld,"
		"%ld,"
		"%d,"
		"%d,"
		"%d,"
		"%d,"
		"%d,"
		"%d,"
		"1 )",
		escID, 
		escName,
		sex, 
		hairStyle, 
		faceStyle, 
		charPos,
		jacket,
		pants,
		glove,
		shoes,
		currTime, 
		currTime,
		10,
		10,
		10,
		str_add,
		int_add,
		dex_add
		);
	
	if( !mysql_query( g_USER_DB, query ) )
	{
		g_nPos = 2;
		PutWord( g_Packet, dDATA_PACKET_MAKENEWCHAR_RESULT, g_nPos );
		PutString( g_Packet, id, g_nPos );
		PutByte( g_Packet, 2, g_nPos );  // 성공
		PutSize( g_Packet, g_nPos );
		
		SendData( pClient, g_Packet, g_nPos );
	}
	else
	{
		g_nPos = 2;
		PutWord( g_Packet, dDATA_PACKET_MAKENEWCHAR_RESULT, g_nPos );
		PutString( g_Packet, id, g_nPos );
		PutByte( g_Packet, 0, g_nPos );  // 실패
		PutSize( g_Packet, g_nPos );
		
		SendData( pClient, g_Packet, g_nPos );
	}

	sprintf( query, "insert into CreateLog( idx, id, name, charPos, date ) values( '', '%s', '%s', %d, %ld )",
		escID, escName, charPos, currTime );

	mysql_query( g_USER_DB, query );

	return 1;
}


//
// Function Name: UpdateCharData
// Date:
// Description: 
//
void UpdateCharData( sPCHAR_DATA pChar )
{
	pChar->isNewCharacter = 0;

	char query[1024];
	char escID[128];

	mysql_escape_string( escID, pChar->id, strlen( pChar->id ) );

	DWORD currTime = time( NULL );
	
	sprintf( query, "update UserTable set "
		"money = %d,"
		"sex = %d,"
		"mapNum = %d,"
		"posX = %d,"
		"posY = %d,"
		"mapKind = %d,"
		"hp = %d,"
		"mp = %d,"
		"str_point = %d,"
		"int_point = %d,"
		"dex_point = %d,"
		"skill_exp0 = %d,"
		"skill_exp1 = %d,"
		"skill_exp2 = %d,"
		"skill_exp3 = %d,"
		"skill_level0 = %d,"
		"skill_level1 = %d,"
		"skill_level2 = %d,"
		"skill_level3 = %d,"
		"last_logout = %ld,"
		"str_exp = %d,"
		"intelli_exp = %d,"
		"dex_exp = %d,"
		"max_hp = %d,"
		"max_mp = %d,"
		"equipHead = %d,"
		"equipJacket = %d,"
		"equipPants = %d,"
		"equipFoot = %d,"
		"equipHand = %d,"
		"mercType = %d,"
		"mercLevel = %d,"
		"mercExp = %d,"
		"posLayer = %d,"
		"cha = %d,"
		"guildNo = %d,"
		"guildLevel = %d,"
		"prevMapNum = %d,"
		"prevPosX = %d,"
		"prevPosY = %d,"
		"lastSkill = %d,"
		"isNewChar = %d,"
		"charClass = %d,"
		"tempGuild = %d"
		" where id = '%s' and charPos = %d",
		
		// 돈
		pChar->money,			
		// 성별, 맵번호, 좌표X, 좌표 Y, 맵 종류
		pChar->sex,
		pChar->mapNum, 
		pChar->posX, 
		pChar->posY, 
		pChar->posMapKind,
		// HP, MP, STR, INT, DEX
		pChar->hp,
		pChar->mp, 
		pChar->str, 
		pChar->intelli,
		pChar->dex,
		// 스킬 경험치 4개
		pChar->skill_exp[0], 
		pChar->skill_exp[1], 
		pChar->skill_exp[2], 
		pChar->skill_exp[3],
		// 스킬 레벨 4 개
		pChar->skill[0], 
		pChar->skill[1],
		pChar->skill[2], 
		pChar->skill[3],
		// 최종 로그아웃시간, STR 경험치 , INT 경험치 , DEX 경험치
		currTime, 
		pChar->str_exp, 
		pChar->intelli_exp,
		pChar->dex_exp,
		// MAX HP, MAX MP
		pChar->max_hp, 
		pChar->max_mp,

		// 장착 
		pChar->chEquip[dCH_EQ_HEAD],
		pChar->chEquip[dCH_EQ_JACKET],
		pChar->chEquip[dCH_EQ_PANTS],
		pChar->chEquip[dCH_EQ_FOOT], 
		pChar->chEquip[dCH_EQ_HAND],
		
		// 용병 
		pChar->mercType,
		pChar->mercLevel,
		pChar->mercExp,
		
		// 맵 레이어
		pChar->posLayer,
		
		// 카리스마 
		pChar->cha, 

		pChar->guildNo,
		pChar->guildLevel,
				
		// prev Map Info
		pChar->prevMapNum,
		pChar->prevPosX,
		pChar->prevPosY,

		pChar->useSkill,
		
		pChar->isNewCharacter,
		pChar->charClass,

		pChar->tmpGuildNum,
		
		escID , pChar->charPos );
	
	if( mysql_query( g_USER_DB, query ) )
		log( "LogOutPlayer:: Data Update Failed\r\n" );
}

//
// Function Name: LogOutPlayer
// Date:
// Description: 
//
BOOL LogOutPlayer( sPDESC_DATA pClient )
{
	char id[128];
	WORD lastSkill;

	GetString( pClient->recvBuff, id, pClient->recvPos );
	lastSkill = GetWord( pClient->recvBuff, pClient->recvPos );

	sPCHAR_DATA charInfo = GetDataFromDataList( pClient, id );

	if( !charInfo )
	{
//		log( "LogOutPlayer : Can't find [%s]\r\n" , id );
		return 1;
	}
	
	charInfo->useSkill = lastSkill;

	UpdateCharData( charInfo );

	int hashNum = GetStrHashIndex( id );
	pClient->DataList[hashNum].remove( charInfo );

	return 1;
}

//
// Function Name: UpdateSkill
// Date:
// Description: 
//
BOOL UpdateSkill( sPDESC_DATA pClient )
{
	char id[128];
	int skill;
	BYTE skill_num;
	
	GetString( pClient->recvBuff, id, pClient->recvPos );
	skill_num = GetByte( pClient->recvBuff, pClient->recvPos );
	skill = GetInteger( pClient->recvBuff, pClient->recvPos );
	
	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );

	if( pChar )
		pChar->skill[skill_num] = skill;

	return 1;
}

//
// Function Name: UpdateSkillExp
// Date:
// Description: 
//
BOOL UpdateSkillExp( sPDESC_DATA pClient )
{
	char id[128];
	int skill_exp;
	BYTE skill_num;
	
	GetString( pClient->recvBuff, id, pClient->recvPos );
	skill_num = GetByte( pClient->recvBuff, pClient->recvPos );
	skill_exp = GetInteger( pClient->recvBuff, pClient->recvPos );
	
	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );
	
	if( pChar )
		pChar->skill_exp[skill_num] = skill_exp;

	return 1;
}



//
// Function Name: UpdateHp
// Date:
// Description: 
//
BOOL UpdateHp( sPDESC_DATA pClient )
{
	char id[128];

	int hp, max_hp;

	GetString( pClient->recvBuff, id, pClient->recvPos );
	hp = GetInteger( pClient->recvBuff, pClient->recvPos );
	max_hp = GetInteger( pClient->recvBuff, pClient->recvPos );
	
	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );
	
	if( pChar )
	{
		pChar->hp = hp;
		pChar->max_hp = max_hp;
	}

	return 1;
}

//
// Function Name: UpdateMp
// Date: 
// Description: 
//
BOOL UpdateMp( sPDESC_DATA pClient )
{
	char id[128];

	int mp, max_mp;

	GetString( pClient->recvBuff, id, pClient->recvPos );
	mp = GetInteger( pClient->recvBuff, pClient->recvPos );
	max_mp = GetInteger( pClient->recvBuff, pClient->recvPos );
	
	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );
	
	if( pChar )
	{
		pChar->mp = mp;
		pChar->max_mp = max_mp;
	}

	return 1;
}

//
// Function Name: UpdateMapInfo
// Date: 
// Description: 
//
BOOL UpdateMapInfo( sPDESC_DATA pClient )
{
	char id[128];
	
	int mapNum, posX, posY;
	BYTE layer, mapKind;
	int prevMapNum, prevPosX, prevPosY;

	GetString( pClient->recvBuff, id, pClient->recvPos );
	mapNum = GetInteger( pClient->recvBuff, pClient->recvPos );
	posX = GetInteger( pClient->recvBuff, pClient->recvPos );
	posY = GetInteger( pClient->recvBuff, pClient->recvPos );
	layer = GetByte( pClient->recvBuff, pClient->recvPos );
	mapKind = GetByte( pClient->recvBuff, pClient->recvPos );
	prevMapNum = GetInteger( pClient->recvBuff, pClient->recvPos );
	prevPosX = GetInteger( pClient->recvBuff, pClient->recvPos );
	prevPosY = GetInteger( pClient->recvBuff, pClient->recvPos );

	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );
	
	if( pChar )
	{
		pChar->mapNum = mapNum;
		pChar->posX = posX;
		pChar->posY = posY;
		pChar->posLayer = layer;
		pChar->posMapKind = mapKind;		
		//
		pChar->prevMapNum = prevMapNum;
		pChar->prevPosX = prevPosX;
		pChar->prevPosY = prevPosY;
	}
	return 1;
}

//
// Function Name: UpdateMoney
// Date: 
// Description: 
//
BOOL UpdateMoney( sPDESC_DATA pClient )
{
	char id[128];

	int money;
	
	GetString( pClient->recvBuff, id, pClient->recvPos );
	money = GetInteger( pClient->recvBuff, pClient->recvPos );

	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );

	if( pChar )
		pChar->money = money;

	return 1;
}


BOOL UpdateCash( sPDESC_DATA pClient )
{
	char id[128];

	int  cash;

	GetString( pClient->recvBuff, id, pClient->recvPos );
	cash = GetInteger( pClient->recvBuff, pClient->recvPos );
	
	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );
	
	if( pChar )
		pChar->nCash = cash;
	
	return 1;
}


//
// Function Name: UpdateStr
// Date: 
// Description: 
//
BOOL UpdateStr( sPDESC_DATA pClient )
{
	char id[128];
	
	int var;
	
	GetString( pClient->recvBuff, id, pClient->recvPos );
	var = GetInteger( pClient->recvBuff, pClient->recvPos );
	
	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );
	
	if( pChar )
		pChar->str = var;
	return 1;
}

//
// Function Name: UpdateStrExp
// Date: 
// Description: 
//
BOOL UpdateStrExp( sPDESC_DATA pClient )
{
	char id[128];
	
	int var;
	
	GetString( pClient->recvBuff, id, pClient->recvPos );
	var = GetInteger( pClient->recvBuff, pClient->recvPos );
	
	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );
	
	if( pChar )
		pChar->str_exp = var;
	return 1;
}

//
// Function Name: UpdateInt
// Date: 
// Description: 
//
BOOL UpdateInt( sPDESC_DATA pClient )
{
	char id[128];
	
	int var;
	
	GetString( pClient->recvBuff, id, pClient->recvPos );
	var = GetInteger( pClient->recvBuff, pClient->recvPos );
	
	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );
	
	if( pChar )
		pChar->intelli = var;
	return 1;
}

//
// Function Name: UpdateIntExp
// Date: 
// Description: 
//
BOOL UpdateIntExp( sPDESC_DATA pClient )
{
	char id[128];
	
	int var;
	
	GetString( pClient->recvBuff, id, pClient->recvPos );
	var = GetInteger( pClient->recvBuff, pClient->recvPos );
	
	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );
	
	if( pChar )
		pChar->intelli_exp = var;
	return 1;
}

//
// Function Name: UpdateDex
// Date: 
// Description: 
//
BOOL UpdateDex( sPDESC_DATA pClient )
{
	char id[128];
	
	int var;
	
	GetString( pClient->recvBuff, id, pClient->recvPos );
	var = GetInteger( pClient->recvBuff, pClient->recvPos );
	
	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );
	
	if( pChar )
		pChar->dex = var;
	return 1;
}

//
// Function Name: UpdateDexExp
// Date: 
// Description: 
//
BOOL UpdateDexExp( sPDESC_DATA pClient )
{
	char id[128];
	
	int var;
	
	GetString( pClient->recvBuff, id, pClient->recvPos );
	var = GetInteger( pClient->recvBuff, pClient->recvPos );
	
	sPCHAR_DATA pChar = GetDataFromDataList( pClient, id );
	
	if( pChar )
		pChar->dex_exp = var;
	return 1;
}


//
// Function Name: DeleteCharacter
// Date: 
// Description: 
//
BOOL DeleteCharacter( sPDESC_DATA pClient )
{
	char id[50];
	BYTE charPos;

	GetString( pClient->recvBuff, id, pClient->recvPos );
	charPos = GetByte( pClient->recvBuff, pClient->recvPos );

	sPCHAR_DATA charInfo = GetDataFromDataList( pClient, id );

	if( charInfo )
	{
		int hashNum = GetStrHashIndex( id );
		pClient->DataList[hashNum].remove( charInfo );
	}

	char query[1024];
	char escID[128];

	mysql_escape_string( escID, id, strlen( id ) );

	sprintf( query, "insert into DELUserTable select * from UserTable where id = '%s' and charPos = %d limit 1", escID, charPos );

	if( mysql_query( g_USER_DB, query ) )
		log( "DELUserTable write failed!! [%s/%s]\r\n", id, charPos );

	sprintf( query, "delete from UserTable where id = '%s' and charPos = %d", escID, charPos );
	
	if( !mysql_query( g_USER_DB, query ) )
	{
		g_nPos = 2;
		PutWord( g_Packet, dDATA_PACKET_DELCHAR_RESULT, g_nPos );
		PutString( g_Packet, id, g_nPos );
		PutByte( g_Packet, 1, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pClient, g_Packet, g_nPos );
	
		DWORD currTime = time(NULL);

		sprintf( query, "insert into DeleteLog( idx, id, charPos, date ) values( '', '%s', %d, %ld )",
			escID, charPos, currTime );

		mysql_query( g_USER_DB, query );
	}
	else
	{
		g_nPos = 2;
		PutWord( g_Packet, dDATA_PACKET_DELCHAR_RESULT, g_nPos );
		PutString( g_Packet, id, g_nPos );
		PutByte( g_Packet, 0, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pClient, g_Packet, g_nPos );
	}

	return 1;
}

//
// Function Name: UpdateEquip
// Date: 
// Description: 
//
BOOL UpdateEquip( sPDESC_DATA pClient )
{
	char id[50];
	WORD foot, hand, head, jacket, pants;

	/*
	PutWord( g_Packet, pPlayer->ch.chEquip[dCH_EQ_FOOT], g_nPos );
	PutWord( g_Packet, pPlayer->ch.chEquip[dCH_EQ_HAND], g_nPos );
	PutWord( g_Packet, pPlayer->ch.chEquip[dCH_EQ_HEAD], g_nPos );
	PutWord( g_Packet, pPlayer->ch.chEquip[dCH_EQ_JACKET], g_nPos );
	PutWord( g_Packet, pPlayer->ch.chEquip[dCH_EQ_PANTS], g_nPos );
	*/
	GetString( pClient->recvBuff, id, pClient->recvPos );
	foot = GetWord( pClient->recvBuff, pClient->recvPos );
	hand = GetWord( pClient->recvBuff, pClient->recvPos );
	head = GetWord( pClient->recvBuff, pClient->recvPos );
	jacket = GetWord( pClient->recvBuff, pClient->recvPos );
	pants = GetWord( pClient->recvBuff, pClient->recvPos );

	sPCHAR_DATA charInfo = GetDataFromDataList( pClient, id );

	if( charInfo )
	{
		charInfo->chEquip[dCH_EQ_FOOT] = foot;
		charInfo->chEquip[dCH_EQ_HAND] = hand;
		charInfo->chEquip[dCH_EQ_HEAD] = head;
		charInfo->chEquip[dCH_EQ_JACKET] = jacket;
		charInfo->chEquip[dCH_EQ_PANTS] = pants;
	}

	return 1;
}

//
// Function Name: UpdateMecrInfo
// Date: 
// Description: 
//
BOOL UpdateMecrInfo( sPDESC_DATA pClient )
{
	char id[50];
	BYTE mercType;
	int mercLevel, mercExp;

	GetString( pClient->recvBuff, id, pClient->recvPos );
	mercType = GetByte( pClient->recvBuff, pClient->recvPos );
	mercLevel = GetInteger( pClient->recvBuff, pClient->recvPos );
	mercExp = GetInteger( pClient->recvBuff, pClient->recvPos );

	sPCHAR_DATA charInfo = GetDataFromDataList( pClient, id );

	if( charInfo )
	{
		charInfo->mercType = mercType;
		charInfo->mercLevel = mercLevel;
		charInfo->mercExp = mercExp;
	}

	return 1;
}

//
// Function Name: UpdateCha
// Date: 
// Description: 
//
BOOL UpdateCha( sPDESC_DATA pClient )
{
	char id[50];
	int cha;

	GetString( pClient->recvBuff, id, pClient->recvPos );
	cha = GetInteger( pClient->recvBuff, pClient->recvPos );

	sPCHAR_DATA charInfo = GetDataFromDataList( pClient, id );

	if( charInfo )
	{
		charInfo->cha = cha;
	}
	return 1;
}





//
// Function Name: UpdateGuild
// Date: 
// Description: 
//
BOOL UpdateGuild( sPDESC_DATA pClient )
{
	char id[50];
	int GuildNo, GuildLevel, tmpGuild;

	GetString( pClient->recvBuff, id, pClient->recvPos );
	GuildNo = GetInteger( pClient->recvBuff, pClient->recvPos );
	GuildLevel = GetInteger( pClient->recvBuff, pClient->recvPos );
	tmpGuild = GetInteger( pClient->recvBuff, pClient->recvPos );	

	sPCHAR_DATA charInfo = GetDataFromDataList( pClient, id );

	if( charInfo )
	{
		charInfo->guildLevel = GuildLevel;
		charInfo->guildNo = GuildNo;
		charInfo->tmpGuildNum = tmpGuild;
	}

	return 1;
}

//
// Function Name: UpdateClass
// Date: 
// Description: 
//
BOOL UpdateClass( sPDESC_DATA pClient )
{
	char id[50];
	BYTE charClass;

	GetString( pClient->recvBuff, id, pClient->recvPos );
	charClass = GetByte( pClient->recvBuff, pClient->recvPos );

	sPCHAR_DATA charInfo = GetDataFromDataList( pClient, id );

	if( charInfo )
	{
		charInfo->charClass = charClass;
	}
	return 1;
}


//
// Function Name: CrashGuild
// Date: 
// Description: 
//
BOOL CrashGuild( sPDESC_DATA pClient )
{
	int guildNo = GetInteger( pClient->recvBuff, pClient->recvPos );

	char query[1024];

	sprintf( query, "update UserTable set guildNo = '0', guildLevel = '0' where guildNo = %d", guildNo );

	if( mysql_query( g_USER_DB, query ) )
		log( mysql_error( g_USER_DB ) );

	return 1;
}


//
// Function Name: MessageFuncsList
// Date: 2002-02-12
// Description: 패킷 처리 함수들의 포인터 배열
//
BOOL (*MessageFuncsList[])( sPDESC_DATA pClient ) =
{
	NullFunc,					//#define dDATA_PACKET_RESERVED						0		// 사용 안함
	GetSelectInfo,				//#define dDATA_PACKET_GETSELECTINFO				1
	NullFunc,					//#define dDATA_PACKET_SENDSELECTINFO				2
	GetCharInfo,				//#define dDATA_PACKET_GETCHARINFO					3		// 선택한 플레이 데이터 요청
	NullFunc,					//#define dDATA_PACKET_GETCHARINFO_RESULT			4		// 선택한 플레이 데이터 전송 
	NullFunc,					//#define dDATA_PACKET_NULLCHAR						5		// 케릭터 존재하지 않음
	MakeNewChar,				//#define dDATA_PACKET_MAKENEWCHAR					6		// 신규 케릭터 생성
	NullFunc,					//#define dDATA_PACKET_MAKENEWCHAR_RESULT			7		// 신규 케릭터 생성 결과
	LogOutPlayer,				//#define dDATA_PACKET_CHAR_LOGOUT					8		// 로그아웃 ( 데이터 저장 )

	NullFunc,					//#define dDATA_PACKET_UPDATELEVEL					9		// 레벨업 정보
	UpdateSkill,				//#define dDATA_PACKET_UPDATESKILL					10		// 스킬업 정보
	UpdateSkillExp,				//#define dDATA_PACKET_UPDATESKILL_EXP				11		// 스킬 경치 정보 
	
	UpdateStr,					//#define dDATA_PACKET_UPDATESTR					12
	UpdateStrExp,				//#define dDATA_PACKET_UPDATESTR_EXP				13
	UpdateInt,					//#define dDATA_PACKET_UPDATEINT					14
	UpdateIntExp,				//#define dDATA_PACKET_UPDATEINT_EXP				15
	UpdateDex,					//#define dDATA_PACKET_UPDATEDEX					16
	UpdateDexExp,				//#define dDATA_PACKET_UPDATEDEX_EXP				17

	UpdateHp,					//#define dDATA_PACKET_UPDATEHP						18		// HP
	UpdateMp,					//#define dDATA_PACKET_UPDATEMP						19		// MP
	UpdateMapInfo,				//#define dDATA_PACKET_UPDATEMAPINFO				20		// mapNum, posX, posY
	UpdateMoney,				//#define dDATA_PACKET_UPDATEMONEY					21		// 소유 금액

	DeleteCharacter,			//#define dDATA_PACKET_DELCHAR						22		// 케릭터 삭제
	NullFunc,					//#define dDATA_PACKET_DELCHAR_RESULT				23		// 케릭터 삭제 결과 
	
	NullFunc,					//#define dDATA_PACKET_ISCONNECTED					24		// 이미 접속중인 계정

	UpdateEquip,				//#define dDATA_PACKET_UPDATEEQUIP					25		// 장비
	UpdateMecrInfo,				//#define dDATA_PACKET_UPDATEMERC					26		// 용병 정보
	UpdateCha,					//#define dDATA_PACKET_UPDATECHA					27		// 카리스마

	NullFunc,					//#define dDATA_PACKET_UPDATECOUNTRY				28		// 국가
	NullFunc,					//#define dDATA_PACKET_UPDATEHOMETOWN				29		// 고향마을

	UpdateGuild,				//#define dDATA_PACKET_UPDATEGUILD					30		// 길드정보
	NullFunc,					//#define dDATA_PACKET_UPDATECOUNTRYLEVEL			31		// 국가 레벨
	CrashGuild,					//#define dDATA_PACKET_CRASHGUILD					32		// 길드 아작
	UpdateClass,				//#define dDATA_PACKET_UPDATECLASS					33		// 직업 업데이트
	UpdateCash,					//#define 
};



//
// Function Name: NullFunc
// Date: 2002-02-12
// Description: recv buffer에 있는 데이터들을 처리함
// TODO : 추후에 .. 순환 Queue로 고쳐야함
//
BOOL ReadRecvBuff( sPDESC_DATA pClient )
{
	WORD msgSize;
	WORD tag;

	while( pClient->recvSize )
	{
		pClient->recvPos = 0;

		msgSize = GetWord( pClient->recvBuff, pClient->recvPos );
		tag = GetWord( pClient->recvBuff, pClient->recvPos );

		if( tag > dMAX_TAG  )
		{
			log( "ReadRecvBuff [%s] : tag > dMAX_TAG [%d]\r\n", pClient->ip, tag );
			return 0;
		}

		if( pClient->recvSize < msgSize )
			return 1;

		if( !MessageFuncsList[tag]( pClient ) )
		{
			log( "ReadRecvBuff [%s] : MessageFuncList return FALSE [tag:%d/size:%d]\r\n", 
				pClient->ip, tag, msgSize );
			return 0;
		}
		
		memmove( &pClient->recvBuff[0], &pClient->recvBuff[msgSize], pClient->recvSize - msgSize );
		pClient->recvSize -= msgSize;
	}
	return 1;
}


