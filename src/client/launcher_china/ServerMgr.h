/////////////////////////////////////////////////////
//
// ServerMgr.h
// Created By : sckyj
//
/////////////////////////////////////////////////////

#ifndef __ServerMgr_Header__
#define __ServerMgr_Header__

#include <map>

struct SServer;
struct SArea;
struct SPage;

typedef std::map<int, SServer*>		ServerMap;
typedef std::map<int, SArea*>		AreaMap;
typedef std::map<int, SPage*>		PageMap;

struct SServer
{
	int			nServerIndex;
	char		szServerName[40];
	
	SServer()
	{
		Init();
	} // SServer

	void Init()
	{
		nServerIndex = -1;
		ZeroMemory( szServerName, 40 );
	}
};

struct SArea
{
	int			nAreaIndex;
	char		szAreaName[40];

	ServerMap	serverMap;

	SArea()	
	{
		Init();
	} // SArea

	~SArea()
	{
		Clear();
	} // ~SArea

	void Init()
	{
		serverMap.clear();
		nAreaIndex = -1;
		ZeroMemory( szAreaName, 40 );		
	} // Init

	void Clear()
	{
		ServerMap::iterator itor = serverMap.begin();
		ServerMap::iterator itorNext;
		while( itor != serverMap.end() )
		{
			itorNext = itor;
			itorNext++;

			SServer* pServer = itor->second;
			if( pServer )
			{
				delete pServer;
			} // if

			itor = itorNext;
		} // while

		serverMap.clear();
		nAreaIndex = -1;
	} // Clear

	SServer* GetServer( int nIndex )
	{
		ServerMap::iterator itor = serverMap.begin();
		if( itor != serverMap.end() )
		{
			return itor->second;
		} // if

		return NULL;
	} // GetServer
};

struct SPage
{
	int			nPageNum;

	AreaMap		areaMap;

	SPage()
	{
		Init();
	} // SPage

	~SPage()
	{
		Clear();
	} // ~SPage

	void Init()
	{
		areaMap.clear();
		nPageNum = 0;
	} // init

	void Clear()
	{
		AreaMap::iterator itor = areaMap.begin();
		AreaMap::iterator itorNext;
		while( itor != areaMap.end() )
		{
			itorNext = itor;
			itorNext++;

			SArea* pArea = itor->second;
			if( pArea )
			{
				delete pArea;
			} // if

			itor = itorNext;
		} // while

		areaMap.clear();
		nPageNum = 0;
	} // clear
	
	SArea* GetArea( int nIndex )
	{
		AreaMap::iterator itor = areaMap.find( nIndex );
		if( itor != areaMap.end() )
		{
			return itor->second;
		} // if

		return NULL;
	} // GetArea
};

class CServerMgr
{
public :
	CServerMgr();
	~CServerMgr();

	BOOL			Init();
	BOOL			ReadScript( const char* szFileName );

	SArea*			GetArea( int nPage, int nArea );
	int				GetPageCount();
	int				GetAreaCount( int nPage );

private :

	void			ReadArea( FILE* fp, SPage* pPage );
	void			ReadServer( FILE* fp, SPage* pPage, SArea* pArea );	

private :

	PageMap			m_mapPage;
};

#endif // #ifndef __ServerMgr_Header__