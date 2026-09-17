/**
 * 메모리 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 05-02-19 7:10p $
 * @version	$Revision: 20 $
 *
 * <HR>
 * $Log: /Common/Inc/Memory.h $
 * 
 * 20    05-02-19 7:10p Sk8snow2
 * 
 * 19    04-12-08 3:06a Sk8snow2
 * 
 * 18    04-11-16 5:32p Sk8snow2
 * 
 * 17    04-01-16 8:03p Kjmgo
 * 
 * 16    04-01-08 9:03a Kjmgo
 * 
 * 15    03-12-14 2:33p Kjmgo
 * 
 * 14    03-08-22 5:27p Kjmgo
 * 
 * 13    03-04-25 7:21p Ahastudio
 *
 * 10    03-03-17 7:32a Ahastudio	DEBUG에서 메모리 할당시 디버그 정보 입력 방법 변경.
 *
 * 9     03-03-13 3:54p Ahastudio	DWORD 메모리 전송 추가.
 * <HR>
 *
 * @file	Memory.h
 */


#ifndef __Wind3D_Common_Memory_Header__
#define __Wind3D_Common_Memory_Header__


/**
 * 메모리 관리자.
 * @ingroup		Common
 */
class COMMON_API MemoryMgr
{
public:
	MemoryMgr();
	~MemoryMgr();

	void*	Alloc( int size );
	void*	Alloc( int nSize, const char* szFile, int nLine );
	void	Free( void* ptr );

	BOOL	CheckBounce( void* ptr );
	BOOL	CheckFree();

	DWORD	GetCount()
	{
		return m_nNumMemAlloc;
	} //GetCount

	DWORD	GetSize()
	{
		return m_dwTotalSize; 
	} //GetSize
private:
	enum
	{ 
		MAX_ALLOCATION	= 1024 * 100,		///< 최대 메모리 할당 블럭 갯수
		PADDING_SIZE	= 1024 * 10,		///< 경계 검사를 위한 삽입 사이즈 ( 10KB )
		GARBAGE			= 0xCC				///< 경계 검사를 위한 blank 값
	}; //enum

	/**
	 * 메모리 블럭 정보
	 */
	struct MemBlockInfo
	{
		BOOL		bAllocated;			///<

		void*		pActualPtr;			///<
		int			nActualSize;		///<

		void*		ptr;				///<
		int			size;				///<

		char		szFile[256];		///<
		int			nLine;				///<
	}; //struct MemBlockInfo

	int				m_nNumMemAlloc;					///<

#ifdef _DEBUG
	MemBlockInfo	m_aMemBlock[MAX_ALLOCATION];	///<
#endif //#ifdef _DEBUG

	char			m_szFile[256];					///<
	int				m_nLine;						///<

	DWORD			m_dwTotalSize;				///< 총 생성한 메모리 
	DWORD			m_dwGarbageSize;			///< 남은 쓰레기 메모리
}; //class COMMON_API MemoryMgr


extern COMMON_API MemoryMgr		GlobalMemory;


#ifdef _DEBUG
	#define UNDER_MEMORY_MGR( C )					\
		void*	operator new( unsigned int size )	\
		{											\
			char	szFile[256];					\
			strcpy( szFile, __FILE__ );				\
			strcat( szFile, " - " );				\
			strcat( szFile, #C );					\
			return GlobalMemory.Alloc( size, szFile, __LINE__ );	\
		}											\
		void	operator delete( void* ptr )		\
		{											\
			_MemFree( ptr );						\
		}
#else
	#define UNDER_MEMORY_MGR( C )					\
		void*	operator new( unsigned int size )	\
		{											\
			return _MemAlloc( size );				\
		}											\
		void	operator delete( void* ptr )		\
		{											\
			_MemFree( ptr );						\
		}
#endif


/**
 * 메모리 복사.
 * @ingroup		Common
 *
 * @param	pDest		[IN]
 * @param	pSrc		[IN]
 * @param	nCount		[IN]
 */
inline void _MemCpy( void* pDest, const void* pSrc, int nCount )
{
	__asm
	{
		// 인자들을 레지스터에 넣어준다.
		mov		esi, pSrc
		mov		edi, pDest
		mov		ecx, nCount

		// BX = Byte 전송량
		// CX = DWord 전송량
		mov     ebx, ecx
		shr     ecx, 2
		and     ebx, 3

		// DWORD 전송
		rep     movsd

		// BYTE 전송
		mov     ecx, ebx
		rep     movsb
	} //__asm
} //_MemCpy

/**
 * 메모리 채움.
 * @ingroup		Common
 *
 * @param	pDest		[IN]
 * @param	bData		[IN]
 * @param	nCount		[IN]
 */
inline void _MemSet( void* pDest, const BYTE bData, const int nCount )
{
	__asm
	{
		// BYTE의 Data를 DWORD로 만들어줌
		mov		al, [bData]
		mov		ah, al
		mov		bx, ax
		shl		eax, 16
		mov		ax, bx

		// 인자들을 레지스터에 넣는다
		mov		edi, [pDest]
		mov		ecx, [nCount]

		// BX = Byte 세팅량
		// CX = DWord 세팅량
		mov		ebx, ecx
		shr		ecx, 2
		and		ebx, 3

		// DWORD 채움
		rep     stosd

		// BYTE 채움
		mov     ecx, ebx
		rep     stosb
	} //__asm
} //_MemSet

/**
 * WORD 데이터로 메모리 채움.
 * @ingroup		Common
 *
 * @param	pDest		[IN]
 * @param	wData		[IN]
 * @param	nCount		[IN]
 */
inline void _WMemSet( void* pDest, const WORD wData, const int nCount )
{
	__asm
	{
		// WORD의 Data를 DWORD로 만들어줌
		mov		ax, [wData]
		mov		bx, ax
		shl		eax, 16
		mov		ax, bx

		// 인자들을 레지스터에 넣는다
		mov		edi, [pDest]
		mov		ecx, [nCount]

		// BX = Word 세팅량
		// CX = DWord 세팅량
		mov		ebx, ecx
		shr		ecx, 1
		and		ebx, 1

		// DWORD 채움
		rep     stosd

		// WORD 채움
		mov     ecx, ebx
		rep     stosw
	} //__asm
} //_WMemSet

/**
 * DWORD 데이터로 메모리 채움.
 * @ingroup		Common
 *
 * @param	pDest		[IN]
 * @param	dwData		[IN]
 * @param	nCount		[IN]
 */
inline void _DWMemSet( void* pDest, const DWORD dwData, const int nCount )
{
	__asm
	{
		// 인자들을 레지스터에 넣는다
		mov		eax, [dwData]
		mov		edi, [pDest]
		mov		ecx, [nCount]

		// DWORD 채움
		rep     stosd
	} //__asm
} //_DWMemSet

/**
 * 메모리 초기화
 * @ingroup		Common
 *
 * @param	pDest		[IN]
 * @param	nCount		[IN]
 */
inline void _ZeroMemory( void* pDest, int nCount )
{
	__asm
	{
		xor		eax, eax
		mov		edi, [pDest]
		mov		ecx, [nCount]

		mov		ebx, ecx
		shr		ecx, 2
		and		ebx, 3

		rep     stosd

		mov     ecx, ebx
		rep     stosb
	} //__asm
} //_ZeroMemory

/**
 * 메모리 할당.
 * @ingroup		Common
 *
 * @param	nSize		[IN]
 *
 * @return	할당된 메모리 주소
 */
#ifdef _DEBUG
	#define _MemAlloc( nSize )		GlobalMemory.Alloc( nSize, __FILE__, __LINE__ )
	#define CHECK_BOUNCE( p )		GlobalMemory.CheckBounce( p )
#else
	inline void*	_MemAlloc( int nSize )
	{
		return GlobalMemory.Alloc( nSize );
	}

	#define CHECK_BOUNCE( p )		
#endif

/**
 * 메모리 해제.
 * @ingroup		Common
 *
 * @param	pMemBlock	[IN]
 */
inline void _MemFree( void* pMemBlock )
{
	GlobalMemory.Free( pMemBlock );
} //_MemFree


#endif // #ifndef __Wind3D_Common_Memory_Header__

