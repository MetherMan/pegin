#ifndef __NPC_WORLD_H__
#define __NPC_WORLD_H__


/*
 *	
 */
typedef struct 
{
	BYTE btn[4];
} sNPC_BUTTON, *sPNPC_BUTTON;


#define dNPC_BUTTON_NONE			0		// 없음 
#define dNPC_BUTTON_SHOP			1		// 상점 
#define dNPC_BUTTON_QUEST			2		// 퀘스트
#define dNPC_BUTTON_WAREHOUSE		3		// 창고
#define dNPC_BUTTON_SKILL			4		// 스킬
#define dNPC_BUTTON_CHANGEJOB		5		// 전직
#define dNPC_BUTTON_10JUN			6		// 열전
#define dNPC_BUTTON_VEHICLE			7		// 이동용마차
#define dNPC_BUTTON_RESETJOB		8		// reset job
#define dNPC_BUTTON_CHANGECOUNTRY	9		// 국가선택

/*
 *	E X T E R N   F U N C T I O N S
 */
void MakeNpcInPacket( sPNPC_DATA pNpc );
void MakeNpcOutPacket( sPNPC_DATA pNpc );
void GetNpcSector( sPDESC_DATA pPlayer, int x, int y );
void GetNpcVSector( int x, int startY, int endY, sPDESC_DATA pPlayer );
void GetNpcHSector( int y, int startX, int endX, sPDESC_DATA pPlayer );
void OutNpcSector( sPDESC_DATA pPlayer, int x, int y );
void OutNpcVSector( int x, int startY, int endY, sPDESC_DATA pPlayer );
void OutNpcHSector( int y, int startX, int endX, sPDESC_DATA pPlayer );
void SendToNpcVSector( int x, int startY, int endY, sPNPC_DATA pNpc, char *data, int size );
void SendToNpcHSector( int y, int startX, int endX, sPNPC_DATA pNpc, char *data, int size );
void SendToNpcArea( sPNPC_DATA pNpc, char *data, int size );
void GetCurrAreaNpc( sPDESC_DATA pPlayer );
void OutCurrAreaNpc( sPDESC_DATA pPlayer );
sPNPC_DATA FindNpcInTile( sPNPC_DATA list, int idxNum );
sPNPC_DATA FindNpcInArea( int mapNum, int posX, int posY, int idxNum );

void NPC_TalkProc();
void NPC_Talk( sPNPC_DATA pNpc );
void NPC_NormalTalk( sPNPC_DATA pNpc, sPDESC_DATA pPlayer );
void NPC_QuestTalk( sPNPC_DATA pNpc, sPDESC_DATA pPlayer );
void MoveNPC( sPNPC_DATA pNpc, int posX, int posY );

void LoadNpcMovePoint( int npcNum );
void DestroyNpcMovePoint( sPNPC_DATA pNpc );
void AddNpcMovePoint( sPNPC_DATA pNpc, int posX, int posY );
void LoadNpcMovePoint( int npcNum );
void NpcMoveProc();

#endif

//EOF




