/**
 * Body 모델 관리자.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-02-02 11:43a $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /GameClient/Src/BodyMgr.h $
 * 
 * 4     04-02-02 11:43a Kjmgo
 * 바디 메니저 생성
 * 
 * 3     03-12-24 8:17a Kjmgo
 * 
 * 2     03-12-24 4:01a Kjmgo
 * 캐릭터 모델을 모두 메모리에 넣어 놓는다.
 * 
 * 1     03-12-24 3:12a Kjmgo
 * 
 *  
 * <HR>
 *
 * @file	BodyMgr.h
 */

#ifndef __BodyManager_Header__
#define __BodyManager_Header__

/**
 *
 */
class CBodyManager	:	public IThread
{
public:
	MEMORY_UNDER_MGR( CBodyManager );

	CBodyManager()
	{
		Create();
	} //CBodyManager

	~CBodyManager()
	{
		Cleanup();
		Kill();
	} //~CBodyManager

	/**
	 *
	 */
	void Init()
	{
		return ;
		char szSearchPath[] = "Body/High/*.mod";

		WIN32_FIND_DATA		wfd;
		HANDLE				hFindFile;

		hFindFile = FindFirstFile( szSearchPath, &wfd );

		while( hFindFile )
		{
			if( !g_pScene->BeginFrame() )		return;

			if( wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE )
			{
				std::string		sName = wfd.cFileName;
				IW3DModel* pModel = MAKE_COMPONENT( Model );

				if( !pModel ||
					!pModel->Load( "Body", ( "High/" + sName ).c_str() ) )
				{
					SAFE_RELEASE( pModel );					
				} 
				else 
				{
					m_mapModel.insert( std::make_pair( sName, pModel ) );
				} //if..else..				
			} //if

			g_pScene->EndFrame();

			if( !FindNextFile( hFindFile, &wfd ) )	break;
		} //while

	} //Init

	/**
	 *
	 */
	void Cleanup()
	{
		ModelMap::iterator itor = m_mapModel.begin();
		ModelMap::iterator itorNext;

		while( itor != m_mapModel.end() )
		{
			itorNext = itor;
			itorNext++;

			SAFE_RELEASE( itor->second );
						
			itor = itorNext;
		} //while
	} //Cleanup
 
	/**
	 *
	 */
	void	Load(	IW3DModel** ppModel, 
					const char*		szPath,		///< [IN]
					const string&	sName,		///< [IN]
					const char*		szType		///< [IN]
					)
	{
		IW3DModel* pModel = Find( szType, sName.c_str() );

		if( pModel )
		{
			*ppModel = pModel;
			pModel->AddRef();
			return ;
		} //pModel

		SBlock* pBlock = new SBlock;
		pBlock->ppModel = ppModel;
		sprintf( pBlock->szName, "%s", sName.c_str() );
		sprintf( pBlock->szType, "%s", szType );
		sprintf( pBlock->szPath, "%s", szPath );
		
		Push( pBlock );
	} //Load

	/**
	 *
	 */
	void	Unload(	const char* szType, const string& sName, IW3DModel** ppModel )
	{
		if( *ppModel )
		{
			m_csMap.Lock();
			IW3DModel*	pModel = *ppModel;

			Assert( pModel->GetRefCount() >= 1 );

			if( pModel->GetRefCount() <= 1 )
			{
			/*	string str = szType;
				str += "/";
				str += sName;*/
				ModelMap::iterator	itor = m_mapModel.find( sName );
				if( itor != m_mapModel.end() )
				{
					DEBUG_OUT( "Remove Character Model - " + sName.c_str() );
					m_mapModel.erase( itor );
				}
				else
				{
					Assert( !"모델 리스트에 없을 수가 없다. 내부 로직에 문제 있을 가능성이 큼." );
				} //if..else..
			} //if
			m_csMap.Unlock();
		} //if

		SAFE_RELEASE( *ppModel );
		
	} //Unload

	/**
	 *
	 */
	void	AddModel( IW3DModel* pModel, string sName )
	{
	//	m_mapModel.insert( std::make_pair( sName, pModel ) );
	} //AddModel


	BOOL Process( void* agr );
	

private:
	struct	SBlock
	{
		IW3DModel**		ppModel;
		char			szPath[128];
		char			szType[128];
		char			szName[128];
	}; //struct SBlock

	typedef map<string, IW3DModel*>	ModelMap;
	ModelMap	m_mapModel;		///< 캐릭터 모델

	typedef std::vector<SBlock*>		ModelArray;
	ModelArray	m_aModel;
	
	CCriticalSection	m_cs;
	CCriticalSection	m_csMap;

	/**
	 *
	 */
	void Push( SBlock* pBlock )
	{
		m_cs.Lock();
			m_aModel.push_back( pBlock );
		m_cs.Unlock();
	} //Push

	/**
	 *
	 */
	SBlock*	Pop()
	{
		CLocker locker( m_cs );

		if( m_aModel.empty() )
		{
			return NULL;
		} //if

		SBlock* p = NULL;
		//m_cs.Lock();
			ModelArray::iterator itor = m_aModel.begin();
			p = *itor;
			m_aModel.erase( itor );
		//m_cs.Unlock();

		return p;
	} //Pop

	/**
	 *
	 */
	void Load( SBlock* pBlock );

	/**
	 *
	 */
	IW3DModel* Find( const char* szType, const char* szName )
	{
		IW3DModel* pModel = NULL;

	//	string sName = szType;
	//	sName += "/";
	//	sName += szName;
		string sName = szName;

		m_csMap.Lock();
			ModelMap::iterator itor = m_mapModel.find( sName );
			if( itor != m_mapModel.end() )
			{
				pModel = itor->second;				
			} //if
		m_csMap.Unlock();

		return pModel;
	} //Find

}; //class CBodyManager

extern CBodyManager		g_BodyMgr;


#endif //#ifndef __BodyManager_Header__

/*	char szSearchPath[] = "Object/*.mod";

	WIN32_FIND_DATA		wfd;
	HANDLE				hFindFile;

	hFindFile = FindFirstFile( szSearchPath, &wfd );

	while( hFindFile )
	{
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE )
		{
			std::string		sName = wfd.cFileName;
			m_dlgModelList.m_listModel.AddString( sName.c_str() );
		} //if

		if( !FindNextFile( hFindFile, &wfd ) )	break;
	} //while*/