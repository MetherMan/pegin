#include "Stdafx.h"
#include "ServerMgr.h"

#pragma warning( disable : 4786 )

CServerMgr::CServerMgr()
{
}

CServerMgr::~CServerMgr()
{
	PageMap::iterator itor = m_mapPage.begin();
	PageMap::iterator itorNext;
	while( itor != m_mapPage.end() )
	{
		itorNext = itor;
		itorNext++;

		SPage* pPage = itor->second;
		if( pPage )
		{
			delete pPage;
		} // if

		itor = itorNext;
	} // while

	m_mapPage.clear();
}

BOOL CServerMgr::Init()
{
	m_mapPage.clear();
	char	szTemp[1024];

	sprintf( szTemp, "%s", "Launcher/server.txt" );

 	if( !ReadScript( szTemp ) )
	{
		return FALSE;
	}

	return TRUE;
}
 
BOOL CServerMgr::ReadScript( const char* szFileName )
{
	FILE* fp = fopen( szFileName, "rt" );
	if( fp == NULL )
	{
		MessageBox( NULL, "DOES NOT EXIST SERVERFILE", "ERROR", 0 );
		return FALSE;
	}

	char		szBuffer[128] = "\0";	
	SPage*		pPage	= NULL;
	SArea*		pArea	= NULL;
	SServer*	pServer	= NULL;

	static	int	nPage		= 1;
	static	int	nAreaNum	= 0;
	static	int nServerNum	= 0;

	while( fscanf( fp, "%s", szBuffer ) != EOF )
	{
		if( !strcmp( szBuffer, "}" ) )
		{
			m_mapPage.insert( std::make_pair( nPage, pPage ) );
			nPage++;
		}
		else if( !strcmp( szBuffer, "PAGE" ) )
		{	
			SPage* pPage = new SPage;
			pPage->nPageNum = nPage;
			nPage++;

			ReadArea( fp, pPage );			
		} // if..else..
	} // while

	fclose( fp );
	return TRUE;
} // CServerMgr::ReadScript
		
void CServerMgr::ReadArea( FILE* fp, SPage* pPage )
{
	char szBuffer[128] = "\0";
	SArea* pArea = NULL;

	static int	nAreaNum = 0;

	while( TRUE )
	{
		fscanf( fp, "%s", szBuffer );
		
		if( !strcmp( szBuffer, "AREA" ) )
		{
			pArea = new SArea;
			pArea->nAreaIndex = nAreaNum;
			nAreaNum++;
			
			fscanf( fp, "%s", szBuffer );
			
			strcpy( pArea->szAreaName, szBuffer );
			
			ReadServer( fp, pPage, pArea );
		}
		else if( !strcmp( szBuffer, "}" ) )
		{
			m_mapPage.insert( std::make_pair( m_mapPage.size(), pPage ) );
			return;
		} // if..else..
	} // while		
}

void CServerMgr::ReadServer( FILE* fp, SPage* pPage, SArea* pArea )
{
	char szBuffer[128] = "\0";
	SServer* pServer = NULL;

	static int	nServerNum = 0;

	while( TRUE )
	{
		fscanf( fp, "%s", szBuffer );
		
		if( !strcmp( szBuffer, "SERVER" ) )
		{
			pServer = new SServer;
			pServer->nServerIndex = nServerNum;
			nServerNum++;
			
			fscanf( fp, "%s", szBuffer );
			
			strcpy( pServer->szServerName, szBuffer );
			
			pArea->serverMap.insert( std::make_pair( pArea->serverMap.size(), pServer ) );							
		}
		else if( !strcmp( szBuffer, "}" ) )
		{
			pPage->areaMap.insert( std::make_pair( pPage->areaMap.size(), pArea ) );
			return;
		} // if..else..
	} // while
} // CServerMgr::ReadServer

SArea* CServerMgr::GetArea( int nPage, int nArea )
{
	PageMap::iterator itor = m_mapPage.find( nPage);
	if( itor != m_mapPage.end() )
	{
		SPage* pPage = itor->second;
		if( pPage )
		{
			return pPage->GetArea( nArea );
		} // if		
	} // if	

	return NULL;
}

int CServerMgr::GetPageCount()
{
	return m_mapPage.size();
} // CServerMgr::GetPageCount

int CServerMgr::GetAreaCount( int nPage )
{
	PageMap::iterator itor = m_mapPage.find( nPage );
	if( itor != m_mapPage.end() )
	{
		SPage* pPage = itor->second;
		if( pPage )
		{
			return pPage->areaMap.size();
		} // if
	} // if

	return 0;
} // CServerMgr::GetAreaCount