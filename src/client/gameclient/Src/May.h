/**
 * 메이
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-05-28 2:45p $
 * @version	$Revision: 18 $
 *
 * <HR>
 * $Log: /GameClient/Src/May.h $
 * 
 * 18    04-05-28 2:45p Sckyj
 * 
 * 17    04-03-25 11:41a Paran2
 * 
 * 16    04-03-24 6:45p Paran2
 * 
 * 13    03-12-19 8:55a Kjmgo			MAY(땡삐) 기능 업그레이드및, 공격시작 패킷을 보낸후, 일정시간동안 시작하라는 패킷을 받지 못하면 다시 보내는 코드 추가
 * 
 * 12    03-12-18 10:04p Kjmgo			함축 단어, Map 추가
 * 
 * 9     03-12-17 12:01a Kjmgo			Command 추가( Command 追加 )
 * 
 * 8     03-12-11 2:40a Kjmgo			3단계로 분리 추가 
 * 
 * 7     03-12-10 11:52p Kjmgo			레벨을 3단계로 나눔
 * 
 * 5     03-11-16 1:30a Kjmgo			May 관련 코드 추가
 * 
 * 4     03-11-12 8:36p Kjmgo			이동속도및, 대화 수정
 * 
 * 3     03-10-13 9:33a Kjmgo			Talk관련 함수 추가
 * 
 * 2     03-10-06 9:38p Kjmgo			May(땡삐) 1단계 완성
 * 
 * <HR>
 *
 * @file	May.h
 */

#ifndef __May_Header__
#define __May_Header__


#define MAY_MAX_STATE		6
#define MAY_MAX_SAY			30
#define MAY_STATE_STOP		0
#define MAY_STATE_MOVE		1
#define MAY_STATE_ATTACK	2
#define MAY_STATE_DEAD		3
#define MAY_STATE_START		4

#define MAY_MAX_TALK_TYPE	20

/////////////////////////////////

/**
 *
 */
class CMay : public CHelper
{ 
public:
	MEMORY_UNDER_MGR( CMay );

	CMay();
	~CMay();

	void		Talk( const char* szTalk );
	void		OpenBox();

	////////////////////////////////////////////////
	// STATE
	////////////////////////////////////////////////
	void		SetState( int nState );

	void		Attack()					{ Talk( MAY_ATTACK ); }
	void		Dead()						{ Talk( MAY_DEAD ); }
	void		LevelUp( WORD wLevel )		{ Talk( MAY_LEVELUP ); }
	void		DealBegin()					{ Talk( MAY_DEAL_BEGIN ); }
	void		Party( BYTE btType )		{ Talk( MAY_PARTY_CREATE + btType ); }
	void		Caution( BYTE btCaution )	{}
	void		AttackMonster( const char* szName )
	{
		int nRandom = random( 10 );

		if( nRandom >= 8 )
		{		
		/*
			TODO : 전투중일경우 몬스터 이름이 인자로 넘어오는데.
				   Talk()을 호출하게 되면.. "무슨 말인지 모르겠는데..."라는 말만
				   화면에 찍고 있다. --> 중국어 버젼에서는 의미없는 깨진 글자가 나와버림..-_-++
				   어떤 의도에서 이부분을 Talk()로 처리했을지 알수 없어 일단 주석 처리..
				   정만씨.. 머땜에 Talk()이라고 했데유?? -_-;;
		*/
		//	Talk( szName );
		} //if
	} //AttackMonster

	BOOL		SetInfo( const WORD wType, WORD wLevel, BOOL bPlayer );

	BOOL		Update( DWORD dwTick );
	void		Draw();

private:
	IW3DRectangle*	m_pRectangle;			///< 사각형.
	IW3DBillboard*	m_pBillboard;			///< 빌보드

	char			m_szTalk[256];			///< 메이가 하는 말

	struct	STalk
	{
		DWORD	dwTime;
		char	szSay[256];
	}; //struct STalk

	struct	STalkState
	{
		int		nState;
		int		nCount;
		STalk	aTalk[MAY_MAX_SAY];

		STalkState()
		{
			nCount = 0;
		} //STalkState
	}; //struct STalkState

	STalkState		m_aTalk[MAY_MAX_TALK_TYPE][MAY_MAX_STATE];

	typedef std::map<std::string, DWORD>		SayMap;
	typedef std::vector<std::string>			SayVector;
	typedef std::vector<SayVector>				SayArray;
	typedef std::map<std::string, std::string>	DefineSay;

	DefineSay		m_mapDefineSay;
	SayMap			m_mapSay;
	SayArray		m_aSay;

private:

	void			DrawTalkText();

	void			MayAttack();

	void			Say( char *buf, ... );
	void			UpdateSay();
	void			LoadTalkScript( const char* szFile );
	void			LoadTalkType( FILE* fp, int nLevel, int nType );
	void			LoadTalkSay( FILE* fp );
	void			LoadDefineSay( FILE* fp );

	int				GetState( char* szState );
	int				TransformState();
	void			Talk( WORD wState );
	WORD			GetLevel( WORD wLevel );
	BOOL			CheckCommand( const char* szSay );

	void			GoAway();
	const char*		GetDefineSay( const char* szTalk );
	void			GetRowDefineSay( const char* szKey, const char* szRow );
}; //class CMay

#endif //#ifndef __May_Header__

