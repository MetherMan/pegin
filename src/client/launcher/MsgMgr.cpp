#include "stdafx.h"

#include "MsgMgr.h"

CMsgMgr::CMsgMgr()
{
	m_mapMsg.clear();
}

CMsgMgr::~CMsgMgr()
{	
	m_mapMsg.clear();
}

void CMsgMgr::Init()
{	
}

void CMsgMgr::ReadScript( const char* szFileName )
{
	FILE	*fp = fopen( szFileName, "rt" );
	if( fp == NULL )
	{	
		return;
	}

	char szBuffer[256] = "\0";

	int nCount = 0;
	while( fgets( szBuffer, 256, fp ) )
	{
		for( int nCnt = 0; nCnt < 256; nCnt++ )
		{
			if( szBuffer[nCnt] == '\t' ||
				szBuffer[nCnt] == '\r' ||
				szBuffer[nCnt] == '\n' ||
				szBuffer[nCnt] == '\0' )
			{
				szBuffer[nCnt] = '\0';
				m_mapMsg.insert( std::make_pair( nCount, std::string( szBuffer ) ) );		
				nCount++;
				break;
			} //if
		} //for
	} //while

	fclose( fp );
}

const char* CMsgMgr::GetString( int nNum )
{
	MSGMAP::iterator itor = m_mapMsg.find( nNum );
	if( itor != m_mapMsg.end() )
	{
		return itor->second.c_str();
	}
	else return "";
}

int CMsgMgr::GetSize()
{
	return m_mapMsg.size();
}