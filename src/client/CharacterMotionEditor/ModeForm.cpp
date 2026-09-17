#include "StdAfx.h"
#include "ModeForm.h"

/**
 *
 */
CModeForm::CModeForm()
{
} //CModeForm::CModeForm

/**
 *
 */
CModeForm::~CModeForm()
{

} //CModeForm::~CModeForm

/**
 *
 */
BOOL CModeForm::Create( int nX, int nY, CWnd* pWnd )
{	

	for( int i = 0 ; i < 6 ; i++ )
	{
		m_aEditForm[i].Create( nX, nY + ( i * 22 ), pWnd );
	}  //if

	m_aEditForm[6].Create( nX, nY + 150, pWnd );
	m_aEditForm[7].Create( nX, nY + 192, pWnd );

	m_aEditForm[8].Create( nX, nY + 254, pWnd );
	m_aEditForm[9].Create( nX, nY + 275, pWnd );
	m_aEditForm[10].Create( nX, nY + 296, pWnd );
	m_aEditForm[11].Create( nX, nY + 317, pWnd );
	m_aEditForm[12].Create( nX, nY + 360, pWnd );
//198
	return FALSE;
} //CModeForm::Create

/**
 *
 */
void CModeForm::Set( const SMode& mode )
{
	mode.nCount;

	for( int i = 0 ; i < mode.nCount ; i++ )
	{
		//m_aEditForm[i].Set( mode.aAction[
	} //for

} //CModeForm::Set

/**
 *
 */
void CModeForm::Show( SMode* pMode )
{
	/*
		정지 
		걷기
		뛰기
		사망
		걷기모드변경
		뛰기모드변경
	*/
	for( int i = 0 ; i < 6 ; i++ )
	{
		m_aEditForm[i].Set( pMode->aAction[i].aMotion[pMode->aAction[i].nSelect] );

	} //if

	// 공격 

	SAction* pAction = &pMode->aAction[STATE_ATTACK1 + g_nAttackNumber];
	m_aEditForm[ROW_ATTACK].Set( pAction->aMotion[pAction->nSelect] );

	// 뛰면서 공격

	pAction = &pMode->aAction[STATE_RUN_ATTACK1 + g_nRunAttackNumber];
	m_aEditForm[ROW_RUNATTACK].Set( pAction->aMotion[pAction->nSelect] );
	
	// 마법
	pAction = &pMode->aAction[STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber];
	m_aEditForm[ROW_MAGIC_1].Set( pAction->aMotion[pAction->nSelect] );
	
	pAction = &pMode->aAction[STATE_MAGIC_ATTACK_DOWNANDUP + g_nMagicNumber];
	m_aEditForm[ROW_MAGIC_2].Set( pAction->aMotion[pAction->nSelect] );
	
	pAction = &pMode->aAction[STATE_MAGIC_ATTACK_GO + g_nMagicNumber];
	m_aEditForm[ROW_MAGIC_3].Set( pAction->aMotion[pAction->nSelect] );
	
	pAction = &pMode->aAction[STATE_MAGIC_ATTACK_AROUND + g_nMagicNumber];
	m_aEditForm[ROW_MAGIC_4].Set( pAction->aMotion[pAction->nSelect] );

	pAction = &pMode->aAction[STATE_EMOTICON + g_nEmoticonNumber ];
	m_aEditForm[ROW_EMOTICON].Set( pAction->aMotion[pAction->nSelect] );
} //CModeForm::Show

/**
 *
 */
void CModeForm::Save( SMode* pMode )
{
	//g_ActionMgr.
	//pMode->nCount
	for( int i = 0 ; i < 6 ; i++ )
	{
		m_aEditForm[i].Save( &pMode->aAction[i].aMotion[pMode->aAction[i].nSelect] );
	} //if

	// 공격 

	SAction* pAction = &pMode->aAction[STATE_ATTACK1 + g_nAttackNumber];
	m_aEditForm[ROW_ATTACK].Save( &pAction->aMotion[pAction->nSelect] );

	// 뛰면서 공격

	pAction = &pMode->aAction[STATE_RUN_ATTACK1 + g_nRunAttackNumber];
	m_aEditForm[ROW_RUNATTACK].Save( &pAction->aMotion[pAction->nSelect] );
	
	// 마법
	pAction = &pMode->aAction[STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber];
	m_aEditForm[ROW_MAGIC_1].Save( &pAction->aMotion[pAction->nSelect] );
	
	pAction = &pMode->aAction[STATE_MAGIC_ATTACK_DOWNANDUP + g_nMagicNumber];
	m_aEditForm[ROW_MAGIC_2].Save( &pAction->aMotion[pAction->nSelect] );
	
	pAction = &pMode->aAction[STATE_MAGIC_ATTACK_GO + g_nMagicNumber];
	m_aEditForm[ROW_MAGIC_3].Save( &pAction->aMotion[pAction->nSelect] );
	
	pAction = &pMode->aAction[STATE_MAGIC_ATTACK_AROUND + g_nMagicNumber];
	m_aEditForm[ROW_MAGIC_4].Save( &pAction->aMotion[pAction->nSelect] );

	pAction = &pMode->aAction[STATE_EMOTICON + g_nEmoticonNumber ];
	m_aEditForm[ROW_EMOTICON].Save( &pAction->aMotion[pAction->nSelect] );
} //CModeForm::Save
