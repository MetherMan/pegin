/**
 * UInterface.DLL 익스포트 전역 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-06-10 5:33p $
 * @version	$Revision: 10 $
 *
 * <HR>
 * $Log: /UInterface/Src/UInterface_Export.h $
 * 
 * 10    04-06-10 5:33p Paran
 * 
 * 9     04-06-08 3:56p Paran2
 * 
 * 8     04-06-02 11:59a Paran
 * 
 * 7     04-01-19 3:11p Paran
 * 
 * 6     04-01-13 7:37p Kjmgo
 * 
 * 5     03-12-14 3:23a Paran
 * 
 * 4     03-10-01 4:28p Paran
 * 
 * 3     03-07-25 12:05a Paran
 * 
 * 2     03-05-24 6:38a Ahastudio
 * <HR>
 *
 * @file	UInterface_Export.h
 */


#ifndef __UInterface_Export_Header__
#define __UInterface_Export_Header__


#include "../Inc/UInterface.h"
#include <queue>

/**
 * 이벤트 정보.
 * @ingroup	Engine
 */
struct UIEvent
{
	const char*		pCtrlName;
	int				nEvent;
	DWORD			dwParam1;
	DWORD			dwParam2;
}; //struct UIEvent

typedef std::queue<UIEvent>					QUEUE_UIEVENT;		///<

extern IW3DScene*		g_pScene;				///<
extern IW3DUIGraphic*	g_pUIGraphicMgr;

extern QUEUE_UIEVENT	g_queueEvent;

extern int				g_nScreenWidth;
extern int				g_nScreenHeight;

BOOL		PopEvent( char* szUIName, UIEVENT* pEvent, DWORD* pdwParam1, DWORD* pdwParam2 );
BOOL		PushEvent( const char* szCtrlName, UIEVENT nEvent, DWORD dwParam1, DWORD dwParam2 );
BOOL		IsDoubleByteCharset( const char* pBuff );

#endif // #ifndef __UInterface_Export_Header__
