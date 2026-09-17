// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FFF26A68_E701_431A_B142_951D8301D3BE__INCLUDED_)
#define AFX_STDAFX_H__FFF26A68_E701_431A_B142_951D8301D3BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <map>

struct SServer
{
	int			nServerIndex;
	char		szServerName[40];
	
	void Init()
	{
		nServerIndex = -1;
		ZeroMemory( szServerName, 40 );
	}
};

typedef std::map<int, SServer*>		ServerMap;


struct SArea
{
	int			nAreaIndex;
	char		szAreaName[40];
	
	ServerMap	serverMap;	
	
	void Init()
	{
		nAreaIndex = -1;
		ZeroMemory( szAreaName, 40 );		
	}
};

typedef std::map<int, SArea*>		AreaMap;



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

typedef std::map<int, SPage*>		PageMap;



class IMode
{
public :
	virtual BOOL Init() = 0;
	virtual BOOL Render() = 0;
	virtual	BOOL LDown( POINT pt ) = 0;
	virtual BYTE LUp( POINT pt ) = 0;
	virtual void Clear() = 0;
};

#include "ServerMgr.h"

extern CServerMgr*		g_pServerMgr;
extern int				g_nServerIndex;

#pragma warning( disable : 4786 )


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FFF26A68_E701_431A_B142_951D8301D3BE__INCLUDED_)
