/**
 * Wind3D 예외 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-03-13 5:54p $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /Common/Inc/Exception.h $
 * 
 * 6     03-03-13 5:54p Ahastudio
 * comment 정리.
 * <HR>
 *
 * @file	Exception.h
 */


#ifndef __Wind3D_Common_Exception_Header__
#define __Wind3D_Common_Exception_Header__


/**
 * 예외.
 * @ingroup		Common
 */
class COMMON_API W3DException
{
public:
	W3DException( const char* fmt = "exception", ... );

	/**
	 * 메세지를 얻는다.
	 */
	const char*		GetMessage()
	{
		return m_szMessage;
	} //GetMessage

private:
	char		m_szMessage[256];		///< 에러 메세지
}; //class W3DException


#endif // #ifndef __Wind3D_Common_Exception_Header__
