/**
 * 검흔에 대한 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /GameClient/Src/Sword.h $
 * 
 * 4     05-02-18 7:38p Sk8snow2
 * 
 * 3     03-12-18 11:02a Kjmgo
 * 
 * 2     03-07-03 7:13a Kjmgo
 * 
 * 1     03-07-02 3:52p Kjmgo
 * 
 * <HR>
 *
 * @file	Sword.h
 */


#ifndef __Sword_Header__
#define __Sword_Header__

#include <string>
#include <map>

using std::map;
using std::string;

/**
 *
 */
struct STrace
{
	IW3DBillboard*	pBillboard;

	string sName;
	string sType;
	Vector3 vP[2];
}; //strucy STrace


/**
 *
 */
class CSwordTrace
{
public:
	UNDER_MEMORY_MGR( CSwordTrace );

	/**
	 *
	 */
	CSwordTrace()
	{

	} //CSwordTrace

	/**
	 *
	 */
	~CSwordTrace()
	{
		TraceMap::iterator itor = m_mapTrace.begin();
		TraceMap::iterator itorNext;

		while( itor != m_mapTrace.end() )
		{
			itorNext = itor;
			itorNext++;

			STrace* pTrace = itor->second;

			if( pTrace )
			{
				SAFE_RELEASE( pTrace->pBillboard );
				SAFE_DELETE( pTrace );
			} //if

			m_mapTrace.erase( itor );
			itor = itorNext;
		} //while

	} //~CSwordTrace

	/**
	 *
	 */
	BOOL Load( const char* szFile )
	{
		FILE* fp = fopen( szFile, "rt" );

		if( NULL == fp )
		{
			Assert( !"Trace Data 파일이 없습니다." );
			return FALSE;
		} //if

		char szBuffer[256];
		char szName[80];
		char szType[80];
		Vector3 vP[2];
		
		while( !feof( fp ) )
		{
			fgets( szBuffer, 256, fp );
			
			STrace* pTrace = new STrace;

			if( sscanf( szBuffer, "%s %s %f %f %f %f %f %f", szName, szType, &vP[0].x, &vP[0].y, &vP[0].z,
														&vP[1].x, &vP[1].y, &vP[1].z ) != 8 )
			{
				Assert( !"sscanf 오류" );
			}


			pTrace->sName = szName;
			pTrace->sType = szType;
			pTrace->vP[0] = vP[0];
			pTrace->vP[1] = vP[1];
			pTrace->pBillboard = NULL;

	//		float fHeight = ( vP[0] - vP[1] ).Length();
	//		pTrace->pBillboard = MAKE_COMPONENT( Billboard );
	//		pTrace->pBillboard->Load( "test.bbs", 0.4F, fHeight );

			m_mapTrace.insert( std::make_pair( pTrace->sName, pTrace ) );
			
		} //while

		fclose( fp );
		return TRUE;
	} //Load

	/**
	 *
	 */
	STrace* LoadTrace( const char* szName )
	{
		TraceMap::iterator itor = m_mapTrace.find( string( szName ) );
		
		if( itor != m_mapTrace.end() )
		{
			// 있다!!!
			Assert( itor->second );
			if( itor->second )
			{	
				return itor->second;
			} //if
		} //if


		return NULL;
	} //LoadTrace

private:
	typedef map<string, STrace*> TraceMap;

	TraceMap	m_mapTrace;
};


#endif
