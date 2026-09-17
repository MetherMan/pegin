/**
 * Wind3D 공통 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-12-08 2:29p $
 * @version	$Revision: 47 $
 *
 * <HR>
 * $Log: /Common/Inc/Common.h $
 * 
 * 47    04-12-08 2:29p Sk8snow2
 * 
 * 46    04-12-08 3:06a Sk8snow2
 * 
 * 45    04-07-08 9:59p Servantes
 * 
 * 44    04-05-29 10:05p Paran
 * 
 * 43    04-01-30 9:24p Kjmgo
 * 
 * 42    04-01-29 10:09a Kjmgo
 * 
 * 41    04-01-29 9:55a Kjmgo
 * 
 * 40    04-01-07 5:54p Sk8snow2
 * 
 * 39    04-01-03 9:45 Sckyj
 * 
 * 38    04-01-02 2:18p Paran
 * 스크링 테이블 추가
 * 
 * 37    03-12-22 10:31p Kjmgo
 * 
 * 36    03-12-09 4:00p Paran
 * wtm 파일 포맷 클래스 추가
 * 
 * 35    03-10-17 1:58p Kjmgo
 * 보간 함수 추가
 * 
 * 34    03-10-02 4:33a Kjmgo
 * AABB추가
 * 
 * 33    03-08-11 6:10p Admin
 * 
 * 32    03-07-25 12:07a Paran
 * 
 * 31    03-07-24 7:20p Paran
 * 
 * 30    03-07-24 7:17p Paran
 * 
 * 29    03-06-28 5:25p Kjmgo
 * 
 * 28    03-06-28 5:14p Kjmgo
 * 
 * 27    03-06-28 5:00p Kjmgo
 * 
 * 26    03-04-25 7:21p Ahastudio
 *
 * 17    03-03-14 5:45a Ahastudio		Assert 중복 정의 제거.
 *
 * 14    03-03-13 3:54p Ahastudio		Archive 추가.
 *
 * 13    03-02-28 2:50p Ahastudio		Singleton 추가.
 * <HR>
 *
 * @file	Common.h
 */


#ifndef __Wind3D_Common_Header__
#define __Wind3D_Common_Header__


#pragma warning( disable : 4786 )
#pragma warning( disable : 4018 )			// signed, unsigned missmatch disable
#pragma warning( disable : 4244 )			// signed, unsigned missmatch disable
#pragma warning( disable : 4183 )			// 

#include <windows.h>


#define DLL_IMPORT	__declspec( dllimport )
#define DLL_EXPORT	__declspec( dllexport )


#ifdef COMMON_EXPORT
#define COMMON_API	DLL_EXPORT
#else
#define COMMON_API	DLL_IMPORT
#endif // #ifdef COMMON_EXPORT


#define SAFE_DELETE( p )		{ if( p ) { delete (p);		(p) = NULL; } }
#define SAFE_DELETE_ARRAY( p )	{ if( p ) { delete [] (p);	(p) = NULL; } }
#define SAFE_RELEASE( p )		{ if( p ) { (p)->Release();	(p) = NULL; } }
#define SAFE_FREE( p )			{ if( p ) { _MemFree( p );	(p) = NULL; } }


typedef char			int8;
typedef short			int16;
typedef long			int32;

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned long	uint32;

#include "Exception.h"

class	IComponent;
struct	ComponentInfo;

struct	Vector2;
struct	Vector3;
struct	Matrix4;
struct	Quaternion;
struct	Plane;
struct	Triangle;
struct	Frustum;
struct  AABB;
class	CCriticalSection;
class	CLocker;
class	IThread;



#ifdef _DEBUG
#define FileCheck( szFileName )\
{\
	int i=0;\
}
#else
#define FileCheck( szFileName )\
{\
int j=0;\
}
#endif

#ifdef _DEBUG
#ifndef _NOASSERT
#define Assert( exp )			\
	{							\
		if( !(exp) )			\
		{						\
			__asm	int		3h	\
		}						\
	}
#else
#define Assert( exp )
#endif
#else
#define Assert( exp )			\
	{							\
		if( !(exp) )			\
		{						\
		    Log( __FILE__, __LINE__, #exp );	\
		}						\
	}		

#endif // #ifdef _DEBUG

#ifdef _DEBUG
#define guard( func )	\
	{					\
		static const TCHAR __FUNC_NAME__[] = TEXT( #func );		\
		try				\
		{
#else
#define guard( func )	
#endif // #ifdef _DEBUG

#ifdef _DEBUG
#define unguard							\
		}								\
		catch( char* e )				\
		{								\
			DEBUG_OUT( "Exception( " + __FUNC_NAME__ + " ) - " + e );	\
			Assert( !__FUNC_NAME__ );	\
		}								\
		catch( ... )					\
		{								\
			DEBUG_OUT( "Exception( " + __FUNC_NAME__ + " )" );	\
			Assert( !__FUNC_NAME__ );	\
		}								\
	}
#else
#define unguard							
#endif // #ifdef _DEBUG


#ifdef _DEBUG
#define CHECK_PTR_RETURN( ptr )	\
	{							\
		if( NULL == (ptr) )		\
		{						\
			Assert( 0 );		\
			return FALSE;		\
		}						\
	}
#else
#define CHECK_PTR_RETURN( ptr )		{	if( NULL == (ptr) )		return FALSE;	}
#endif // #ifdef _DEBUG

#define COLOR_ARGB(a,r,g,b)  (((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define COLOR_RGBA(r,g,b,a)  COLOR_ARGB( a, r, g, b )


#include "Log.h"
#include "Singleton.h"
#include "DebugHelper.h"
#include "Memory.h"
#include "Archive.h"
#include "Component.h"

#include "Math.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Plane.h"
#include "Triangle.h"
#include "Frustum.h"
#include "AABB.h"
#include "WTM.h"
#include "Package.h" 
#include "CriticalSection.h"
#include "Thread.h"
#include "Handle.h"

#ifndef _CHINAVERSION
	#include "stringtable.h"
#else
	#include "StringTable_c.h"
#endif


#endif // #ifndef __Wind3D_Common_Header__
