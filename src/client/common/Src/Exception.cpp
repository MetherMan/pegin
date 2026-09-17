/**
 * Wind3D 예외 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-03-13 5:54p $
 * @version	$Revision: 5 $
 *
 * <HR>
 * $Log: /Common/Src/Exception.cpp $
 * 
 * 5     03-03-13 5:54p Ahastudio
 * comment 정리.
 * <HR>
 *
 * @file	Exception.cpp
 */


#define COMMON_EXPORT

#include "../Inc/Common.h"
#include <stdio.h>


/**
 * 생성자.
 *
 * @param	fmt		[IN] 포맷.
 * @param	...		[IN] 인자들.
 */
W3DException::W3DException( const char* fmt, ... )
{
	char	szMsg[256];

	va_list ap;
	va_start( ap, fmt );
	vsprintf( szMsg, fmt, ap );
	va_end( ap );

	strcpy( m_szMessage, szMsg );
} //W3DException::W3DException
