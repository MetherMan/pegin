/********************************************************************

	Bitmap.h

********************************************************************/


#ifndef	__Bitmap_Header__
#define	__Bitmap_Header__


#include <windows.h>


namespace my
{
	class CBitmap
	{
	public:
		CBitmap();
		~CBitmap();

		HRESULT		Load( LPCSTR szFile );
		HRESULT		ReLoad( LPCTSTR szFile );
		HRESULT		Draw( int x, int y, HDC hDest );

		int			GetWidth()
		{
			return m_nWidth;
		}

		int			GetHeight()
		{
			return m_nHeight;
		}

		HBITMAP		GetBitmap()
		{
			return m_hBitmap;
		}

	private:
		HBITMAP		m_hBitmap;
		int			m_nWidth;
		int			m_nHeight;
	}; //class CBitmap
} //namespace my


#endif // #ifndef	__Bitmap_Header__
