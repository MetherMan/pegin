#include "Render_Export.h"
#include "VertexBuffer.h"
#include "../src/Texture.h"
#include <list>
#include <queue>
#include <map>

#define NUM_ALLOCATE_PARTICLEMEM			1000
#define LENGTH_PARTICLENAME					64

using std::list;
using std::queue;
using std::map;

/**
 * Particle Head
 */
struct SParticleHead
{
	SParticleType*		psParticleType;				///< Particle Type
	Vector3				vStartPos;					///< Start Position
	DWORD				dwLoop;						///< Loop
	DWORD				dwLoopCount;				///< Loop Count
	DWORD				dwPrevTick;					///< Previous Tick
	BOOL				bRun;						///< Run
}; //SParticleHead

/** 
 * One Particle
 */
struct SParticle
{
	SParticleType*			psParticleType;			///< Particle Type
	
	Vector3					vPosition;			///< Particle Position
	Vector3					vStartPos;
	Vector3					vDirection;			///< Direction

	BOOL					bLightMode;			///< Light Mode
	DWORD					dwColor;			///< Color
	float					fSize;				///< Size

	BOOL					bInView;			///< Check Particle In Frustum
	int						nRemainTime;		///< Remain Tick
}; //struct SParticle

/**
 * Class ParticleEx
 */
class CW3DParticleEx : public IW3DParticleEx
{
private:	//struct
	struct VERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
		Vector3			p;
		float			tu, tv;
	}; //struct VERTEX


public:		//construction and destruction
	CW3DParticleEx();
	~CW3DParticleEx();

public:		//operators
	BOOL					Init( const char* szFileName );
	void					Cleanup();

	SParticleType*			LoadParticleType( const char* szLoadName );
	SParticleType*			LoadParticleType( const int nCount, char* szName, const int nNameLength );
	BOOL					SaveParticleType( 
		const char* szName, 
		SParticleType* psPType,
		const char* szTextureName
		);

	void					SetParticleFileName( const char* szFileName );
	int						CheckFileHead( const char* szFileName );
	BOOL					SaveFileHead();
	
	HPARTICLE				AddParticle( const char* szParticleName, Vector3& vPos, DWORD dwLoop = 0 );
	void					DeleteParticle( HPARTICLE hParticle );
	SParticleType*			GetParticleType( HPARTICLE hParticle );
	SParticleType*			FindParticleType( const char* szParticleName );

	void					Run( HPARTICLE hParticle );
	void					Stop( HPARTICLE hParticle );
	void					SetTexture( HPARTICLE hParticle, const char* szFileName );
	
	int						GetOneDataSize();

	void					DrawText( int nX, int nY, DWORD dwColor, const char* szFormat, ... );

	void					AllocateMem();
	SParticle*				GetParticleMem();

	void					Update( const DWORD dwTick );
	void					SetRenderState( const BOOL bLight );
	void					SetWorld( const Vector3& vPos, const float fW );
	void					Render( const DWORD dwTick );

private:	//attribute
	typedef std::map<string, SParticleType*>		MAP_PARTICLETYPE;
	typedef std::list<HPARTICLE>					LIST_HPARTICLEHEAD;
	typedef std::list<SParticle*>					LIST_PARTICLE;
	typedef std::queue<SParticle*>					QUEUE_PATICLE;
	typedef CHandleMgr<SParticleHead, HPARTICLE>	HANDLE_PARTICLEHEAD;

	MAP_PARTICLETYPE			m_mapParticleType;
	LIST_HPARTICLEHEAD			m_listHParticleHead;
	LIST_PARTICLE				m_listParticle;
	QUEUE_PATICLE				m_queParticleMem;
	HANDLE_PARTICLEHEAD			m_hParticleHeadMgr;

	CVertexBuffer*				m_pVB;									///< Vertex Buffer
	SParticle					m_asParticle[NUM_ALLOCATE_PARTICLEMEM];	///< Reserve for Particle Mem
	
	DWORD						m_dwTick;								///< Current Tick
	DWORD						m_dwIntervalTick;						///< Interval Tick

	ID3DXFont*					m_pFont;				///< 폰트

	char						m_szFileName[64];
}; //class CW3DParticleEx

IMPLEMENT_COMPONENT( CW3DParticleEx, ParticleEx );

/**
 * Construction
 */
CW3DParticleEx::CW3DParticleEx()
	: m_pVB( NULL )
{
	m_pFont = NULL;
} //CW3DParticleEx::CW3DParticleEx

/**
 * Destruction
 */
CW3DParticleEx::~CW3DParticleEx()
{
	Cleanup();
	SAFE_DELETE( m_pVB );
	SAFE_RELEASE( m_pFont );
} //CW3DParticleEx::~CW3DParticleEx

/**
 * Cleanup
 */
void CW3DParticleEx::Cleanup()
{
	MAP_PARTICLETYPE::iterator		iter_Type =	m_mapParticleType.begin();
	while( iter_Type != m_mapParticleType.end() )
	{
		SParticleType*		pType = iter_Type->second;
		CTexture*			pTexture = (CTexture*)pType->pTexture;
		SAFE_RELEASE( pTexture );
		SAFE_FREE( pType );
		iter_Type = m_mapParticleType.erase( iter_Type );
	} //while
	m_listHParticleHead.clear();

	Assert( m_mapParticleType.empty() );
} //CW3DParticleEx::Cleanup

/**
 * Initialize for Particle Resource
 */
BOOL CW3DParticleEx::Init( const char* szFileName )
{
	SAFE_RELEASE( m_pFont );
	LOGFONT LogicalFont;
	LogicalFont.lfHeight			= 12;
	LogicalFont.lfWidth				= 0;
	LogicalFont.lfEscapement		= 0;
	LogicalFont.lfOrientation		= 0;
	LogicalFont.lfWeight			= FW_NORMAL;
	LogicalFont.lfItalic			= FALSE;
	LogicalFont.lfUnderline			= FALSE;
	LogicalFont.lfStrikeOut			= FALSE;
	LogicalFont.lfCharSet			= HANGUL_CHARSET;
	LogicalFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	LogicalFont.lfClipPrecision		= CLIP_DEFAULT_PRECIS;
	LogicalFont.lfQuality			= PROOF_QUALITY;
	LogicalFont.lfPitchAndFamily	= FIXED_PITCH | FF_MODERN;
	strcpy( LogicalFont.lfFaceName, "굴림" );

	D3DXCreateFontIndirect( g_pd3dDevice, &LogicalFont, &m_pFont );

	if( szFileName )	strcpy( m_szFileName, szFileName );
	else				strcpy( m_szFileName, STR_PARTICLEEXFILENAME );

	VERTEX			vertex[4];

	vertex[0].p.x		= -0.03F;
	vertex[0].p.y		=  0.03F;
	vertex[0].p.z		=  0.0F;
	vertex[0].tu		=  0.0F;
	vertex[0].tv		=  0.0F;

	vertex[1].p.x		=  0.03F;
	vertex[1].p.y		=  0.03F;
	vertex[1].p.z		=  0.0F;
	vertex[1].tu		=  1.0F;
	vertex[1].tv		=  0.0F;

	vertex[2].p.x		= -0.03F;
	vertex[2].p.y		= -0.03F;
	vertex[2].p.z		=  0.0F;
	vertex[2].tu		=  0.0F;
	vertex[2].tv		=  1.0F;

	vertex[3].p.x		=  0.03F;
	vertex[3].p.y		= -0.03F;
	vertex[3].p.z		=  0.0F;
	vertex[3].tu		=  1.0F;
	vertex[3].tv		=  1.0F;

	m_pVB = new CVertexBuffer( VERTEX::FVF, sizeof( VERTEX ) );
	if( !m_pVB )
	{
		Assert( !"Failed Create Instance for Vertex Buffer" );
		return FALSE;
	} //if

	if( !m_pVB->Create( 4 ) )
	{
		Assert( !"Failed Create Vertex Buffer" );
		SAFE_DELETE( m_pVB );
		return FALSE;
	} //if

	VERTEX*		pVert = (VERTEX*)m_pVB->Lock();
	if( !pVert )
	{
		SAFE_DELETE( m_pVB );
		return FALSE;
	} //if

	_MemCpy( pVert, vertex, sizeof( VERTEX[4] ) );
	m_pVB->Unlock();

	AllocateMem();

	return TRUE;
} //CW3DParticleEx::Init

/**
 * Set FileName Particle 
 */
void CW3DParticleEx::SetParticleFileName( const char* szFileName )
{
	strcpy( m_szFileName, szFileName );
} //CW3DParticleEx::SetParticleFileName

/**
 * Check the header of opened file
 */
int CW3DParticleEx::CheckFileHead( const char* szFileName )
{
	FILE*	pFile = fopen( szFileName, "rb" );
	if( !pFile )
	{
		Assert( !"Open Not File" );
		return -1;
	} //if

	char		szFileInfo[64];
	int			nVersion;
	int			nStrLen = strlen( STR_PARTICLEEXFILEINFO );
	fread( szFileInfo,	1, nStrLen,			pFile );
	fread( &nVersion,	1, sizeof( int ),	pFile );
	fclose( pFile );

	szFileInfo[nStrLen] = '\0';

	if( nVersion != VERSION_FILEVERSION ||
		strcmp( szFileInfo, STR_PARTICLEEXFILEINFO ) )
	{
		return -1;
	} //if

	DWORD		dwHeadSize = nStrLen + sizeof( int );
	return dwHeadSize;
} //CW3DParticleEx::CheckFileHead

/**
 * GetParticleType 
 */
SParticleType* CW3DParticleEx::GetParticleType( HPARTICLE hParticle )
{
	SParticleHead*		pPHead = m_hParticleHeadMgr.Dereference( hParticle );
	if( !pPHead )
	{
		Assert( !"It's NULL" );
		return NULL;
	} //if

	Assert( pPHead->psParticleType );
	return pPHead->psParticleType;
} //CW3DParticleEx::GetParticleType

/**
 * Get is ParticleType
 */
SParticleType* CW3DParticleEx::FindParticleType( const char* szParticleName )
{
	SParticleType*	pType = LoadParticleType( szParticleName );	
	if( !pType )
	{
		Assert( !"Not Found ParticleType" );
		return NULL;
	} //if

	return pType;
} //CW3DParticleEx::FindParticleType

/**
 * Get Size of OneData 
 */
int CW3DParticleEx::GetOneDataSize()
{
	return
		sizeof( DWORD )		+			// psPType->dwLifeTime
		sizeof( DWORD )		+			// psPType->dwIntervalTime
		sizeof( int )		+			// psPType->nNumOneParticles
	
		sizeof( Vector3 )	+			// psPType->vAccelerate
		sizeof( float )		+			// psPType->fGravity

		sizeof( DWORD )		+			// psPType->dwOneNodeTime
		sizeof( BOOL )		+			// psPType->bInverse

		sizeof( float[NUM_PARTICLE_NODE] )		+			// psPType->afSizes
		sizeof( float[NUM_PARTICLE_NODE] )		+			// psPType->afSpeeds
		sizeof( BYTE[NUM_PARTICLE_NODE] )		+			// psPType->abyColorAlpha
		sizeof( BYTE[NUM_PARTICLE_NODE] )		+			// psPType->abyColorRed
		sizeof( BYTE[NUM_PARTICLE_NODE] )		+			// psPType->abyColorGreen
		sizeof( BYTE[NUM_PARTICLE_NODE] )		+			// psPType->abyColorBlue
		sizeof( BOOL[NUM_PARTICLE_NODE] )		+			// psPType->abLightMode
	
		LENGTH_PARTICLENAME;
} //CW3DParticleEx::GetOneDataSize

/**
 * Save the header
 */
int CW3DParticleEx::SaveFileHead()
{
	if( strlen( m_szFileName ) <= 0 )
	{
		Assert( !"FileName" );
		return -1;
	} //if

	FILE*		pFile = fopen( m_szFileName, "wb" );
	if( !pFile )
	{
		Assert( !"Open Not the File" );
		return -1;
	} //if

	char		szFileInfo[] = STR_PARTICLEEXFILEINFO;
	int			nVersion	 = VERSION_FILEVERSION;
	int			nStrLen		 = strlen( szFileInfo );
	fwrite( szFileInfo,	1, nStrLen,			pFile );
	fwrite( &nVersion,	1, sizeof( int ),	pFile );
	fclose( pFile );

	DWORD		dwHeadSize = nStrLen + sizeof( int );
	return dwHeadSize;
} //CW3DParticleEx::SaveFileHead

/**
 * Load Particle Type
 */
SParticleType* CW3DParticleEx::LoadParticleType( const char* szLoadName )
{
	SParticleType*					pParticleType;
	MAP_PARTICLETYPE::iterator		iter_Type = m_mapParticleType.find( szLoadName );
	if( iter_Type != m_mapParticleType.end() )
	{
		pParticleType = iter_Type->second;
		Assert( pParticleType );

		return pParticleType;
	} //if

	int		nOffset = CheckFileHead( m_szFileName );
	if( nOffset < 0 )
	{
		Assert( !"Discord is Disagreenment and Version between File" );
		return FALSE;
	} //if

	FILE*	pFile = fopen( m_szFileName, "rb" );
	if( !pFile )
	{
		Assert( !"Can not Open Particle File" );
		return NULL;
	} //if
	fseek( pFile, nOffset, SEEK_SET );

	int		nDataOffset = GetOneDataSize();
	char	szParticleName[LENGTH_PARTICLENAME];
	char	szTextureName[LENGTH_PARTICLENAME];

	while( fread( szParticleName, 1, LENGTH_PARTICLENAME, pFile ) )
	{
		if( !strcmp( szParticleName, szLoadName )  )
		{
			pParticleType = (SParticleType*)_MemAlloc( sizeof( SParticleType ) );
			if( !pParticleType )
			{
				Assert( !"Failed Allocate Memory for ParticleType" );
				break;
			} //if
			ZeroMemory( pParticleType, sizeof( SParticleType ) );

			fread( &pParticleType->dwLifeTime,			1, sizeof( DWORD ),		pFile );		///< LifeTime
			fread( &pParticleType->dwIntervalTime,		1, sizeof( DWORD ),		pFile );		///< IntervalTime
			fread( &pParticleType->nNumOneParticles,	1, sizeof( int ),		pFile );		///< Number Once Particle
			
			fread( &pParticleType->vAccelerate,			1, sizeof( Vector3 ),	pFile );		///< Accelerate
			fread( &pParticleType->fGravity,			1, sizeof( float ),		pFile );		///< Gravity

			fread( &pParticleType->dwOneNodeTime,		1, sizeof( DWORD ),		pFile );		///< Node Per Time
			fread( &pParticleType->bInverse,			1, sizeof( BOOL ),		pFile );		///< Node Per Time

			fread( &pParticleType->afSizes,				NUM_PARTICLE_NODE, sizeof( float ),	pFile );		///< Node Per Size
			fread( &pParticleType->afSpeeds,			NUM_PARTICLE_NODE, sizeof( float ),	pFile );		///< Node Per Speed
			fread( &pParticleType->abyColorAlpha,		NUM_PARTICLE_NODE, sizeof( BYTE ),	pFile );		///< Node Per Color the Alpha
			fread( &pParticleType->abyColorRed,			NUM_PARTICLE_NODE, sizeof( BYTE ),	pFile );		///< Node Per Color the Red
			fread( &pParticleType->abyColorGreen,		NUM_PARTICLE_NODE, sizeof( BYTE ),	pFile );		///< Node Per Color the Green
			fread( &pParticleType->abyColorBlue,		NUM_PARTICLE_NODE, sizeof( BYTE ),	pFile );		///< Node Per Color the Blue
			fread( &pParticleType->abLightMode,			NUM_PARTICLE_NODE, sizeof( BOOL ),	pFile );		///< Node Per LightMode
			
			fread( szTextureName,						1, LENGTH_PARTICLENAME, pFile );		///< Texture Name

			pParticleType->pTexture = g_pTextureMgr->LoadTexture( "Texture/Particle/", szTextureName );
			if( !pParticleType->pTexture )
			{
				Assert( !"Not Found Particle Texture File" );
			} //if
			
			m_mapParticleType.insert( std::make_pair( string( szParticleName ), pParticleType ) );

			fclose( pFile );
			return pParticleType;
		}
		else
		{
			fseek( pFile, nDataOffset, SEEK_CUR );
		} //if..else
	} //while
	
	Assert( !"Not Found ParticleType On File" );

	fclose( pFile );
	return NULL;
} //CW3DParticleEx::LoadParticleType


/**
 * Load Particle Type
 */
SParticleType* CW3DParticleEx::LoadParticleType( const int nCount, char* szName, const int nNameLength )
{
	Assert( szName );

	SParticleType*					pParticleType;

	int		nOffset = CheckFileHead( m_szFileName );
	if( nOffset < 0 )
	{
		Assert( !"Discord is Disagreenment and Version between File" );
		return FALSE;
	} //if

	FILE*	pFile = fopen( m_szFileName, "rb" );
	if( !pFile )
	{
		Assert( !"Can not Open Particle File" );
		return NULL;
	} //if
	fseek( pFile, nOffset, SEEK_SET );

	int		nCurrCount = 0;
	int		nDataOffset = GetOneDataSize();
	char	szParticleName[LENGTH_PARTICLENAME];
	char	szTextureName[LENGTH_PARTICLENAME];

	while( fread( szParticleName, 1, LENGTH_PARTICLENAME, pFile ) > 0 )
	{
		if( nCurrCount == nCount )
		{
			pParticleType = (SParticleType*)_MemAlloc( sizeof( SParticleType ) );
			if( !pParticleType )
			{
				Assert( !"Failed Allocate Memory for ParticleType" );
				break;
			} //if
			ZeroMemory( pParticleType, sizeof( SParticleType ) );

			fread( &pParticleType->dwLifeTime,			1, sizeof( DWORD ),		pFile );		///< LifeTime
			fread( &pParticleType->dwIntervalTime,		1, sizeof( DWORD ),		pFile );		///< IntervalTime
			fread( &pParticleType->nNumOneParticles,	1, sizeof( int ),		pFile );		///< Number Once Particle
			
			fread( &pParticleType->vAccelerate,			1, sizeof( Vector3 ),	pFile );		///< Accelerate
			fread( &pParticleType->fGravity,			1, sizeof( float ),		pFile );		///< Gravity

			fread( &pParticleType->dwOneNodeTime,		1, sizeof( DWORD ),		pFile );		///< Node Per Time
			fread( &pParticleType->bInverse,			1, sizeof( BOOL ),		pFile );		///< Node Per Time

			fread( &pParticleType->afSizes,				NUM_PARTICLE_NODE, sizeof( float ),	pFile );		///< Node Per Size
			fread( &pParticleType->afSpeeds,			NUM_PARTICLE_NODE, sizeof( float ),	pFile );		///< Node Per Speed
			fread( &pParticleType->abyColorAlpha,		NUM_PARTICLE_NODE, sizeof( BYTE ),	pFile );		///< Node Per Color the Alpha
			fread( &pParticleType->abyColorRed,			NUM_PARTICLE_NODE, sizeof( BYTE ),	pFile );		///< Node Per Color the Red
			fread( &pParticleType->abyColorGreen,		NUM_PARTICLE_NODE, sizeof( BYTE ),	pFile );		///< Node Per Color the Green
			fread( &pParticleType->abyColorBlue,		NUM_PARTICLE_NODE, sizeof( BYTE ),	pFile );		///< Node Per Color the Blue
			fread( &pParticleType->abLightMode,			NUM_PARTICLE_NODE, sizeof( BOOL ),	pFile );		///< Node Per LightMode
			
			fread( szTextureName,						1, LENGTH_PARTICLENAME, pFile );		///< Texture Name

			pParticleType->pTexture = g_pTextureMgr->LoadTexture( "Texture/Particle/", szTextureName );
			if( !pParticleType->pTexture )
			{
				Assert( !"Not Found Particle Texture File" );
			} //if
			
			m_mapParticleType.insert( std::make_pair( string( szParticleName ), pParticleType ) );

			fclose( pFile );

			if( strlen( szParticleName ) > nNameLength )
			{
				Assert( !"Over String Length" );
				return NULL;
			} //if

			strcpy( szName, szParticleName );
			return pParticleType;
		}
		else
		{
			fseek( pFile, nDataOffset, SEEK_CUR );
		} //if..else

		nCurrCount++;
	} //while
	
	Assert( !"Not Found ParticleType On File" );

	fclose( pFile );

	szName[0] = '\0';
	return NULL;
} //CW3DParticleEx::LoadParticleType

/**
 * Save the data of Particletype by opened the file of end
 */
BOOL CW3DParticleEx::SaveParticleType( 
	const char* szName, 
	SParticleType* psPType,
	const char* szTextureName
	)
{
	Assert( szName );
	Assert( psPType );
	Assert( szTextureName );

	int		nOffset = CheckFileHead( m_szFileName );
	if( nOffset < 0 )
	{
		Assert( !"Decord is Disagreenment and Version between file" );
		return FALSE;
	} //if

	FILE*	pFile = fopen( m_szFileName, "ab" );
	if( !pFile )
	{
		Assert( !"Do Not Open the File" );
		return FALSE;
	} //if
	fseek( pFile, nOffset, SEEK_SET );

	fwrite( szName,				1, LENGTH_PARTICLENAME,	pFile );		///< Particle Name

	fwrite( &psPType->dwLifeTime,		1, sizeof( DWORD ),		pFile );		///< LifeTime
	fwrite( &psPType->dwIntervalTime,	1, sizeof( DWORD ),		pFile );		///< IntervalTime
	fwrite( &psPType->nNumOneParticles,	1, sizeof( int ),		pFile );		///< Number Once Particle
	
	fwrite( &psPType->vAccelerate,		1, sizeof( Vector3 ),	pFile );		///< Accelerate
	fwrite( &psPType->fGravity,			1, sizeof( float ),		pFile );		///< Gravity

	fwrite( &psPType->dwOneNodeTime,	1, sizeof( float ),		pFile );		///< Node Per Time
	fwrite( &psPType->bInverse,			1, sizeof( BOOL ),		pFile );		///< Inverse

	fwrite( psPType->afSizes,			NUM_PARTICLE_NODE, sizeof( float ),		pFile );		///< Node Per Size
	fwrite( psPType->afSpeeds,			NUM_PARTICLE_NODE, sizeof( float ),		pFile );		///< Node Per Speed
	fwrite( psPType->abyColorAlpha,		NUM_PARTICLE_NODE, sizeof( BYTE ),		pFile );		///< Node Per Color the Alpha
	fwrite( psPType->abyColorRed,		NUM_PARTICLE_NODE, sizeof( BYTE ),		pFile );		///< Node Per Color the Red
	fwrite( psPType->abyColorGreen,		NUM_PARTICLE_NODE, sizeof( BYTE ),		pFile );		///< Node Per Color the Green
	fwrite( psPType->abyColorBlue,		NUM_PARTICLE_NODE, sizeof( BYTE ),		pFile );		///< Node Per Color the Blue
	fwrite( psPType->abLightMode,		NUM_PARTICLE_NODE, sizeof( BOOL ),		pFile );		///< Node Per LightMode
	
	fwrite( szTextureName,		1, LENGTH_PARTICLENAME,	pFile );		///< Texture Name

	fclose( pFile );
	return TRUE;
} //CW3DParticleEx::SaveParticleType

/**
 * Add Particle
 */
HPARTICLE CW3DParticleEx::AddParticle( const char* szParticleName, Vector3& vPos, DWORD dwLoop )
{
	HPARTICLE	hParticle;
	SParticleType*		pParticleType = LoadParticleType( szParticleName );
	if( !pParticleType )
	{
		Assert( !"Failed Found Particle Type" );
		return hParticle;
	} //if
	
	SParticleHead*		pParticleHead = m_hParticleHeadMgr.Acquire( hParticle );
	if( !pParticleHead )
	{
		Assert( !"Failed Allocate Memory for ParticleHead" );
		return hParticle;
	} //if

	pParticleHead->psParticleType	= pParticleType;
	pParticleHead->vStartPos		= vPos;
	pParticleHead->dwLoop			= dwLoop;
	pParticleHead->dwLoopCount		= 0;
	pParticleHead->bRun				= TRUE;

	m_listHParticleHead.push_back( hParticle );
	
	return hParticle;
} //CW3DParticleEx::AddParticle

/**
 * DeleteParticle 
 */
void CW3DParticleEx::DeleteParticle( HPARTICLE hParticle )
{
	m_hParticleHeadMgr.Release( hParticle );
	
	LIST_HPARTICLEHEAD::iterator	iter = m_listHParticleHead.begin();
	while( iter != m_listHParticleHead.end() )
	{
		if( (*iter) == hParticle )
		{
			m_listHParticleHead.erase( iter );
			return;
		} //if
		iter++;
	} //while
} //CW3DParticleEx::DeleteParticle

/**
 * Allocation Memory for Particle
 */
void CW3DParticleEx::AllocateMem()
{
	ZeroMemory( m_asParticle, sizeof( SParticle ) * NUM_ALLOCATE_PARTICLEMEM );
	for( int nCnt = 0; nCnt < NUM_ALLOCATE_PARTICLEMEM; nCnt++ )
	{
		m_queParticleMem.push( &m_asParticle[nCnt] );
	} //for
} //CW3DParticleEx::AllocateMem

/**
 * 
 */
SParticle* CW3DParticleEx::GetParticleMem()
{
	Assert( m_queParticleMem.size() == NUM_ALLOCATE_PARTICLEMEM );

	SParticle*			pParticle = m_queParticleMem.front();
	m_queParticleMem.pop();
	m_queParticleMem.push( pParticle );
	ZeroMemory( pParticle, sizeof( SParticle ) );

	return pParticle;
} //CW3DParticleEx::GetParticleMem

/**
 * Run 
 */
void CW3DParticleEx::Run( HPARTICLE hParticle )
{
	SParticleHead*		pParticleHead = m_hParticleHeadMgr.Dereference( hParticle );
	if( !pParticleHead )
	{
		Assert( !"It's NULL" );
		return;
	} //if

	pParticleHead->bRun = TRUE;
} //CW3DParticleEx::Run

/**
 * Stop 
 */
void CW3DParticleEx::Stop( HPARTICLE hParticle )
{
	SParticleHead*		pParticleHead = m_hParticleHeadMgr.Dereference( hParticle );
	if( !pParticleHead )
	{
		Assert( !"It's NULL" );
		return;
	} //if

	pParticleHead->bRun = FALSE;
} //CW3DParticleEx::Stop

/**
 * Set Texture ( in SParticleType )
 */
void CW3DParticleEx::SetTexture( HPARTICLE hParticle, const char* szFileName )
{
	SParticleHead*		pParticleHead = m_hParticleHeadMgr.Dereference( hParticle );
	if( !pParticleHead )
	{
		Assert( !"It's NULL" );
		return;
	} //if

	if( !pParticleHead->psParticleType )
	{
		Assert( !"It's NULL" );
		return;
	} //if

	SParticleType*		pPType = pParticleHead->psParticleType;
	Assert( pPType );

	CTexture*		pTexture = static_cast<CTexture*>( pPType->pTexture );
	SAFE_RELEASE( pTexture );
	pPType->pTexture = NULL;
	pPType->pTexture = g_pTextureMgr->LoadTexture( "", szFileName );
	Assert( pPType->pTexture );
} //CW3DParticleEx::SetTexture

/**
 * Progresing Particles
 */
void CW3DParticleEx::Update( const DWORD dwTick )
{
	m_dwIntervalTick = dwTick - m_dwTick;
	m_dwTick = dwTick;

	SParticle*					pParticle;
	SParticleHead*				pHead;
	HPARTICLE					hParticleHead;
	SParticleType*				pType;
	LIST_HPARTICLEHEAD::iterator	 iter_Head = m_listHParticleHead.begin();
	while( iter_Head != m_listHParticleHead.end() )
	{
		hParticleHead = (*iter_Head);
		if( hParticleHead.IsNull() )
		{
			Assert( !"It's NULL" );
			continue;
		} //if
		pHead = m_hParticleHeadMgr.Dereference( hParticleHead );
		Assert( pHead );

		if( !pHead || !pHead->bRun )
		{
			iter_Head++;
			continue;
		} //if

		if( pHead->dwLoop > 0 && 
			pHead->dwLoop <= pHead->dwLoopCount )
		{
			m_hParticleHeadMgr.Release( hParticleHead );
			DeleteParticle( hParticleHead );
			iter_Head = m_listHParticleHead.erase( iter_Head );
			continue;
		} //if

		pType = pHead->psParticleType;
		Assert( pType );

		DWORD		dwInterTick = dwTick - pHead->dwPrevTick;
		if( dwInterTick > pType->dwIntervalTime )
		{
			pHead->dwPrevTick = dwTick;
			pHead->dwLoopCount++;
			for( int nCnt = 0; nCnt < pType->nNumOneParticles; nCnt++ )
			{
				int			nRemainTime = pType->dwLifeTime - ( dwInterTick - pType->dwIntervalTime );
				if( nRemainTime <= 0 )
				{
					break;
				} //if

				pParticle = GetParticleMem();
				Assert( pParticle );

				pParticle->vStartPos		= pHead->vStartPos;
				pParticle->psParticleType	= pType;
				pParticle->nRemainTime		= nRemainTime;
				pParticle->vDirection.x = float( ( rand() % 6 ) - 3.0F );
				pParticle->vDirection.y = float( ( rand() % 6 ) - 3.0F );
				pParticle->vDirection.z = float( ( rand() % 6 ) - 3.0F );
				m_listParticle.push_back( pParticle );
			} //for
		} //if
		iter_Head++;
	} //while

	Vector3				vDirection;
	float				fSpeed;
	float				fAddSize;
	int					nBlendColor;
	BYTE				byColor[4];
	DWORD				dwCurrentTime;
	float				fRatioNode;
	int					nCurrentNodeIdx;
	
	LIST_PARTICLE::iterator		iter_Particle = m_listParticle.begin();
	while( iter_Particle != m_listParticle.end() )
	{
		pParticle		= (*iter_Particle);
		Assert( pParticle );
		
		pParticle->nRemainTime -= m_dwIntervalTick;
		if( pParticle->nRemainTime <= 0 )
		{
			iter_Particle = m_listParticle.erase( iter_Particle );
			continue;
		} //if

		pType = pParticle->psParticleType;
		if( pType->bInverse )	dwCurrentTime = pParticle->nRemainTime;
		else					dwCurrentTime = pType->dwLifeTime - pParticle->nRemainTime;
		dwCurrentTime--;

		nCurrentNodeIdx = dwCurrentTime / pType->dwOneNodeTime;
		if( nCurrentNodeIdx >= NUM_PARTICLE_NODE || 
			nCurrentNodeIdx < 0 )
		{
			iter_Particle++;
			continue;
		} //if

		fRatioNode = (float)( dwCurrentTime % pType->dwOneNodeTime ) / pType->dwOneNodeTime;
		
		float	fAddSpeed = pType->afSpeeds[nCurrentNodeIdx+1] - pType->afSpeeds[nCurrentNodeIdx];
		fSpeed = pType->afSpeeds[nCurrentNodeIdx];
		if( fAddSpeed != 0.0F )	fSpeed += fAddSpeed * fRatioNode;

		vDirection.x = pParticle->vDirection.x + ( pType->vAccelerate.x * dwCurrentTime );
		vDirection.y = pParticle->vDirection.y + ( pType->vAccelerate.y * dwCurrentTime );
		vDirection.z = pParticle->vDirection.z + ( pType->vAccelerate.z * dwCurrentTime );

		pParticle->vPosition.x = pParticle->vStartPos.x + ( vDirection.x * fSpeed );
		float		fG = pType->fGravity * dwCurrentTime;
		pParticle->vPosition.y = pParticle->vStartPos.y + ( vDirection.y * fSpeed ) - ( fG * fG );
		pParticle->vPosition.z = pParticle->vStartPos.z + ( vDirection.z * fSpeed );

		pParticle->bInView = g_Frustum.CheckPoint( pParticle->vPosition );
		if( !pParticle->bInView )
		{
			iter_Particle++;
			continue;
		} //if

		nBlendColor = pType->abyColorAlpha[nCurrentNodeIdx+1] - pType->abyColorAlpha[nCurrentNodeIdx];
		byColor[0] = pType->abyColorAlpha[nCurrentNodeIdx];
		if( nBlendColor != 0 )	byColor[0] += (BYTE)( (float)nBlendColor * fRatioNode );

		nBlendColor = pType->abyColorRed[nCurrentNodeIdx+1] - pType->abyColorRed[nCurrentNodeIdx];
		byColor[1] = pType->abyColorRed[nCurrentNodeIdx];
		if( nBlendColor != 0 )	byColor[1] += (BYTE)( (float)nBlendColor * fRatioNode );

		nBlendColor = pType->abyColorGreen[nCurrentNodeIdx+1] - pType->abyColorGreen[nCurrentNodeIdx];
		byColor[2] = pType->abyColorGreen[nCurrentNodeIdx];
		if( nBlendColor != 0 )	byColor[2] += (BYTE)( (float)nBlendColor * fRatioNode );

		nBlendColor = pType->abyColorBlue[nCurrentNodeIdx+1] - pType->abyColorBlue[nCurrentNodeIdx];
		byColor[3] = pType->abyColorBlue[nCurrentNodeIdx];
		if( nBlendColor != 0 )	byColor[3] += (BYTE)( (float)nBlendColor * fRatioNode );

		pParticle->dwColor = COLOR_ARGB( byColor[0],  byColor[1], byColor[2], byColor[3] );

		fAddSize = pType->afSizes[nCurrentNodeIdx+1] - pType->afSizes[nCurrentNodeIdx];
		pParticle->fSize = pType->afSizes[nCurrentNodeIdx];
		if( fAddSize != 0.0F )		pParticle->fSize += (fAddSize * fRatioNode);
		if( pParticle->fSize < 0.02 )
		{
			pParticle->fSize = pParticle->fSize;
		} //if

		pParticle->bLightMode = pType->abLightMode[nCurrentNodeIdx];
		if( pParticle->bLightMode )
		{
			m_listParticle.push_front( pParticle );
			iter_Particle = m_listParticle.erase( iter_Particle );
		}
		else
		{
			iter_Particle++;
		} //if..else
	} //while

} //CW3DParticleEx::Update

/**
 * Multiply Particle With Billboard Matrix 
 */
void CW3DParticleEx::SetWorld( const Vector3& vPos, const float fW )
{
	static Matrix4	matWorld;
	matWorld = g_Frustum.m_matView.GetBillboard();

	matWorld._41 = vPos.x * fW;
	matWorld._42 = vPos.y * fW;
	matWorld._43 = vPos.z * fW;
	matWorld._44 *= fW;

	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );
} //CW3DParticleEx::SetWorld

/**
 * Setting Render State for Particle
 */
void CW3DParticleEx::SetRenderState( const BOOL bLight )
{
	D3D::ZEnable( TRUE );
	D3D::ZWriteEnable( FALSE );
	D3D::Lighting( FALSE );
	D3D::AlphaBlend( TRUE );

	D3D::FogEnable( FALSE );

	// Texture 관련 세팅
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
} //CW3DParticleEx::SetRenderState

/**
 * Rendering All Live Particle
 */
void CW3DParticleEx::Render( const DWORD dwTick )
{	
	Update( dwTick );

	int		nLight = -1;
	
	SetRenderState( TRUE );

	LIST_PARTICLE::iterator		iter_Particle = m_listParticle.begin();
	while( iter_Particle != m_listParticle.end() )
	{
		SParticle*		pParticle = (*iter_Particle);
		Assert( pParticle );
		
		iter_Particle++;

		if( !pParticle->bInView )
		{
			continue;
		} //if

		if( nLight != pParticle->bLightMode )
		{
			nLight = pParticle->bLightMode;

			if( nLight )
			{
				g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
				g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
				D3D::AlphaTest( FALSE );
			}
			else
			{
				g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
				g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
				D3D::AlphaTest( TRUE );
			} //if..else..
		} //if

		SParticleType*		pType = pParticle->psParticleType;
		Assert( pType );

		if( pType->pTexture )
		{
			CTexture*	pTexture = static_cast<CTexture*>( pType->pTexture );
			Assert( pTexture );
			pTexture->SetTexture( 0 );
			g_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, pParticle->dwColor );
		} //if

		SetWorld( pParticle->vPosition, pParticle->fSize );
		m_pVB->Draw( D3DPT_TRIANGLESTRIP, 2 );
	} //while
} //CW3DParticleEx::Render

void CW3DParticleEx::DrawText( int nX, int nY, DWORD dwColor, const char* szFormat, ... )
{
	Assert( g_pd3dDevice );
	Assert( m_pFont );

	char		szText[256];

	va_list		argptr;
	va_start( argptr, szFormat );
	vsprintf( szText, szFormat, argptr );
	va_end( argptr );

	m_pFont->Begin();

	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE );
	g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );

	RECT	rc;
	SetRect( &rc, nX, nY, nX + strlen( szText ) * 8, nY + 12 );

	m_pFont->DrawText( szText, -1, &rc, DT_LEFT | DT_TOP, dwColor );

	m_pFont->End();
} //DrawText