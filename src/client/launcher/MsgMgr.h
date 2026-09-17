#ifndef __MSGMGR_HEADER__
#define __MSGMGR_HEADER__

#include <map>
#include <string>

typedef std::map<int, std::string>		MSGMAP;

class CMsgMgr
{
public :
	CMsgMgr();
	~CMsgMgr();

	void			Init();

	const char*		GetString( int nNum );
	int				GetSize();
	void			ReadScript( const char* szFileName );

private :	

	MSGMAP			m_mapMsg;
};

#endif // #ifndef __MSGMGR_HEADER__