#pragma once

#include <list>

// _CDaumGameMemoryBlock is needed to CDaumGameParameter
class _CDaumGameMemoryBlock;

class CDaumGameParameter
{
public:
	CDaumGameParameter( void );
	~CDaumGameParameter( void );

	BOOL	Init( void );
	BOOL	Flush( void );
	void	Empty( void );
	void	Destroy( void );

	BOOL	Set( LPSTR lpBuffer );
	BOOL	Set( LPCSTR lpszKeyName, LPSTR lpBuffer );
	BOOL	Get( LPCSTR lpszKeyName, LPSTR lpBuffer, DWORD nSize );

protected:
	UINT	m_uDGPFormat;
	BOOL	m_bModified;
	_CDaumGameMemoryBlock *m_pMemoryBlock;
};

class _CDaumGameMemoryBlock
{
public:
	_CDaumGameMemoryBlock( void );
	~_CDaumGameMemoryBlock( void );

	BOOL Assign( HANDLE hBlock );
	HANDLE CreateHandle( void );

	LPSTR Find( LPCSTR lpszKeyName );
	BOOL Put( LPSTR lpBuffer );
	BOOL Put( LPCSTR lpszKeyName, LPSTR lpBuffer );
	BOOL Get( LPCSTR lpszKeyName, LPSTR lpBuffer, DWORD nSize );

protected:
	typedef std::list<CHAR *>	StrList;
	typedef StrList::iterator	StrListItr;

	StrList m_StrList;
};
