/**
 * General Handle
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	ÆÄ¶õ
 * @date	$Date: 04-05-29 10:05p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /Common/Inc/Handle.h $
 * 
 * 2     04-05-29 10:05p Paran
 * 
 * 1     04-05-28 3:55p Paran
 * 
 * <HR>
 *
 * @file	Handle.h
 */

#include <vector>

template <typename TAG>
class CHandle
{
	union
	{
		enum 
		{ 
			MAX_BITS_INDEX = 16, 
			MAX_BITS_MAGIC = 16,
			MAX_INDEX = ( 1 << MAX_BITS_INDEX ) - 1,
			MAX_MAGIC = ( 1 << MAX_BITS_MAGIC ) - 1
		}; //enum

		struct  
		{
			unsigned m_Index : MAX_BITS_INDEX;
			unsigned m_Magic : MAX_BITS_MAGIC;
		}; //struct
		DWORD		m_dwHandle;
	}; //union

public:
	CHandle() : m_dwHandle( 0 ){};

	void		Init( const DWORD dwIndex );
	
	DWORD		GetIndex()	const	{ return m_Index; }
	DWORD		GetMagic()	const	{ return m_Magic; }
	DWORD		GetHandle() const	{ return m_dwHandle; }
	BOOL		IsNull()	const	{ return !m_dwHandle; }

	operator DWORD ()		const	{ return m_dwHandle; }
}; //class CHandle

/**
 * Initialize a handle 
 */
template <typename TAG>
void CHandle<TAG>::Init( const DWORD dwIndex )
{
	Assert( IsNull() );
	Assert( dwIndex  <= MAX_INDEX );

	static DWORD		s_dwAutoMagic = 0;
	if( ++s_dwAutoMagic > MAX_MAGIC )
	{
		s_dwAutoMagic = 1;
	} //if

	m_Index = dwIndex;
	m_Magic = s_dwAutoMagic;
} //CHandle<TAG>::Init

/**
 * Operator != 
 */
template <typename TAG>
inline BOOL operator != ( CHandle<TAG>l, CHandle<TAG>r )
{
	return l.GetHandle() != r.GetHandle();
} //inline BOOL operator !=

/**
 * Operator == 
 */
template <typename TAG>
inline BOOL operator == ( CHandle<TAG>l, CHandle<TAG>r )
{
	return l.GetHandle() == r.GetHandle();
} //inline BOOL operator ==


/**
 * Class CHandleMgr 
 */
template <typename DATA, typename HANDLE>
class CHandleMgr
{
private:
	typedef std::vector<DATA>			VEC_DATA;
	typedef std::vector<DWORD>			VEC_MAGIC;
	typedef std::vector<DWORD>			VEC_FREE;

	VEC_DATA		m_vecData;
	VEC_MAGIC		m_vecMagic;
	VEC_FREE		m_vecFreeSlot;

	DWORD			m_dwDereference;

public:
	CHandleMgr(){};
	~CHandleMgr(){};

	DATA*		Acquire( HANDLE& handle );
	void		Release( HANDLE handle );

	DATA*		Dereference( HANDLE handle );
	const DATA*	Dereference( HANDLE handle ) const;

	DWORD		GetUsedHandleCount() const			{ return ( m_vecMagic.size() - m_vecFreeSlot.size() ); }
	DWORD		HasUsedHandles()	 const			{ return ( !!GetUsedHandleCount() ); }

	DATA*		FirstDereference();
	DATA*		NextDereference();
}; //class CHandleMgr

/**
 * Acquire 
 */
template <typename DATA, typename HANDLE>
DATA* CHandleMgr<DATA, HANDLE>::Acquire( HANDLE& handle )
{
	DWORD		dwIndex;
	if( m_vecFreeSlot.empty() )
	{
		dwIndex = m_vecMagic.size();
		handle.Init( dwIndex );
		m_vecData.push_back( DATA() );
		m_vecMagic.push_back( handle.GetMagic() );
	}
	else
	{
		dwIndex = m_vecFreeSlot.back();
		handle.Init( dwIndex );
		m_vecData.pop_back();
		m_vecMagic[dwIndex] = handle.GetMagic();
	} //if..else

	return m_vecData.begin() + dwIndex;
} //CHandleMgr<DATA, HANDLE>::Acquire

/**
 * Release 
 */
template <typename DATA, typename HANDLE>
void CHandleMgr<DATA, HANDLE>::Release( HANDLE handle )
{
	DWORD		dwIndex  = handle.GetIndex();

	Assert( dwIndex <m_vecData.size() );
	Assert( m_vecMagic[dwIndex] == handle.GetMagic() );

	m_vecMagic[dwIndex] = 0;
	m_vecFreeSlot.push_back( dwIndex );
} //CHandleMgr<DATA, HANDLE>::Release

/**
 * Dereference 
 */
template <typename DATA, typename HANDLE>
inline DATA* CHandleMgr<DATA, HANDLE>::Dereference( HANDLE handle )
{
	if( handle.IsNull() )
	{
		return NULL;
	} //if

	DWORD		dwIndex = handle.GetIndex();
	if( ( dwIndex >= m_vecData.size() ) ||
		( m_vecMagic[dwIndex] != handle.GetMagic() ) )
	{
		Assert( 0 );
		return NULL;
	} //if

	return ( m_vecData.begin() + dwIndex );
} //CHandleMgr<DATA, HANDLE>::Dereference

/**
 * Dereference 
 */
template <typename DATA, typename HANDLE>
inline const DATA* CHandleMgr<DATA, HANDLE>::Dereference( HANDLE handle ) const
{
	typedef CHandleMgr<DATA, HANDLE>	ThisType;
	return ( const_cast<ThisType*>( this )->Dereference( handle ) );
} //CHandleMgr<DATA, HANDLE>::Dereference

/**
 * First Dereference
 */
template <typename DATA, typename HANDLE>
inline DATA* CHandleMgr<DATA, HANDLE>::FirstDereference()
{
	m_dwDereference = 0;

	DWORD		dwIndex = m_vecMagic[m_dwDereference];
	return m_vecData.begin() + dwIndex;
} //CHandleMgr<DATA, HANDLE>::FirstDereference

/**
 * Next Dereference
 */
template <typename DATA, typename HANDLE>
inline DATA* CHandleMgr<DATA, HANDLE>::NextDereference()
{
	if( ++m_dwDereference > m_vecMagic.size() )
	{
		return NULL;
	} //if

	DWORD		dwIndex = m_vecMagic[m_dwDereference];
	return m_vecData.begin() + dwIndex;
} //CHandleMgr<DATA, HANDLE>::NextDereference