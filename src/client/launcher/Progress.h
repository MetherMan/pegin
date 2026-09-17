/********************************************************************

	Progress.h

********************************************************************/

#ifndef	__Property_Header__
#define	__Property_Header__


#include <windows.h>


class CProgress
{
public:
	CProgress( void );
	~CProgress( void );

	BOOL	Create( int nWidth, int nHeight );
	void	SetRange( int nRange );
	void	SetPos( int nPos );
	void	Draw( int x, int y, HDC hDC );

private:
	int		m_nRange;
	int		m_nWidth;
	int		m_nHeight;
	int		m_nStep;
};


#endif