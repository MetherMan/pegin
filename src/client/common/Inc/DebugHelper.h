/**
 * Wind3D 디버그 정보 출력 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-11-16 6:43p $
 * @version	$Revision: 13 $
 *
 * <HR>
 * $Log: /Common/Inc/DebugHelper.h $
 * 
 * 13    04-11-16 6:43p Sk8snow2
 * 
 * 12    03-04-25 7:21p Ahastudio
 *
 * 11    03-03-06 7:14p Ahastudio	OutputStream 버그 잡음.
 *
 * 10    03-02-26 10:32a Ahastudio	OutputStream에 +로 인지할 수 있는 데이터형 추가.
 *
 * 9     03-02-25 12:47p Ahastudio	OutputStream 추가. DEBUG_OUT 사용할 때  \n을 제거할 것.
 * <HR>
 *
 * @file	DebugHelper.h
 */


#ifndef __Wind3D_Common_DebugHelper_Header__
#define __Wind3D_Common_DebugHelper_Header__


#ifdef _DEBUG
#define DEBUG_OUT( msg )		GlobalDebugHelper.Out( ( GlobalOutputStream.Clear() + msg ).Get() )
#else
#define DEBUG_OUT( msg )		
#endif // #ifdef _DEBUG


/**
 * Debug Helper.
 * @ingroup		Common
 */
class COMMON_API DebugHelper
{
public:
	DebugHelper();
	~DebugHelper();

	void Out( const char* fmt, ... );
}; //class COMMON_API DebugHelper


/**
 * Output Stream 클래스.
 * @ingroup		Common
 */
class COMMON_API OutputStream
{
public:
 	OutputStream();
	~OutputStream();

	/**
	 * 텍스트를 초기화한다.
	 */
	OutputStream&	Clear()
	{
		szText[0] = NULL;
		return *this;
	} //Clear

	/**
	 * 텍스트를 얻는다.
	 *
	 * @return	텍스트.
	 */
	char*	Get()
	{
		return szText;
	} //Get

	OutputStream&	operator + ( const char data );
	OutputStream&	operator + ( const int data );
	OutputStream&	operator + ( const float data );
	OutputStream&	operator + ( const char* data );

	inline OutputStream&	operator + ( const BYTE data )		{ return operator + ( (int)data ); }
	inline OutputStream&	operator + ( const WORD data )		{ return operator + ( (int)data ); }
	inline OutputStream&	operator + ( const DWORD data )		{ return operator + ( (int)data ); }

private:
	char	szText[1024];
}; //class COMMON_API OutputStream


extern COMMON_API DebugHelper	GlobalDebugHelper;
extern COMMON_API OutputStream	GlobalOutputStream;


#endif // #ifndef __Wind3D_Common_DebugHelper_Header__
