/* message.h */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

BOOL NullFunc( pClient_Data pClient );

extern BOOL (*MessageFuncsList[])( pClient_Data pClient, char *readBuff, int readPos );

#endif //__MESSAGE_H__
