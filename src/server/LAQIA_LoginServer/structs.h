/* structs.h */ 

#ifndef __STRUCTS_H__
#define __STRUCTS_H__

///////////////////////////////////////////////
// for name hash
///////////////////////////////////////////////
#define dHASHKEY_STR_ETC	85
#define dHASHKEY_STR_MAX	86


struct _Client_Data
{
	SOCKET	sock;							// 소켓번호 
	char	ip[20];							// ip 

	char	id[50];							// 아이디 

	DWORD	lastRecvTime;					
	DWORD	lastAliveCheck;					// 연결상태 체크 패킷 최종 도착시간 
	// 시간제 사용자 남은 시간 관리 
	DWORD	timeLimit;						// 시간제 유저의 사용가능 시간을 분(min) 단위로 계산 
	BYTE	IsAccepted;				

	char	recvBuff[dMAX_SOCK_BUFF];		// recv 데이터 버퍼 
	int		recvSize;						// recv 버퍼 상의 총 데이터 량
	int		recvPos;						
	
	char	sendBuff[dMAX_SOCK_BUFF];		// send 대기중인 데이터 0
	int		sendSize;						// send 할 데이터량

	BYTE	disconnectFlag;

	// 전체 리스트용 prev, next
	struct	_Client_Data *prev;		
	struct	_Client_Data *next;	
	
	// ID 리스트용 prev, next
	struct  _Client_Data *id_prev;
	struct	_Client_Data *id_next;
};

typedef struct _Client_Data sClient_Data, *pClient_Data;

struct sClients
{
	pClient_Data id_list[dHASHKEY_STR_MAX];		// ID 를 이용한 hash 테이블 

	pClient_Data list;		// 전체 접속자 리스트
	pClient_Data memory;	// Garbage List
	int	total;
	int	total_memory;		
};

typedef struct
{
	char	host[128];
	int	port;

	char	id[128];
	char	pw[128];
	char	dbName[128];
} sDB_INFO, *sPDB_INFO;


#endif // __STRUCTS_H__
