/**
 * 다음 인증처리 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	전홍
 * @date	$Date: 03-10-10 1:20p $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /GameClient/Src/DaumLogin.h $
 * 
 * 4     03-10-10 1:20p Kjmgo
 * Login 에 관련하여, 몇가지 수정및, 로그가 남도록 수정 
 * 
 * 3     03-06-24 5:48p Paran
 * 
 * 2     03-06-24 11:16a Paran
 * 
 * <HR>
 *
 * @file	DaumLogin.h
 */

#if !defined(AFX_DAUMLOGIN_H__1D3D24C8_B638_44DC_8206_3999B996DE2D__INCLUDED_)
#define AFX_DAUMLOGIN_H__1D3D24C8_B638_44DC_8206_3999B996DE2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDaumLogin  
{
public:
	void		Init();
	void		Close();
	BOOL		Connect();

	char*		GetTS();
	char*		GetPROF();
	char*		GetHTS();
	char*		GetHMCU();
	char*		GetAuthBuffer();
	int			GetAuthResult();
	
	BOOL		RecvData();
	void		SendLoginInfo( const char *id, const char *pw );
	
	CDaumLogin();
	virtual ~CDaumLogin();

private:
	SOCKET		m_Sock;
	int			m_resultStringLen;
	char		m_resultString[40960];
	int			m_recvSize;
	char		m_recvBuff[4096];
	
	char		m_HMCU_Buffer[40960];
	char		m_TS_Buffer[40960];
	char		m_HTS_Buffer[40960];
	char		m_PROF_Buffer[40960];

	string		m_sAccountHost;

	BOOL		Send( void* pData, int nSize );
	BOOL		Recv();
}; //CDaumLogin

#endif // !defined(AFX_DAUMLOGIN_H__1D3D24C8_B638_44DC_8206_3999B996DE2D__INCLUDED_)
