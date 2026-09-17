/* client.h */

#ifndef __CLIENT_H__
#define __CLIENT_H__

pClient_Data	NewClient();
void			AcceptNewClient( SOCKET MotherSock );
void			InitClientData( pClient_Data pData, SOCKET sock, char *hostname );
BOOL			RecvFromClient( pClient_Data pClient );
BOOL			ReadRecvBuff( pClient_Data pClient );
void			SendData( pClient_Data pClient , const char *data , int size );
int				FlushSendBuff( pClient_Data pClient );
void			DisconnectClient( pClient_Data pData );
pClient_Data	FindClientIdList( char* id );
void			SaveUserData( pClient_Data pClient_Data );

#endif // __CLIENT_H__
