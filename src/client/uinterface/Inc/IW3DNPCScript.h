/**
 * NPC 스크립트 처리 인터페이스 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-07-23 3:14p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /UInterface/Inc/IW3DNPCScript.h $
 * 
 * 2     03-07-23 3:14p Paran
 * 
 * 1     03-05-17 12:00a Paran
 * 
 * 11    03-05-16 8:09p Paran
 * NPC 스크립트 수정.
 * 
 * 10    03-04-04 5:00a Ahastudio
 * <HR>
 *
 * @file	IW3DNPCScript.h
 */


#ifndef __Wind3D_UInterface_IW3DNPCScript_Header__
#define __Wind3D_UInterface_IW3DNPCScript_Header__


/**
 * NPC 스크립트 처리 인터페이스.
 * @ingroup		Engine
 */
class IW3DNPCScript : public IComponent
{
public:
	/**
	 * 스크립트 파일을 로드합니다.
	 *
	 * @param	szFileName		[IN] 스크립트 파일(e.g. npcinfo.src)
	 *
	 * @return	로드 성공/실패
	 */
	virtual BOOL				Load( const char* szFileName ) = 0;	

	/**
	 * 작업할 NPC를 설정합니다.
	 *
	 * @param	szNPCName		[IN] NPC이름
	 *
	 * @return	설정 성공/실패
	 */
	virtual BOOL				OpenNPC( const WORD wAddr_ ) = 0;

	/**
	 * ........
	 *
	 * @return	......
	 */
	virtual BOOL				FirstTalk( const int nTalkNum  ) = 0;

	/**
	 * ........
	 *
	 * @param	nAnswer		[IN] ........
	 *
	 * @return	......
	 */
	virtual BOOL				NextTalk( int nAnswer = 0 ) = 0;

	/**
	 * 다음 대사가 있는지 여부.
	 */
	virtual BOOL				IsNextTalk( int nAnswer = 0 ) = 0;

	/**
	 * 작업 중인 NPC의 이름을 얻는다.
	 */
	virtual const char*			GetNPCName() = 0;

	/**
	 * 작업 중인 NPC의 직업을 얻는다.
	 */
	virtual const char*			GetNPCJob() = 0;

	/**
	 * 현재 대화 텍스트를 얻는다.
	 */	
	virtual const char*			GetText() = 0;

	/**
	 * 작업 중인 NPC에게의 답 개수를 얻습니다.
	 */
	virtual int					GetAskNum() = 0;

	/**
	 * 작업 중인 NPC에게의 대답 텍스트를 얻는다.
	 *
	 * @param	대답 배열의 인덱스
	 *
	 * @return	대답 텍스트.
	 */
	virtual const char*			GetAskText( int nIndex ) = 0;
#ifdef _dUMENOKOJI_QUEST_
	virtual	int					GetNowTalkNum() = 0;
	virtual	int					GetPreTalkNum() = 0;
	virtual int					SetPreTalkNum() = 0;
	virtual	void				InitPreTalkNum() = 0;
	virtual	BOOL				PreTalk( int nAnswer = 0 ) =0; 
	virtual BOOL				IsPreTalk( int nAnswer = 0 ) = 0;
#endif

}; //class IW3DNPCScript


#endif // #ifndef __Wind3D_UInterface_IW3DNPCScript_Header__
