/**
 * 로그.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-10-10 1:59p $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /Common/Inc/Log.h $
 * 
 * 6     03-10-10 1:59p Kjmgo
 * Log 함수 하나 추가
 * 
 * 5     03-06-28 8:15p Kjmgo
 * 
 * 4     03-06-28 6:26p Kjmgo
 * 
 * 3     03-06-28 6:22p Kjmgo
 * 
 * 2     03-06-28 5:25p Kjmgo
 * 
 * 1     03-06-28 5:21p Kjmgo
 * 
 * 2     03-06-28 4:59p Kjmgo
 * 
 * <HR>
 *
 * @file	Log.cpp
 */


#ifndef __Wind3D_Common_Log_Header__
#define __Wind3D_Common_Log_Header__

#include <stdio.h>
#include <time.h>

/**
 * Log
 */
inline void Log( char* strFile, unsigned uLine, char *buf, ... )
{
	char tmp_buf[1024], tmp_buf2[1024]; 
	va_list args; 
	FILE *fp; 

	time_t tval; 
	time( &tval ); 
	struct tm *currTM = localtime( &tval ); 

	//-=-=-=-= File -=-=-=-=//
	fp = fopen( "errlog.txt", "a" ); 

	if( !fp ) 
	{
			return; 
	} //if

	va_start( args, buf ); 
	vsprintf( tmp_buf, buf, args ); 
	va_end( args ); 

	sprintf( tmp_buf2, "[%s/%d][%d/%02d/%02d %02d:%02d:%02d] %s\n", strFile, uLine, currTM->tm_year+1900, 
			currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec, tmp_buf ); 

	strcpy( tmp_buf, tmp_buf2 ); 

	fprintf( fp, tmp_buf ); 

	//-=-=-=-= Close -=-=-=-=-=//

	fclose( fp ); 
} //Log

/**
 * Log
 */
inline void Log( char *buf, ... )
{
	char tmp_buf[1024], tmp_buf2[1024]; 
	va_list args; 
	FILE *fp; 

	time_t tval; 
	time( &tval ); 
	struct tm *currTM = localtime( &tval ); 

	//-=-=-=-= File -=-=-=-=//
	fp = fopen( "errlog.txt", "a" ); 

	if( !fp ) 
	{
			return; 
	} //if

	va_start( args, buf ); 
	vsprintf( tmp_buf, buf, args ); 
	va_end( args ); 

	sprintf( tmp_buf2, "[%d/%02d/%02d %02d:%02d:%02d] %s\n", currTM->tm_year+1900, 
			currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec, tmp_buf ); 

	strcpy( tmp_buf, tmp_buf2 ); 

	fprintf( fp, tmp_buf ); 

	//-=-=-=-= Close -=-=-=-=-=//

	fclose( fp ); 
} //Log

#endif

