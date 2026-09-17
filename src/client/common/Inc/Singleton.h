/**
 * ½Ì±ÛÅæ ¸ðµâ.
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	¾Æ¼£
 * @date	$Date: 03-03-25 9:27a $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Common/Inc/Singleton.h $
 * 
 * 3     03-03-25 9:27a Ahastudio
 *
 * 2     03-02-28 2:48p Ahastudio	SingletonÀ» Common¿¡ Ãß°¡.
 * <HR>
 *
 * @file	Singleton.h
 */


#ifndef __Singleton_Header__
#define __Singleton_Header__


/**
 * ½Ì±ÛÅæ º£ÀÌ½º Å¬·¡½º.
 * @ingroup		Common
 */
template <typename T>
class Singleton
{
public:
	/**
	 * »ý¼ºÀÚ.
	 */
	Singleton()
	{
		Assert( NULL == ms_pSingleton );
		int offset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;
		ms_pSingleton = (T*)( (int)this + offset );
	} //Singleton

	/**
	 * ¼Ò¸êÀÚ.
	 */
	~Singleton()
	{
		Assert( ms_pSingleton );
		ms_pSingleton = NULL;
	} //~Singleton

	/**
	 * ½Ì±ÛÅæ °´Ã¼¸¦ ¾ò´Â´Ù.
	 *
	 * @return	Singleton Object
	 */
	static T& GetSingleton()
	{
		Assert( ms_pSingleton );
		return *ms_pSingleton;
	} //GetSingleton

	/**
	 * ½Ì±ÛÅæ °´Ã¼ÀÇ Æ÷ÀÎÅÍ¸¦ ¾ò´Â´Ù.
	 *
	 * @return	Singleton Object
	 */
	static T* GetSingletonPtr()
	{
		Assert( ms_pSingleton );
		return ms_pSingleton;
	} //GetSingletonPtr

protected:
	static	T*		ms_pSingleton;		///< ½Ì±ÛÅæ °´Ã¼
}; //class Singleton


template <typename T>
T* Singleton <T>::ms_pSingleton = NULL;


#endif // #ifndef __Singleton_Header__
