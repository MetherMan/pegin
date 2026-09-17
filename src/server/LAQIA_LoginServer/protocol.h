/* protocol.h 패킷넘버를 정의한다 */

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define dFIRST_PACKET						1					

#define dPACKET_RESERVED					0

#define dLOGIN_PACKET_LOGININFO				1	// client -> server
#define dLOGIN_PACKET_LOGINRESULT			2	// server -> client 

//(TODO) 테스트용이므로 1 -_- 까지만 
#define dMAX_TAG							2	


//////////////// GameServer
#define dMAX_TAG_GAMESERVER 0

#endif // __PROTOCOL_H__
