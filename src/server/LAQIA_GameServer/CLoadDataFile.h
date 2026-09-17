#ifndef __LOAD_DATA_FILE_H__
#define __LOAD_DATA_FILE_H__

#ifndef NULL_STR
#define NULL_STR	'\0'
#endif

#ifndef SAFE_FCLOSE
#define SAFE_FCLOSE( p ) { fclose( p ); p = NULL; }
#endif

/*
 *	CLoadDataFile
 */
class CLoadDataFile
{
public:
	enum
	{
		IS_EOF	=	0,
		IS_SKIP,
		IS_OK
	};

	/*
	 *	생성자 
	 */
	CLoadDataFile()
	{
		fp = NULL;
		*m_readBuff = NULL_STR;
		nPos = 0;
	};
	
	/*
	 *	소멸자 
	 */
	~CLoadDataFile()
	{
		Close();
	};

	/*
	 *	파일 로드 
	 */
	BOOL	Load( char *filename )
	{
		fp = fopen( filename, "rb" );

		if( !fp )
			return 0;

		return 1;
	};

	/*
	 *	파일포인터 해제 
	 */
	void	Close()
	{
		if( fp )
		{
			SAFE_FCLOSE( fp );
		}
	};

	/*
	 *	한줄 읽어오기 ( max 1024 )
	 */
	int		GetLine()
	{
		if( feof( fp ) )
			return IS_EOF;

		nPos = 0;

		if( fgets( m_readBuff, 4096, fp ) == NULL )
			return IS_EOF;
		
		if( *m_readBuff == ';' )
			return IS_SKIP;

		if( *m_readBuff == '\n' ||
			*m_readBuff == '\r' ||
			*m_readBuff == '\0' ||
			*m_readBuff == ' ' ||
			*m_readBuff == '#' )
			return IS_EOF;	
		
		return IS_OK;
	};

	/*
	 *	string 
	 */
	void Get( char *str )
	{
		int tmpPos = nPos;
		
		for( int i = 0; i < 128; i++ )
		{
			if( m_readBuff[tmpPos] == '\t' ||
				m_readBuff[tmpPos] == '\r' ||
				m_readBuff[tmpPos] == '\n' )
			{
				strncpy( str, &m_readBuff[nPos], tmpPos - nPos );
				str[tmpPos-nPos] = NULL_STR;
				nPos = tmpPos+1;
				return;
			}
			tmpPos++;
		}
	};

	/*
	 *	numberic
	 */
	int Get()
	{
		int tmpPos = nPos;
		char str[128];
		
		for( int i = 0; i < 128; i++ )
		{
			if( m_readBuff[tmpPos] == '\t' ||
				m_readBuff[tmpPos] == '\r' ||
				m_readBuff[tmpPos] == '\n' )
			{
				strncpy( str, &m_readBuff[nPos], tmpPos - nPos );
				str[tmpPos-nPos] = NULL_STR;
				nPos = tmpPos+1;
				return atoi( str );
			}
			tmpPos++;
		}
		return 0;
	};

private:
	FILE	*fp;
	int		nPos;
	char	m_readBuff[4096];
};

#endif

//EOF

