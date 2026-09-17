/**
 * 금칙 단어
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-02-03 6:07p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /UInterface/Src/NotUseWord.h $
 * 
 * 2     04-02-03 6:07p Paran
 * 
 * <HR>
 *
 * @file	NotUseWord.h
 */

#ifndef __NotUseWord_Header__
#define __NotUseWord_Header__

#include <windows.h>

/**
 * 단어가 금칙단어인지를 알아 봅니다.
 * 금칙단어이면 FALSE를 리턴 합니다.
 */
BOOL CheckWord( const char* szWord );
BOOL CheckUseID( const char* szID ); 

/**
 *

class CNotUseWords
{
public:
	MEMORY_UNDER_MGR( CNotUseWords );

private:
	typedef std::map<std::string, std::string>

}; //class CNotUseWords

 */


#endif //#ifndef __NotUseWord_Header__
