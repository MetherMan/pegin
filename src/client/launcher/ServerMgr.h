/////////////////////////////////////////////////////
//
// ServerMgr.h
// Created By : sckyj
//
/////////////////////////////////////////////////////

#ifndef __ServerMgr_Header__
#define __ServerMgr_Header__

//#include <map>
#include "stdafx.h"

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