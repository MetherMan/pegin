#ifndef __CHINA_VER__
#include "DaumGameCrypt.h"
#include <malloc.h>

CDaumGameCrypt::CDaumGameCrypt( void ) : m_bInit( FALSE )
{
	Init();
}

CDaumGameCrypt::~CDaumGameCrypt( void )
{
	Destroy();
}

BOOL CDaumGameCrypt::Init( void )
{
	if( m_bInit == TRUE )
		return FALSE;
	m_pCryptor	= (daum_encrypt_type *) malloc( sizeof( daum_encrypt_type ) );
	memset( m_pCryptor, 0, sizeof( daum_encrypt_type ) );
	m_bInit = TRUE;
	return TRUE;
}

void CDaumGameCrypt::Destroy( void )
{
	if( m_bInit == FALSE )
		return;
	if( m_pCryptor ) {
		free( m_pCryptor );
		m_pCryptor = NULL;
	}
	m_bInit = FALSE;
}

BOOL CDaumGameCrypt::SetKey( LPCTSTR szKey )
{
	daum_encrypt_init( m_pCryptor, (LPTSTR) szKey );
	return TRUE;
}

BOOL CDaumGameCrypt::Encrypt( IN LPCTSTR szText, OUT LPTSTR szEncryptText, int nMaxSize )
{
	if( daum_encrypt( m_pCryptor, (LPTSTR) szText, _tcslen( szText ), szEncryptText, &nMaxSize ) == -1 )
			return FALSE;
	return TRUE;
}

BOOL CDaumGameCrypt::Decrypt( IN LPCTSTR szEncryptedText, OUT LPTSTR szText, int nMaxSize )
{
	if( daum_decrypt( m_pCryptor, (LPTSTR) szEncryptedText, _tcslen( szEncryptedText ), szText, &nMaxSize ) == -1 )
			return FALSE;
	return TRUE;
}
#endif // __CHINA_VER__

// EOF




