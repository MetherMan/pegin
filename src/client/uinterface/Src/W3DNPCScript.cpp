/**
 * NPC 스크립트 처리 구현부.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-09-17 5:08p $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /UInterface/Src/W3DNPCScript.cpp $
 * 
 * 4     03-09-17 5:08p Paran
 * 
 * 3     03-07-25 12:05a Paran
 * 
 * 2     03-07-23 3:14p Paran
 * 
 * 11    03-05-16 8:09p Paran			NPC 스크립트 수정.
 * 
 * 10    03-03-09 9:04p Ahastudio		STL 사용법 변경.
 * 
 * <HR>
 *
 * @file	W3DNPCScript.cpp
 */


#include "UInterface_Export.h"
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <map>


using namespace std;


struct	NPCFuncHead;
struct	NPCFunc;
struct	NPCHead;
struct	NPCInfo;


typedef map<BYTE, NPCFunc*>				MAP_BYTENPCFUNC;		///< 함수들
typedef map<string, MAP_BYTENPCFUNC*>	MAP_STRMAP;				///< 함수 종류들...

/**
 * NPC 스크립트 처리 구현부.
 * @ingroup	Engine
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
class CW3DNPCScript : public IW3DNPCScript
{
public:
	UNDER_MEMORY_MGR( W3DNPCScript )

	CW3DNPCScript();
	~CW3DNPCScript();

	BOOL				Load( const char* szFileName );
	BOOL				OpenNPC( const WORD wAddr_ );
	
	void				Cleanup();

	BOOL				FirstTalk( const int nTalkNum );
	BOOL				NextTalk( int nAnswer = 0 );
	BOOL				IsNextTalk( int nAnswer = 0 );
	NPCFunc*			GetNextTalk( int nAnswer_ );

#ifdef _dUMENOKOJI_QUEST_
	BOOL				IsPreTalk( int nAnswer );
	NPCFunc*			GetPreTalk( int nAnswer );
	BOOL				PreTalk( int nAnswer );
	int					GetNowTalkNum( );
	int					GetPreTalkNum( );
	int					SetPreTalkNum( );
	void				InitPreTalkNum( );
#endif

	const char*			GetText();
	int					GetAskNum();
	const char*			GetAskText( int nIndex );


	const char*			GetNPCName();
	const char*			GetNPCJob();

	typedef map<WORD, NPCInfo*>				MAP_NPCINFO;				///< NPC 맵
private:
	MAP_NPCINFO			m_mapNPCInfos;			///< NPC 정보 맵
	NPCInfo*			m_pCurrentNPCInfo;		///< 현재 NPC 정보

#ifdef _dUMENOKOJI_QUEST_
	int					m_PreTalk;
#endif

	BOOL				ReadNPCHead( char** szData, NPCHead* pNPCHead );
	BOOL				ReadNPCPosition( char** szData, Vector3* pvPos );
	BOOL				ReadNPCName( char** szData, char* pName );
	BOOL				ReadNPCJob( char** szData, char* szJob );
	BOOL				ReadNPCFunc( char** szData, NPCFunc* pFunc );
	BOOL				ReadNPCGoSubFunc( char** szData, NPCFuncHead* pFuncHead );

	void				GetWord( char** szData, char* szWord );
	void				GetString( char** szData, char* szString );
	BOOL				IsComment( const char* szData );
	BOOL				IsComma( char chData );
	BOOL				IsSkipChar( char chData );
	BOOL				IsBeginofLine( char chData );
	BOOL				IsEndofLine( char chData );
	BOOL				IsQuote( char chData );

	BOOL				AddFunction( NPCInfo* pNPCInfo, NPCFunc* pFunc );
	NPCInfo*			GetNPCInfo( const WORD wAddr_ );
	NPCFunc*			GetNPCFunc( NPCFuncHead* pFuncHead );
};


IMPLEMENT_COMPONENT( CW3DNPCScript, NPCScript );


//
// struct NPCFuncHead
//
struct NPCFuncHead
{
	string					strFunc;
	BYTE					byAddr;
	string					strText;
};

//
// struct NPCFunc
//
struct NPCFunc
{
	NPCFuncHead					FuncHead;
	vector<NPCFuncHead*>		vSubFuncHead;

	~NPCFunc()
	{
		vector<NPCFuncHead*>::iterator		iterVec_Head;
		for( iterVec_Head = vSubFuncHead.begin(); 
			 iterVec_Head != vSubFuncHead.end(); 
			 iterVec_Head++ )
		{
			delete *iterVec_Head;
		}
		vSubFuncHead.clear();
	}
};

//
// struct NPCHead
//
struct NPCHead
{
	WORD							wAddr;
	string							strName;
};

//
// struct NPCInfo
//
struct NPCInfo
{
	NPCHead							head;
	string							strNPCJob;
	MAP_STRMAP						mapFuncMgr;				///< 함수그룹
	NPCFuncHead						*pBeginFuncHead;
	NPCFuncHead						*pCurrentFuncHead;

	NPCInfo()
	{
		pBeginFuncHead		= NULL;
		pCurrentFuncHead	= NULL;
	}

	~NPCInfo()
	{
		NPCFunc*					pFunc;
		MAP_BYTENPCFUNC*			pmapFunc;
		MAP_BYTENPCFUNC::iterator	iterMap_Func;
		MAP_STRMAP::iterator		iterMap_FuncMgr;

		for( iterMap_FuncMgr = mapFuncMgr.begin(); 
			 iterMap_FuncMgr != mapFuncMgr.end(); 
			 iterMap_FuncMgr++ )
		{
			pmapFunc = iterMap_FuncMgr->second;
			for( iterMap_Func = pmapFunc->begin(); 
				 iterMap_Func != pmapFunc->end(); 
				 iterMap_Func++ )
			{
				pFunc = iterMap_Func->second;
				delete pFunc;
			}
			pmapFunc->clear();
		}
		mapFuncMgr.clear();
	}
};


/**
 * 생성자.
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
CW3DNPCScript::CW3DNPCScript()
{
	m_pCurrentNPCInfo	= NULL;
}

/**
 * 소멸자.
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
CW3DNPCScript::~CW3DNPCScript()
{
	Cleanup();
}

/**
 * 클린업.
 */
void CW3DNPCScript::Cleanup()
{
	MAP_NPCINFO::iterator	itor = m_mapNPCInfos.begin();
	MAP_NPCINFO::iterator	itorNext;

	while( itor != m_mapNPCInfos.end() )
	{
		itorNext = itor;
		itorNext++;

		NPCInfo*	pNPCInfo = itor->second;
		SAFE_DELETE( pNPCInfo );

		m_mapNPCInfos.erase( itor );

		itor = itorNext;
	} //while	
} //CW3DNPCScript::Cleanup

/**
 * 스크립트 파일을 로드한다.
 *
 * @param	szFileName		[IN] 스크립트 파일 (e.g. npcinfo.src)
 * @return	로드 성공/실패
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::Load( const char* szFileName )
{
	Assert( szFileName );

	Cleanup();

	ifstream		scrfile( szFileName );
	string			strLine;
	BOOL			bHeadBegin = FALSE;
	BOOL			bFail = TRUE;
	char*			pData;
	char			pBuff[256];
	NPCInfo*		pNpcInfo;
	NPCFunc*		pFunc;

	if( scrfile.is_open() )
	{
		while( 1 )
		{
			getline( scrfile, strLine );
			if( scrfile.fail() )
			{
				break;
			}

			pData = (char*)strLine.c_str();
			for( char *pCurr = pData; pCurr < pData + strlen( pData ); pCurr++ )
			{
				if( IsComment( pCurr ) )									// 주석이면 다음 라인으로 넘어간다.
				{
					break;
				}
				else if( IsSkipChar( *pCurr ) )
				{
					continue;
				}
				
				if( IsBeginofLine( *pCurr ) )
				{
					bHeadBegin = TRUE;
					continue;
				}

				if( bHeadBegin )
				{
					bHeadBegin = FALSE;

					GetWord( &pCurr, pBuff );

					if( strcmp( pBuff, "NPC" ) == 0 )
					{
						pNpcInfo = new NPCInfo;
						if( !pNpcInfo )
						{
							Assert( !"메모리 할당 실패" );
							bFail = FALSE;
							goto Failed;
						}

						if( !ReadNPCHead( &pCurr, &pNpcInfo->head ) )
						{
							bFail = FALSE;
							goto Failed;
						}
					}
					else if( strcmp( pBuff, "JOB" ) == 0 )
					{
						if( !ReadNPCJob( &pCurr, pBuff ) )
						{
							bFail = FALSE;
							goto Failed;
						}
						pNpcInfo->strNPCJob = pBuff;
					}
					else if( strcmp( pBuff, "SAY" ) == 0 )
					{
						pFunc = new NPCFunc;
						if( !pFunc )
						{
							Assert( !"메모리 할당 실패" );
							bFail = FALSE;
							goto Failed;
						}

						if( !ReadNPCFunc( &pCurr, pFunc ) )
						{
							delete pFunc;
							bFail = FALSE;
							goto Failed;
						}
						pFunc->FuncHead.strFunc = "SAY";
						if( !AddFunction( pNpcInfo, pFunc ) )
						{
							Assert( !"함수 추가 실패" );
							bFail = FALSE;
							goto Failed;
						}
					}
					else if( strcmp( pBuff, "ASK" ) == 0 )
					{
						pFunc = new NPCFunc;
						if( !pFunc )
						{
							Assert( !"메모리 할당 실패" );
							bFail = FALSE;
							goto Failed;
						}

						if( !ReadNPCFunc( &pCurr, pFunc ) )
						{
							delete pFunc;
							bFail = FALSE;
							goto Failed;
						}
						
						pFunc->FuncHead.strFunc = "ASK";
						if( !AddFunction( pNpcInfo, pFunc ) )
						{
							Assert( !"함수 추가 실패" );
							bFail = FALSE;
							goto Failed;
						}
					}
					else if( strcmp( pBuff, "END" ) == 0 )
					{
						GetWord( &pCurr, pBuff );
						if( pNpcInfo->head.strName == pBuff )
						{
							WORD		wAddr = pNpcInfo->head.wAddr;
							MAP_NPCINFO::iterator	iterMap_NPCInfo = m_mapNPCInfos.find( wAddr );
							if( iterMap_NPCInfo != m_mapNPCInfos.end() )
							{
								DEBUG_OUT( "NPC대사 이미 있습니다 - # " + wAddr );
								continue;
							}
							m_mapNPCInfos.insert( make_pair( pNpcInfo->head.wAddr , pNpcInfo ) );
						}
					}
				}
			}
		}
	}

Failed:
	if( !bFail )
	{
		SAFE_DELETE( pNpcInfo );
	}

	return bFail;
}

/**
 * ...
 *
 * @param	szData		[IN]
 * @param	pNPCHead	[IN]
 * @return	성공 여부
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::ReadNPCHead( char** szData, NPCHead* pNPCHead )
{
	Assert( szData );
	Assert( pNPCHead );

	char		pBuff[128];
	char*		pCurrPtr;
	int			nAddr;
	int			nDataLen = strlen( *szData );

	pCurrPtr = pBuff;
	for( char *pCurr = *szData ; pCurr < *szData + strlen( *szData ) ; pCurr++ )
	{
		if( !IsSkipChar( *pCurr ) )
		{
			if( IsComma( *pCurr ) )
			{
				*pCurrPtr = '\0';
				//lAddr = strtol( pBuff, &pCurrPtr, 16 );
				nAddr = atoi( pBuff );
				pCurrPtr = pBuff;
				continue;
			}

			if( IsEndofLine( *pCurr ) )
			{
				pNPCHead->wAddr = (WORD)nAddr;
				*pCurrPtr = '\0';
				pNPCHead->strName = pBuff;
				*szData = pCurr;
				return TRUE;
			}
			*pCurrPtr = *pCurr;
			pCurrPtr++;
		}

		if( IsComment( pCurr ) )
		{
			return NULL;
		}
	}

	return FALSE;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::ReadNPCPosition( char** szData, Vector3 *pvPos )
{
	Assert( szData );
	Assert( pvPos );

	int			nCnt = 0;
	float		fPos[3];
	char		aBuff[16];
	char*		pCurrPtr;
	int			nDataLen = strlen( *szData );

	pCurrPtr = aBuff;
	for( char* pCurr = *szData ; pCurr < *szData  + strlen( *szData ) ; pCurr++ )
	{
		if( !IsSkipChar( *pCurr ) )
		{
			if( IsComma( *pCurr ) )					// x, y, z값만 받는다. 그 이상 감이 들어오면 에러
			{
				*pCurrPtr = '\0';
				fPos[nCnt] = (float)atof( aBuff );
				nCnt++;
				pCurrPtr = aBuff;
				continue;
			}

			if( IsEndofLine( *pCurr ) )
			{
				*pCurrPtr	= '\0';
				fPos[nCnt]	= (float)atof( aBuff );
				pvPos->x	= fPos[0];
				pvPos->y	= fPos[1];
				pvPos->z	= fPos[2];
				*szData		= pCurr;
				return TRUE;
			}

			*pCurrPtr = *pCurr;
			pCurrPtr++;
		}

		if( IsComment( pCurr ) )
		{
			return NULL;
		}
	}

	return FALSE;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::ReadNPCName( char** szData_, char *pName_ )
{
	Assert( szData_ );
	Assert( pName_ );

	BOOL		bQuote = FALSE;
	char		*pCurrPtr;
	int			nDataLen = strlen( *szData_ );

	pCurrPtr = pName_;
	for( char *pCurr = *szData_; pCurr < *szData_ + strlen( *szData_ ); pCurr++ )
	{
		if( bQuote )
		{
			if( IsQuote( *pCurr ) )
			{
				bQuote = FALSE;
				*pCurrPtr = '\0';
				continue;
			}
			*pCurrPtr = *pCurr;
			pCurrPtr++;
		}
		else
		{
			if( !IsSkipChar( *pCurr ) )
			{
				if( IsQuote( *pCurr ) )
				{
					bQuote = TRUE;
				}

				if( IsEndofLine( *pCurr ) )
				{
					*pCurrPtr = '\0';
					*szData_ = pCurr;
					return TRUE;
				}
			}
			if( IsComment( pCurr ) )
			{
				return FALSE;
			}
		}
	}

	return FALSE;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::ReadNPCJob( char** szData_, char* szJob_ )
{
	Assert( szData_ );
	Assert( szJob_ );

	BOOL		bQuote = FALSE;
	char		*pCurrPtr;
	int			nDataLen = strlen( *szData_ );

	pCurrPtr = szJob_;
	for( char *pCurr = *szData_; pCurr < *szData_ + strlen( *szData_ ); pCurr++ )
	{
		if( bQuote )
		{
			if( IsQuote( *pCurr ) )
			{
				bQuote = FALSE;
				*pCurrPtr = '\0';
				continue;
			}
			*pCurrPtr = *pCurr;
			pCurrPtr++;
		}
		else
		{
			if( !IsSkipChar( *pCurr ) )
			{
				if( IsQuote( *pCurr ) )
				{
					bQuote = TRUE;
				}

				if( IsEndofLine( *pCurr ) )
				{
					*pCurrPtr = '\0';
					*szData_ = pCurr;
					return TRUE;
				}
			}

			if( IsComment( pCurr ) )
			{
				return FALSE;
			}
		}
	}

	return FALSE;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::ReadNPCFunc( char** szData, NPCFunc *pFunc )
{
	Assert( szData );
	Assert( pFunc );

	NPCFuncHead*	pFuncHead	= NULL;
	BOOL			bQuote		= FALSE;
	BOOL			bEnd		= FALSE;
	char			aBuff[256];
	char*			pCurrPtr;
	int				nDataLen = strlen( *szData );
	long			lAddr;

	pCurrPtr = aBuff;
	for( char* pCurr = *szData ; pCurr < *szData + strlen( *szData ) ; pCurr++ )
	{
		if( bQuote )
		{
			if( IsQuote( *pCurr ) )
			{	
				bQuote	 = FALSE;
				*pCurrPtr = '\0';
				pFunc->FuncHead.strText  = aBuff;
				pCurrPtr = aBuff;
				continue;
			}
			*pCurrPtr = *pCurr;
			pCurrPtr++;
		}
		else
		{
			if( !IsSkipChar( *pCurr ) )
			{
				if( IsComma( *pCurr ) )
				{
					*pCurrPtr = '\0';
					lAddr = strtol( aBuff, &pCurrPtr, 16 );
					pFunc->FuncHead.byAddr = (BYTE)lAddr;
					pCurrPtr = aBuff;
					continue;
				}

				if( IsQuote( *pCurr ) )
				{
					bQuote = TRUE;
					continue;
				}

				if( IsBeginofLine( *pCurr ) )
				{
					bEnd = FALSE;
					pFuncHead = new NPCFuncHead;
					CHECK_PTR_RETURN( pFuncHead );

					if( ReadNPCGoSubFunc( &pCurr, pFuncHead ) )
					{
						bEnd = TRUE;
						pFunc->vSubFuncHead.push_back( pFuncHead );
						*szData = pCurr;
					}
					else
					{
						delete pFuncHead;
					}
					continue;
				}

				if( IsEndofLine( *pCurr ) )
				{
					bEnd		= TRUE;
					*pCurrPtr	= '\0';
					*szData		= pCurr;
					continue;
				}

				*pCurrPtr = *pCurr;
				pCurrPtr++;

				if( IsComment( pCurr ) )
				{
					return bEnd;
				}
			}
		}
	}

	return bEnd;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::ReadNPCGoSubFunc( char** szData_, NPCFuncHead *pFuncHead_ )
{
	Assert( szData_ );
	Assert( pFuncHead_ );

	BOOL		bGo = FALSE;
	char		pBuff[32];
	char		*pCurrPtr;
	long		lAddr;

	for( char *pCurr = *szData_; pCurr < *szData_ + strlen( *szData_ ); pCurr++ )
	{
		if( bGo )
		{
			GetWord( &pCurr, pBuff );
			pFuncHead_->strFunc	= pBuff;

			GetWord( &pCurr, pBuff );
			if( IsComma( *(pBuff+strlen( pBuff )-1 ) ))
			{
				*(pBuff+strlen( pBuff )-1) = '\0';
				pCurr--;
			}
			lAddr = strtol( pBuff, &pCurrPtr, 16 );
			pFuncHead_->byAddr	= (BYTE)lAddr;
		}

		if( *pCurr == 'G' && *(pCurr+1) == 'O' )
		{
			bGo			= TRUE;
			pCurr		+= 2;
		}
		
		if( IsComma( *pCurr ) )
		{
			pCurr++;
			GetString( &pCurr, pBuff );

			pFuncHead_->strText = pBuff;
		}

		if( IsEndofLine( *pCurr ) )
		{
			*szData_ = pCurr;
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
void CW3DNPCScript::GetWord( char** szData_, char* szWord_ )
{
	Assert( szData_ );
	Assert( szWord_ );

	BOOL		bBegin = FALSE;
	char		*pCurrPtr;
	int			nDataLen = strlen( *szData_ );

	pCurrPtr = szWord_;
	for( char *pCurr = *szData_; pCurr < *szData_ + strlen( *szData_ ); pCurr++ )
	{
		if( !IsSkipChar( *pCurr ) && !IsEndofLine( *pCurr ) )
		{
			bBegin = TRUE;
		}
		else if( bBegin )
		{	
			bBegin = FALSE; 
			*pCurrPtr = '\0';
			*szData_ = pCurr;
			break;
		}

		if( bBegin )
		{
			*pCurrPtr = *pCurr;
			pCurrPtr++;
		}
	}
}

void CW3DNPCScript::GetString( char** szData_, char* szString_ )
{
	BOOL		bQuote = FALSE;
	char		*pCurrPtr;
	int			nDataLen = strlen( *szData_ );

	pCurrPtr = szString_;
	for( char *pCurr = *szData_; pCurr < *szData_ + strlen( *szData_ ); pCurr++ )
	{
		if( bQuote )
		{
			if( IsQuote( *pCurr ) )
			{
				bQuote = FALSE;
				*pCurrPtr = '\0';
				continue;
			}
			*pCurrPtr = *pCurr;
			pCurrPtr++;
		}
		else
		{
			if( !IsSkipChar( *pCurr ) )
			{
				if( IsQuote( *pCurr ) )
				{
					bQuote = TRUE;
				}

				if( IsEndofLine( *pCurr ) )
				{
					*pCurrPtr = '\0';
					*szData_ = pCurr;
					return;
				}
			}
			if( IsComment( pCurr ) )
			{
				return;
			}
		}
	}
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::IsComment( const char* szData_ )
{
	if( szData_[0] == '/' && szData_[1] == '/' )
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::IsComma( char chData_ )
{
	if( chData_ == ',' )
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::IsSkipChar( char chData_ )
{
	if( chData_ == 0x20 || chData_ == 0x09 )		// Space, Tab
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::IsBeginofLine( char chData_ )
{
	if( chData_ == '[' )
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::IsEndofLine( char chData_ )
{
	if( chData_ == ']' )
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::IsQuote( char chData_ )
{
	if( chData_ == 0x22 )				// "
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::AddFunction( NPCInfo *pNPCInfo_, NPCFunc* pFunc_ )
{
	MAP_BYTENPCFUNC				*pmapFunc = NULL;
	MAP_STRMAP::iterator		iterMap_FuncMgr;
	BOOL						bBeginFunc = FALSE;

	if( pNPCInfo_->mapFuncMgr.empty() )
	{
		bBeginFunc = TRUE;
	}

	iterMap_FuncMgr = pNPCInfo_->mapFuncMgr.find( pFunc_->FuncHead.strFunc );
	if( iterMap_FuncMgr == pNPCInfo_->mapFuncMgr.end() )
	{
		pmapFunc = new MAP_BYTENPCFUNC;
		if( !pmapFunc )
		{
			Assert( !"메모리할당 실패" );
			return FALSE;
		}
		pNPCInfo_->mapFuncMgr.insert( make_pair( pFunc_->FuncHead.strFunc, pmapFunc ) );
	}
	else
	{
		pmapFunc = iterMap_FuncMgr->second;
	}
	pmapFunc->insert( make_pair( pFunc_->FuncHead.byAddr, pFunc_ ) );
	if( bBeginFunc )
	{
		pNPCInfo_->pBeginFuncHead = &pFunc_->FuncHead;
	}

	return TRUE;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
NPCInfo* CW3DNPCScript::GetNPCInfo( const WORD wAddr_ )
{
	MAP_NPCINFO::iterator		iterMap_NPCInfo;
	iterMap_NPCInfo = m_mapNPCInfos.find( wAddr_ );

	if( iterMap_NPCInfo != m_mapNPCInfos.end() )
	{
		NPCInfo		*pNPCInfo = iterMap_NPCInfo->second;
		return pNPCInfo;
	}
	return NULL;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::OpenNPC( const WORD wAddr_ )
{
	NPCInfo*	pNPCInfo = GetNPCInfo( wAddr_ );
	if( !pNPCInfo )
	{
		return FALSE;
	}
	m_pCurrentNPCInfo = pNPCInfo;
	return TRUE;
} //CW3DNPCScript::OpenNPC

/**
 * 현재 대화 텍스트
 *
 * @return	TEXT
 *
 * @author	파란~*
 * @date	2003-02-12 오후 5:56
 */
const char*	CW3DNPCScript::GetText()
{
	if( !m_pCurrentNPCInfo )
	{
		return NULL;
	}

	return m_pCurrentNPCInfo->pCurrentFuncHead->strText.c_str();
}

/**
 * 작업 중인 NPC의 이름을 얻습니다.
 *
 * @return	NPC 이름
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
const char* CW3DNPCScript::GetNPCName()
{
	if( !m_pCurrentNPCInfo )
	{
		return NULL;
	}
	return m_pCurrentNPCInfo->head.strName.c_str();
}

/**
 * 작업 중인 NPC에게의 답 개수를 얻습니다.
 *
 * @return	답의 개수
 *
 * @author	파란~*
 * @date	2003-02-12 오후 9:36
 */
int CW3DNPCScript::GetAskNum()
{
	NPCFunc			*pFunc = GetNPCFunc( m_pCurrentNPCInfo->pCurrentFuncHead );
	if( !pFunc )
	{
		return -1;
	}

	int		nSize = (int)pFunc->vSubFuncHead.size();
	if( nSize <= 1 )
	{
		return 0;
	}

	return nSize;
}

/**
 * 작업 중인 NPC에게의 대답 텍스트
 *
 * @param	대답배열의 인덱스
 *
 * @return	답의 개수
 *
 * @author	파란~*
 * @date	2003-02-12 오후 9:36
 */
const char* CW3DNPCScript::GetAskText( int nIndex )
{
	NPCFunc			*pFunc = GetNPCFunc( m_pCurrentNPCInfo->pCurrentFuncHead );
	if( !pFunc )
	{
		return NULL;
	}

	char		*pText = (char*)pFunc->vSubFuncHead[nIndex]->strText.c_str();
	Assert( pText );
	return pText;
}

/**
 * 작업 중인 NPC의 직업을 얻습니다.
 *
 * @return	NPC 직업
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
const char* CW3DNPCScript::GetNPCJob()
{
	if( !m_pCurrentNPCInfo )
	{
		return NULL;
	}
	return m_pCurrentNPCInfo->strNPCJob.c_str();
}

/**
 * 작업 중인 NPC의 기능을 얻습니다.
 *
 * @return	NPC 기능
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
NPCFunc* CW3DNPCScript::GetNPCFunc( NPCFuncHead* pFuncHead )
{
	Assert( pFuncHead );

	if( pFuncHead == NULL ) 
	{
		return NULL;
	}

	string		strFuncName = pFuncHead->strFunc;
	BYTE		byAddr		= pFuncHead->byAddr;

	if( strFuncName == "END" )
	{
		return NULL;
	}

	MAP_STRMAP::iterator		iterMap_FuncMgr;
	iterMap_FuncMgr = m_pCurrentNPCInfo->mapFuncMgr.find( strFuncName );
	if( iterMap_FuncMgr == m_pCurrentNPCInfo->mapFuncMgr.end() )
	{
		return NULL;
	}

	MAP_BYTENPCFUNC::iterator	iterMap_Func;
	MAP_BYTENPCFUNC				*pMap_Func = NULL;
	pMap_Func = iterMap_FuncMgr->second;
	Assert( pMap_Func );
	iterMap_Func = pMap_Func->find( byAddr );
	if( iterMap_Func == pMap_Func->end() )
	{
		return NULL;
	}

	NPCFunc		*pNPCFunc = iterMap_Func->second;
	Assert( pNPCFunc );
	return pNPCFunc;
}

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::FirstTalk( const int nTalkNum )
{
	if( !m_pCurrentNPCInfo )
	{
		return FALSE;
	}

	NPCFunc*	pNPCFunc;
	if( nTalkNum == 0 )
	{
		m_pCurrentNPCInfo->pCurrentFuncHead = m_pCurrentNPCInfo->pBeginFuncHead;
	}
	else
	{
		NPCFuncHead		FuncHead;
		FuncHead.byAddr		= ( 0x10 * nTalkNum );
		FuncHead.strFunc	= "SAY";
		pNPCFunc = GetNPCFunc( &FuncHead );
		if( !pNPCFunc )
		{
			Assert( !"없는 대사" );
			return FALSE;
		} //if

		m_pCurrentNPCInfo->pCurrentFuncHead = &pNPCFunc->FuncHead;
	} //switch

	return TRUE;
} //CW3DNPCScript::FirstTalk

/**
 * ...
 *
 * @author	파란~*
 * @date	2003-02-07 오후 3:58
 */
BOOL CW3DNPCScript::NextTalk( int nAnswer_ )
{
	NPCFunc		*pNPCFunc = GetNextTalk( nAnswer_ );
	if( !pNPCFunc )
	{
		return FALSE;
	}
	m_pCurrentNPCInfo->pCurrentFuncHead = &pNPCFunc->FuncHead;

	return TRUE;
}

/** 
 * 다음 대화내용을 얻는다.
 */
NPCFunc* CW3DNPCScript::GetNextTalk( int nAnswer_ )
{
	if( !m_pCurrentNPCInfo )
	{
		return NULL;
	}

	NPCFunc		*pNPCFunc = GetNPCFunc( m_pCurrentNPCInfo->pCurrentFuncHead );
	if( !pNPCFunc )
	{
		return NULL;
	}

	if( pNPCFunc->vSubFuncHead.empty() )
	{
		return NULL;
	}

	pNPCFunc = GetNPCFunc( pNPCFunc->vSubFuncHead[nAnswer_] );
	return pNPCFunc;
} //CW3DNPCScript::GetNextTalk


/**
 * 다음 대사가 있는지 여부.
 */
BOOL CW3DNPCScript::IsNextTalk( int nAnswer )
{
	if( GetNextTalk( nAnswer ) )
	{
		return TRUE;
	}
	return FALSE;
} //CW3DNPCScript::IsNextTalk

#ifdef _dUMENOKOJI_QUEST_
void CW3DNPCScript::InitPreTalkNum()
{
	m_PreTalk = 0;
}

int	CW3DNPCScript::GetNowTalkNum()
{
	return m_PreTalk;
}

int CW3DNPCScript::GetPreTalkNum()
{
	if( m_PreTalk > 0 )
		m_PreTalk -= 1;

	else
		m_PreTalk = 0;
	
	return m_PreTalk;
}

int CW3DNPCScript::SetPreTalkNum()
{

	return m_PreTalk += 1;
}

BOOL CW3DNPCScript::PreTalk( int nAnswer )
{
	NPCFunc	*pNPCFunc = GetPreTalk( nAnswer );

	if( !pNPCFunc )
	{
		return FALSE;
	}

	return TRUE;
}

NPCFunc *CW3DNPCScript::GetPreTalk( int nAnswer )
{
	if( !m_pCurrentNPCInfo )
	{
		return NULL;
	}

	NPCFunc *pNPCFunc = GetNPCFunc( m_pCurrentNPCInfo->pCurrentFuncHead );
	
	if( !pNPCFunc )
	{
		return NULL;
	}

	if( pNPCFunc->vSubFuncHead.empty() )
	{
		return NULL;
	}

	pNPCFunc = GetNPCFunc( pNPCFunc->vSubFuncHead[nAnswer] );

	return pNPCFunc;
}

BOOL CW3DNPCScript::IsPreTalk( int nAnswer )
{
	if( GetPreTalk( nAnswer ) ) 
	{
		return TRUE;
	}
	return FALSE;
}

#endif