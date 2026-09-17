#ifndef __GameClient_Helper_Header__
#define __GameClient_Helper_Header__

#define		MAY_STOP	0
#define		MAY_MOVE	1
#define		MAY_FOLLOW	2
#define		MAY_ROT		3
#define		MAY_DEAD	4
#define		MAY_START	5
#define		MAY_LEVELUP	6
#define		MAY_ATTACK	7
#define		MAY_DEAL_BEGIN			8
#define		MAY_PARTY_CREATE		9
#define		MAY_PARTY_JOIN_FAIL		10
#define		MAY_PARTY_JOIN_SUCCED	11
#define		MAY_PARTY_DESTORY		12
/*
#define		MAY_MONSTER_MAX			13
#define		MAY_MONSTER_MIN			14
#define		MAY_PLAYER_MAX			15
#define		MAY_PLAYER_MIN			16
*/

#define	MAY_HEIGHT			1.2F


/**
 * Bezier Curve.
 * @ingroup		Engine
 */
struct BEZIER
{
	UNDER_MEMORY_MGR( BEZIER );

	//-=-=-=-=-=-=-=-=-=-=-= Vector3 -=-=-=-=-=-=-=-=-=-=-=//
	Vector3		v1;			///< 시작점
	Vector3		v2;			///< Control Point 1
	Vector3		v3;			///< Control Point 2
	Vector3		v4;			///< 종료 점

	float		t;

	/**
	 * 초기화.
	 */
	void Init()
	{
		v1 = Vector3( 0.0F, 0.0F, 0.0F );
		v2 = v3 = v4 = v1;
		t = 0.0F;
	} //Init

	BOOL Move( float time )
	{
		t += time;

		if( t >= 0.99 )
		{
			return FALSE;
		}

		return TRUE;
	}//Move

	/**
	 * 곡선 위의 중간 좌표를 얻어 온다.
	 *
	 * @param	t		0부터 1 사이값
	 *
	 * @return		중간 좌표.
	 */
	Vector3		GetBezier()
	{
		Vector3		vPos;

		vPos  = v1     * ( 1 - t ) * ( 1 - t ) * ( 1 - t );
		vPos +=	v2 * 3 * t         * ( 1 - t ) * ( 1 - t );
		vPos +=	v3 * 3 * t         * t         * ( 1 - t );
		vPos +=	v4     * t         * t         * t;

		return vPos;
	} //GetBezier
}; //struct BEZIER

/**
 * Class CHelper
 */
class CHelper
{
public: //< operator
	enum { MAY_TYPE_MAY = 1, MAY_TYPE_PET = 2 };

	MEMORY_UNDER_MGR( CHelper );

	CHelper();
	virtual ~CHelper();
	
	BOOL		Init( IW3DWorld* pWorld, IW3DCreature* pCreature );
	void		Cleanup();

	BYTE		GetType()					{ return m_byType; }

	virtual BOOL		Update( DWORD dwTick );
	virtual void		DrawName( IW3DRectangle* pRectangle ){};
	virtual void		Draw();	

	virtual BOOL		IsEndAction()		{ return TRUE; };
	
	virtual BOOL		SetInfo( const WORD wType, WORD wLevel, BOOL bPlayer ){ return TRUE; };
	virtual BOOL		SetInfo( const WORD wType ){ return TRUE; };

	void		SetCreature( IW3DCreature* pCreature );	
	void		SetSpeed( float fSpeed )	{ m_fSpeed = fSpeed; }
	void		SetHeight( float fHeight )	{ m_fHeight = fHeight; }
	void		SetData( void* pData )		{}

	virtual void		Attack( IW3DCreature* pTarget )		{};
	virtual void		Attack()							{};
	virtual void		Dead()								{};

	virtual void		Talk( const char* szTalk )			{}
	virtual void		AttackMonster( const char* szName )	{}
	virtual void		Party( BYTE btType )				{}

protected:
	void				MoveTo( const DWORD dwTick );
	void				HeightWave();
	void				StateMove();
	virtual	void		StateStop()		{};
	virtual void		StateAttack()	{};
	
protected : //< protected
	BYTE			m_byType;				///< 종류
	WORD			m_wKind;				///< 메이의 종류
	WORD			m_wLevel;				///< 메이의 레벨( 캐릭터의 레벨 )

	IW3DScene*		m_pScene;
	IW3DWorld*		m_pWorld;				///< World포인터
	IW3DCreature*	m_pCreature;			///< 메이의 주인
	IW3DCreature*	m_pTarget;				///< 메이의 적

	IW3DModel*		m_pModel;				///< 모델

	DWORD			m_dwTime;				///< 경과틱
	DWORD			m_dwTick;				///< 누적틱
	DWORD			m_dwParticle;			///< 파티클 누적틱
	DWORD			m_dwStateTime;
	DWORD			m_dwTalkTime;

	BOOL			m_bLetGoAway;			
	BOOL			m_bGoAway;				///< 떠나라~ ( 去(さ)りなさい。)
	BOOL			m_bStart;				///< 시작 부분인가?
	BOOL			m_bPlayer;				///< 플레이어인지의 여부

	Vector3			m_vPosForCreature;		///< 플레이어의 위치
	Vector3			m_vPos;					///< 메이의 위치
	Quaternion		m_qRot;					///< 메이의 방향
	Vector3			m_vGoal;				///< 종료 위치

	int				m_nState;
	int				m_nStage;
	float			m_fSpeed;				///< 이동 속도
	float			m_fHeight;

	BEZIER			m_Bezier;
}; //CHelper

#endif //__GameClient_Helper_Header__

