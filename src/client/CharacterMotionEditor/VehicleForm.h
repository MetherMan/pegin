#ifndef __VEHICLEFORM_HEADER__
#define __VEHICLEFORM_HEADER__

#define MAXEDITFORM	13

/**
 *
 */
class CVehicleForm
{
public:
	~CVehicleForm();
	CVehicleForm();

	BOOL	Create( int nX, int nY, CWnd* pWnd );

private:

	DWORD		m_dwId;
	CEdit		m_aEdit[MAXEDITFORM];

}; //class CModeForm


#endif //#ifndef __EDITFORM_HEADER__