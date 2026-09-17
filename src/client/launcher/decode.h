#ifndef __DECODE_H__
#define __DECODE_H__

#include <stdio.h>
#include <limits.h>


typedef UINT               UCRC;
typedef UINT               uint;
typedef UINT               ulong;
typedef UCHAR              uchar;
typedef unsigned short     ushort;


#define BINARY_TYPE        0    /* This must line up with binary/text strings */
#define TEXT_TYPE          1
#define COMMENT_TYPE       2
#define DIR_TYPE           3
#define LABEL_TYPE         4
#define BUFFERSIZE         4096
#define STRTP              9
#define STOPP              13
#define STRTL              0
#define STOPL              7
#define DEFAULT_DIR         ""
#define ARJ_X_VERSION      3    /* decoder version */
#define GARBLE_FLAG        0x01
#define VOLUME_FLAG        0x04
#define EXTFILE_FLAG       0x08
#define PATHSYM_FLAG       0x10
#define BACKUP_FLAG        0x20
#define MAXMETHOD          4

#define PBIT               5
#define CODE_BIT           16
#define MATCHBIT           8
#define MAXDICBIT          16
#define NP                 (MAXDICBIT + 1)
#define NT                 (CODE_BIT + 3)
#if NT > NP
#define NPT NT
#else
#define NPT NP
#endif
#define CHAR_BIT           8
#define DDICSIZ            26624
#define PATH_CHAR          '\\'
#define ARJ_PATH_CHAR      '/'
#define PATHSYM_FLAG       0x10
#define NULL_CHAR          '\0'
#define HEADER_ID          0xEA60
#define CRC_MASK           0xFFFFFFFF
#define CRCPOLY            0xEDB88320
#define HEADER_ID_LO       0x60
#define HEADER_ID_HI       0xEA
#define FIRST_HDR_SIZE     30
#define COMMENT_MAX        2048
#define FNAME_MAX          512
#define HEADERSIZE_MAX     (FIRST_HDR_SIZE + 10 + FNAME_MAX + COMMENT_MAX)
#define MAXMATCH           256
#define THRESHOLD          3
#define TBIT               5
#define CBIT               9
#define NC                 (UCHAR_MAX + MAXMATCH + 2 - THRESHOLD)
#define CTABLESIZE         4096


#define UPDATE_CRC(r,c)    r=crctable[((uchar)(r)^(uchar)(c))&0xff]^(r>>CHAR_BIT)
#define fget_crc(f)        fget_longword(f)
#define setup_get(PTR)     (get_ptr = (PTR))
#define get_byte()         ((uchar)(*get_ptr++ & 0xff))
#define get_crc()          get_longword()
#define BFIL               {getbuf|=bitbuf>>getlen;fillbuf(CODE_BIT-getlen);getlen=CODE_BIT;}
#define GETBIT(c)          {if(getlen<=0)BFIL c=(getbuf&0x8000)!=0;getbuf<<=1;getlen--;}
#define BPUL(l)            {getbuf<<=l;getlen-=l;}
#define GETBITS(c,l)       {if(getlen<l)BFIL c=(ushort)getbuf>>(CODE_BIT-l);BPUL(l)}


int UpdateDecode( const char* szFileName, int com );
static long find_header(FILE *fd);
BOOL IsValidHeader(char *filename);

#endif // #ifndef __DECODE_H__
