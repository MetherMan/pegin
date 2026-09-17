
#ifndef __MODEFORM_HEADER__
#define __MODEFORM_HEADER__

#include "EditForm.h"

#define MAXEDITFORM	13

/**
 *
 */
class CModeForm
{
public:
	~CModeForm();
	CModeForm();

	BOOL	Create( int nX, int nY, CWnd* pWnd );

	void	Set( int nIndex, const SMotion& motion )
	{
		m_aEditForm[nIndex].Set( motion );
	} //

	void	Set( const SMode& mode );

	void	Select();
	void	Show( SMode* pMode );
	void	Save( SMode* pMode );


private:
	
	CEditForm	m_aEditForm[MAXEDITFORM];
}; //class CModeForm


#endif //#ifndef __EDITFORM_HEADER__