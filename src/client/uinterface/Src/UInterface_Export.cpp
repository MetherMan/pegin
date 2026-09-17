/**
 * UInterface.Dll 익스포트 설정 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-06-10 5:33p $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /UInterface/Src/UInterface_Export.cpp $
 * 
 * 9     04-06-10 5:33p Paran
 * 
 * 8     04-06-08 3:56p Paran2
 * 
 * 7     04-06-02 11:59a Paran
 * 
 * 6     04-01-19 3:11p Paran
 * 
 * 5     04-01-13 7:37p Kjmgo
 * 
 * 4     03-10-01 4:28p Paran
 * 
 * 3     03-07-25 12:05a Paran
 * 
 * 2     03-05-24 6:38a Ahastudio
 * <HR>
 *
 * @file	UInterface_Export.cpp
 */


#include "UInterface_Export.h"

IW3DScene*		g_pScene;
IW3DUIGraphic*	g_pUIGraphicMgr = NULL;

int				g_nScreenWidth;
int				g_nScreenHeight;

QUEUE_UIEVENT	g_queueEvent;

BOOL		PopEvent( char* szUIName, UIEVENT* pEvent, DWORD* pdwParam1, DWORD* pdwParam2 );
BOOL		PushEvent( const char* szCtrlName, UIEVENT nEvent, DWORD dwParam1, DWORD dwParam2 );

DECLARE_COMPONENT( UInterface )
DECLARE_COMPONENT( NPCScript )

BEGIN_PACKAGE_LIST
	COMPONENT( UInterface )
	COMPONENT( NPCScript )
END_PACKAGE_LIST
