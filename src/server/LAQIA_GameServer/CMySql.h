#ifndef __CMYSQL_H__
#define __CMYSQL_H__

#include <stdarg.h>
#include <mysql.h>

#ifdef WIN32
#pragma comment( lib, "libmysql.lib" )
#endif

class CMySql
{
public:
	CMySql()
	{
		pDB = NULL;
		result = NULL;
	};
	
	~CMySql()
	{
		Close();
	};

	/*
	 *	DB 연결
	 */
	BOOL DBConnect( char *host, int port, char *id, char *pw , char *dbName )
	{
		pDB = mysql_init( (MYSQL*)NULL );		// 초기화 함수
		
		if( pDB )
		{
			if( mysql_real_connect( pDB, host, id, pw, NULL, port, (char*)NULL, 0 ) ) // DB 접속 
			{
				if( mysql_select_db( pDB, dbName ) != 0 ) // DB 선택
				{
					mysql_close( pDB );
					return 0;
				}
			}
			else
			{
				mysql_close( pDB );
				return 0;
			}
		}
		else
			return 0;
		return 1;
	};

	/*
	 *	DB 연결 종료 
	 */
	void Close()
	{
		if( pDB )
		{
			mysql_close( pDB );
			pDB = NULL;
		}
	};
	
	/*
	 *	쿼리 전송
	 */
	int Query( char *query, ... )
	{
		char tmp_buf[1024];
		va_list args;
		
		va_start( args, query );
		vsprintf( tmp_buf, query, args );
		va_end( args );

		return mysql_query( pDB, tmp_buf );
	};

	/*
	 *	문자데이터 변환 
	 */
	char *StringConvert( char *szStr, char *szResult )
	{
		*szResult = NULL_STR;
		
		mysql_escape_string( szResult, szStr, strlen( szStr ) );

		return szResult;
	};

	/*
	 *	row 갯수 구하기
	 */
	int GetRowCount()
	{
		return (int)mysql_num_rows( result );
	};
	

	/*
	 *	에러 원인 얻기
	 */
	char *GetLastError()
	{
		return (char*)mysql_error( pDB );
	};

	/*
	 *	result 얻기
	 */
	BOOL GetResult()
	{
		result = mysql_store_result( pDB );

		if( result )
			return 1;
		return 0;
	};

	/*
	 *	row 값 얻기 
	 */
	BOOL GetRowData()
	{
		if( ( row = mysql_fetch_row( result ) ) )
			return 1;
		return 0;
	};

	/*
	 *	result 해제
	 */
	void FreeResult()
	{
		mysql_free_result( result );
	};

	/*
	 *	Row값 int 형으로 반환 
	 */
	int	GetInt( int rowNum )
	{
		return atoi( row[rowNum] );
	};
	
	/*
	 *	Row 값 long 형으로 반환 
	 */
	long GetLong( int rowNum )
	{
		return atol( row[rowNum] );
	};

	/*
	 *	Row 값 float 형으로 반환 
	 */
	float GetFloat( int rowNum )
	{
		return (float)atof( row[rowNum] );
	};

	/*
	 *	Row 값 
	 */
	char *GetRow( int rowNum )
	{
		return row[rowNum];
	};

	/*
	 *	연결 상태 유지 
	 */
	void ConnectionKeeper()
	{
		mysql_ping( pDB );
	};

private:
	MYSQL			*pDB;
	MYSQL_RES		*result;
	MYSQL_ROW		row;
};

#endif

//EOF

