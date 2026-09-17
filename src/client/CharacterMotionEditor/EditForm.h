
#ifndef __EDITFORM_HEADER__
#define __EDITFORM_HEADER__

/**
 *
 */
class CEditForm
{
public:
	~CEditForm();
	CEditForm();

	BOOL	Create( int nX, int nY, CWnd* pWnd, BOOL bShow = TRUE );

	
	void	Set( const SMotion& motion );
	void	Save( SMotion* motion );
	void	Select();

	char*	GetAniFile();
	char*	GetSoundFile();
	DWORD	GetBeginFrame();
	DWORD	GetEndFrame();
	BOOL	IsUse();


private:
	CEdit	m_BeginFrame;
	CEdit	m_EndFrame;
	CEdit	m_WeaponFile;
	CEdit	m_AniFile;
	CEdit	m_SoundFile;

	DWORD	m_dwId;
}; //class CEditForm




#endif //#ifndef __EDITFORM_HEADER__