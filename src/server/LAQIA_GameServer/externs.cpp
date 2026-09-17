#include "def.h"
#include "structs.h"
#include "externs.h"
#include "party.h"
#include "player.h"
#include "skills.h"
#include "friends.h"

/**************************************************
 *	E X T E R N  V A R I A B L E S
 **************************************************/
BOOL					g_GameLife = 1;
SOCKET					g_MainSock = -1;

int						g_ServerPort = dSERVER_PORT;
int						g_MaxListen	 = dMAX_LISTEN;
char					g_ServerName[128];

#ifdef WIN32
HINSTANCE				hInst;		// Current instance
HWND					g_hwnd;		// Window Handle
#else
DWORD					g_ProcessStartTime;
#endif

DWORD					g_CurrTime;
DWORD					g_CurrRealTime;

int						i,j,k,l,x,y,z;

char					escID[128];
char					escName[128];
char					escItemName[128];
char					escMaker[128];
char					escSerial[128];

sDB_INFO				g_DATADB;		// DATADB
sDB_INFO				g_LOGDB;		// LOGDB
CMySql					g_DataSql;	
CMySql					g_ItemSql;
CMySql					g_LogSql;

sGLOBAL_MEMORY			g_MEMORY;

int						g_EXPTABLE[dMAX_LEVEL];	

DWORD					timePulse = 0;


/*
 *	InitGlobalMemory
 */
void InitGlobalMemory()
{
	g_MEMORY.actionMemory = NULL;
	g_MEMORY.addDurationMemory = NULL;
	g_MEMORY.playerQuestMemory = NULL;
	g_MEMORY.friendMemory = NULL;
}

/*
 *	DestroyGlobalMemory
 */
void DestroyGlobalMemory()
{
	/*
	 *	Action
	 */
	sPACTION act, next_act;

	LIST_WHILE( g_MEMORY.actionMemory, act, next_act, next );
	REMOVE_FROM_LIST( g_MEMORY.actionMemory, act, prev, next );
	SAFE_FREE( act );
	LIST_WHILEEND();

	/*
	 *	Add Duration memory
	 */
	sPADD_DURATION dur, next_dur;

	LIST_WHILE( g_MEMORY.addDurationMemory, dur, next_dur, next );
	REMOVE_FROM_LIST( g_MEMORY.addDurationMemory, dur, prev, next );
	SAFE_FREE( dur );
	LIST_WHILEEND();

	/*
	 *	PLAYER QUEST
	 */

	sPPLAYER_QUEST quest, next_quest;

	LIST_WHILE( g_MEMORY.playerQuestMemory, quest, next_quest, next );
	REMOVE_FROM_LIST( g_MEMORY.playerQuestMemory, quest, prev, next );
	SAFE_FREE( quest );
	LIST_WHILEEND();

	// PLAYER FRIENDS
	DestroyFriendMemory();

}

/*
 *	CHATFUNC_PartyOut
 */
void CHATFUNC_PartyOut( sPDESC_DATA pPlayer, char *command )
{
	if( !GET_PARTY_MASTER( pPlayer ) )
		return;

	// 파티장이면 파티해체 
	if( GET_PARTY_MASTER( pPlayer ) == pPlayer )
	{
		PARTY_Destroy( pPlayer );
		return;
	}
	else // 일반 맴버면 파티서 나가기
	{
		sPDESC_DATA pMaster = GET_PARTY_MASTER( pPlayer );

		PARTY_OutMember( pPlayer );
		PARTY_CheckDeatroy( pMaster );
	}
}


/*
 *	admin_cmd[]
 */
const sCHAT_COMMAND chat_cmd[] =
{
	{	"탈퇴",			CHATFUNC_PartyOut,				},
	{	"\n",			NULL,							},
};

/*
 *	CheckAdminCommand
 */
BOOL CheckChatCommand( sPDESC_DATA pPlayer, char *command )
{
	int cmd = 0;

	for( int i = 0; i < 512; i++ )
	{
		if( *chat_cmd[cmd].command == '\n' )
			return 0;

		if( !strncmp( chat_cmd[cmd].command, command, strlen( chat_cmd[cmd].command ) ) )
		{
			chat_cmd[cmd].chat_func( pPlayer, &command[strlen( chat_cmd[cmd].command )+1] );
			return 1;
		}
		cmd++;		
	}
	return 0;
}



//EOF

