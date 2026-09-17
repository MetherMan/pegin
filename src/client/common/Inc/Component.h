/**
 * Wind3D 컴포넌트 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-08-07 5:39p $
 * @version	$Revision: 17 $
 *
 * <HR>
 * $Log: /Common/Inc/Component.h $
 * 
 * 17    03-08-07 5:39p Paran
 * 
 * 16    03-05-10 12:49p Ahastudio
 *
 * 15    03-05-10 12:45p Ahastudio		AddComponent() 추가.
 * <HR>
 *
 * @file	Component.h
 */


#ifndef __Wind3D_Common_Component_Header__
#define __Wind3D_Common_Component_Header__


#define MAKE_COMPONENT( C )		( (IW3D##C*)MakeComponent( #C ) )


COMMON_API BOOL			AddComponent( ComponentInfo** apComponentInfo );
COMMON_API BOOL			LoadComponentPackage( const char* szFileName );
COMMON_API void			UnloadComponentPackage( const char* szFileName );
COMMON_API IComponent*	MakeComponent( const char* szName );
COMMON_API void			_ComponentReference( IComponent* pCom );
COMMON_API void			_ComponentRelease( IComponent* pCom );


/**
 * Component Base Class.
 * @ingroup		Common
 */
class IComponent
{
public:
	UNDER_MEMORY_MGR( IComponent );

	/**
	 * A constructor.
	 */
	IComponent()
	:	m_nReferenceCount( 0 )
	{
		_ComponentReference( this );
	} //IComponent

	/**
	 * Reference Count를 증가시킨다.
	 */
	void	AddRef()
	{
		_ComponentReference( this );
	} //AddRef

	/**
	 * Reference Count 알아낸다.
	 */
	int		GetRefCount()
	{
		return m_nReferenceCount;
	} //GetRefCount

	/**
	 * Component 해제.
	 * Reference Count를 감소시키고, 참조가 없을 경우 완전 삭제한다.
	 */
	virtual void	Release()
	{
		_ComponentRelease( this );
	} //Release

protected:
	/**
	 * A destructor.
	 */
	virtual ~IComponent()
	{
	} //~IComponent

private:
	int		m_nReferenceCount;		///< 레퍼런스 카운트

	friend COMMON_API void	_ComponentReference( IComponent* pCom );
	friend COMMON_API void	_ComponentRelease( IComponent* pCom );
}; //class IComponent


/*
 * Component Information
 * @ingroup		Common
 */
struct ComponentInfo
{
	const char*		szName;				///< Component 이름
	IComponent*		(*Factory)();		///< Factory 함수
}; //struct ComponentInfo


// Component Export --------------------------------------------------------------

#define IMPLEMENT_COMPONENT( I, C )		\
	static IComponent* Component_Factory_##C()	\
	{	\
		return new I;	\
	}	\
	ComponentInfo Component_##C =	\
	{	\
		#C,	\
		Component_Factory_##C	\
	};

#define SINGLETON_COMPONENT( I )	\
	private:						\
		static I* ms_pSingleton;	\
	public:							\
		static I* GetSingleton()	\
		{							\
			return ms_pSingleton;	\
		}							\
		void SetSingleton()			\
		{							\
			ms_pSingleton = this;	\
		}							\
		void Release()				\
		{							\
			ms_pSingleton = NULL;	\
			IComponent::Release();	\
		}

#define IMPLEMENT_SINGLETON_COMPONENT( I, C )	\
	I*	I::ms_pSingleton = NULL;				\
	static IComponent* Component_Factory_##C()	\
	{	\
		if( I::GetSingleton() )			\
		{	\
			return I::GetSingleton();	\
		}	\
		I* pCom = new I;		\
		Assert( pCom );			\
		pCom->SetSingleton();	\
		return pCom;			\
	}	\
	ComponentInfo Component_##C =	\
	{	\
		#C,	\
		Component_Factory_##C	\
	};


#define DECLARE_COMPONENT( C )	extern ComponentInfo	Component_##C;


#define BEGIN_PACKAGE_LIST	\
	extern "C" DLL_EXPORT ComponentInfo**	Package()	\
	{	\
		static ComponentInfo*	apComponentInfo[] =	\
		{

#define COMPONENT( C )		&Component_##C,

#define END_PACKAGE_LIST	\
		0		\
		};		\
		return apComponentInfo;	\
	}


#endif // #ifndef __Wind3D_Common_Component_Header__
