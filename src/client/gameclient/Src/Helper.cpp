#include "Global.h"
#include "Helper.h"

/**
 *
 */
Vector3	GetRandomPos( Vector3 vPos )
{
	int nRandom = random( 2 );
	int nGop	= 1;

	if( nRandom > 0 )
	{
		nGop	= -1;
	} //if

	Vector3 vGoal = vPos;
	vGoal.x += ( ( (float)random( 10 ) / 10.0F + 0.3F ) * nGop );

	nRandom = random( 2 );
	nGop	= 1;

	if( nRandom > 0 )
	{
		nGop	= -1;
	} //if

	vGoal.y += ( ( (float)random( 10 ) / 10.0F + 0.3F ) * nGop );
	vGoal.z = vPos.z;

	return vGoal;
} //GetRandomPos

/**
 * Construction
 */
CHelper::CHelper()
:	m_wKind			( 0 ),
	m_wLevel		( 0 ),
	m_nStage		( 0 ),
	m_dwStateTime	( 0 ),
	m_dwTime		( 0 ),
	m_dwParticle	( 0 ),
	m_dwTick		( 0 ),
	m_bGoAway		( FALSE ),
	m_bPlayer		( FALSE ),
	m_bLetGoAway	( FALSE ),
	m_pWorld		( NULL ),
	m_pScene		( NULL ),
	m_pModel		( NULL ),
	m_pCreature		( NULL )
{
} //CHelper::CHelper

/**
 * Destruction
 */
CHelper::~CHelper()
{
	SAFE_RELEASE( m_pModel );
} //CHelper::~CHelper

/**
 * Initialize
 */
BOOL CHelper::Init( IW3DWorld* pWorld, IW3DCreature* pCreature )
{
	Assert( pWorld );
	Assert( pCreature );

	m_pWorld	= pWorld;
	
	SetCreature( pCreature );

	m_nState	= MAY_STOP;

	m_pScene	 = MAKE_COMPONENT( Scene );
	if( !m_pScene )
	{
		Assert( !"Not Found Scene" );
		return FALSE;
	} //if

	m_Bezier.Init();
	m_fSpeed	 = 1.5F;
	m_fHeight	 = MAY_HEIGHT;

	return TRUE;
} //CHelper::Init

/** 
 * Cleanup
 */
void CHelper::Cleanup()
{
	SAFE_RELEASE( m_pModel );
} //CHelper::Cleanup

/**
 *
 */
void CHelper::MoveTo( const DWORD dwTick )
{
	float fHeight = m_vPosForCreature.z + m_fHeight;
	m_vPosForCreature.z = m_vPos.z = 0.0F;
	float fDist = ( m_vPosForCreature - m_vPos ).Length();

	float fTraffic = 4.0F;

	if( m_nState == MAY_FOLLOW ) 
	{
		fTraffic = 1.5F;
	} //if

	if( fDist <= fTraffic )
	{
		if( m_nState == MAY_FOLLOW )
		{
			m_nState = MAY_STOP;
		} //if

		m_vPos.z = fHeight;
		m_vPosForCreature.z = fHeight - m_fHeight;
		return ;
	} //if

	m_nState = MAY_FOLLOW;
	float	fOffset = m_fSpeed * dwTick / 1000.0F;

	Vector3 vDir = ( m_vPosForCreature - m_vPos ).Normal();

	if( vDir == Vector3( 0.0F, 0.0F, 0.0F ) )
	{
		m_vPos.z = fHeight;
		m_vPosForCreature.z = fHeight - m_fHeight;
		m_nState = MAY_STOP;
		return ;
	} //if

	if( fDist < fOffset )
	{
		fOffset = fDist + 0.2F;
	} //if

	m_vPos += vDir * fOffset;
	m_vPos.z = fHeight;
	m_vPosForCreature.z = fHeight - m_fHeight;

	m_qRot = ( m_vPosForCreature - m_vPos ).GetQuaternion();
	
} //CHelper::MoveTo

/**
 *
 */
void CHelper::StateMove()
{
	if( m_nState != MAY_MOVE )
	{
		m_nState = MAY_MOVE;

		m_vGoal = GetRandomPos( m_vPosForCreature );
		m_vGoal.z += m_fHeight;

		m_Bezier.Init();
		m_Bezier.v1 = m_vPos;
		m_Bezier.v2 = GetRandomPos( m_vPosForCreature );
		m_Bezier.v3 = GetRandomPos( m_vPosForCreature );
		m_Bezier.v4 = m_vGoal;

		m_Bezier.v2.z += m_fHeight;
		m_Bezier.v3.z += m_fHeight;
	} //if
 
	m_vGoal.z = 0.0F;
	m_vPos.z = 0.0F;

//////////////////////
	// 매개 변수 f를 구한다.. 그래봤자..0에서 1사이값을 결정해주는 것일뿐
	float f = (float)( 100 ) / 10000;
			
	if( !m_Bezier.Move( f ) )
	{
		m_nState = MAY_STOP;
		m_vPos.z = m_vPosForCreature.z + m_fHeight;
		return ;
	} //if
	
	Vector3	vDir = m_vPos;

	if( vDir == Vector3( 0.0F, 0.0F, 0.0F ) )
	{
		vDir.y = 1.0F;
	} //if

	m_vPos = m_Bezier.GetBezier();

	vDir  -= m_vPos;
	m_qRot = ( vDir * -1 ).GetQuaternion();
} //CHelper::Move

/**
 * Update
 */
BOOL CHelper::Update( DWORD dwTick )
{
	if( m_bGoAway || !m_pCreature )
	{
		return TRUE;
	} //if

	m_dwTick = dwTick;

	m_vPosForCreature = m_pCreature->GetPosition();	

	MoveTo( dwTick );

	if( m_dwStateTime >= 1000 && m_nState == MAY_STOP )
	{
		m_dwStateTime = 0;

		if( m_nState == MAY_STOP )
		{
			m_vGoal = m_vPosForCreature;

			if( random( 10 ) >= 7 )
			{		
				StateMove();
			} //if..else..
		} //if
	} //if

	switch( m_nState )
	{		
	case MAY_STOP:
		StateStop();
		break;

	case MAY_MOVE:
		StateMove();
		break;

	case MAY_FOLLOW:
		MoveTo( dwTick );
		break;

	case MAY_ATTACK:
		StateAttack();
		break;
	}; //switch

	HeightWave();	
	
	m_dwTime		+= dwTick;
	m_dwStateTime	+= dwTick;
	m_dwTalkTime	+= dwTick;
	m_dwParticle	+= dwTick;

	return FALSE;
} //CHelper::Update

/**
 * Draw
 */
void CHelper::Draw()
{
	if( !m_pModel )
	{
		Assert( !"Model Is Null" );
		return;
	} //if

	Matrix4	matWorld;
	matWorld.SetIdentity();
	matWorld.SetWorld( m_vPos, m_qRot );
	m_pModel->SetWorld( matWorld );
	m_pModel->Render( m_dwTime );
} //CHelper::Draw

/**
 *
 */
void CHelper::SetCreature( IW3DCreature* pCreature )
{
	Assert( pCreature );

	m_pCreature = pCreature;
	m_vPos		= m_pCreature->GetPosition() + Vector3( 1.0F, 0.0F, m_fHeight );
	m_vGoal		= m_vPos;

	Vector3		vDir( 0.0F, 0.0F, 1.0F );
	m_qRot		= vDir.GetQuaternion();

} //CHelper::SetCreature

 
/**
 *
 */
void CHelper::HeightWave()
{
//	m_vPos.z += sin( ANGLE( m_dwTime / 1000 ) * 2.0F ) / 100.0F;
//	m_vPos.x += sin( ANGLE( m_dwTime / 1000 ) ) + random( 5 );
//	m_vPos.y -= cos( ANGLE( m_dwTime / 1000 ) * 2.0F ) * 5.0F;

} //CHelper::HeightWave
