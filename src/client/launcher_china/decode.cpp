#include "StdAfx.h"
#include <direct.h>

#include "Decode.h"

static char		gAPP_DIR[MAX_PATH];			///< Application Directory

static char M_CANTREAD[] = "Can't read file or unexpected end of file";
static char M_NOTARJ  [] = "%s is not an UDT archive";
static char M_BADHEADR[] = "Bad header";
static char M_HEADRCRC[] = "Header CRC error!";
static char M_BADTABLE[] = "Bad Huffman code";
static char M_CANTWRIT[] = "Can't write file. Disk full?";
static char M_BADCOMNT[] = "Invalid comment header";

static char   arc_name[FNAME_MAX];
static char   filename[FNAME_MAX];
static char   comment[COMMENT_MAX];
static char   *hdr_filename;
static char   *hdr_comment;

static int    command;    
static short  getbuf;
static short  getlen;
static long   first_hdr_pos;
static char   *writemode[2]  = { "wb",  "w" };
static int    no_output;
static ushort c_table[CTABLESIZE];
static uchar  c_len[NC];
static ushort left[2 * NC - 1];
static ushort right[2 * NC - 1];
static ushort pt_table[256];
static uchar  pt_len[NPT];
static int    bitcount;
static uchar  subbitbuf;
static ushort bitbuf;
static ushort blocksize;
static uchar  *text = NULL;
static FILE   *arcfile;
static FILE   *outfile;
static ushort host_data;
static uint   file_mode;
static short  entry_pos;
static UCRC   file_crc;
static long   compsize;
static ulong   time_stamp;
static int     file_type;
static short   method;
static uchar   arj_flags;
static uchar   host_os;
static uchar   arj_x_nbr;
static uchar   arj_nbr;
static uchar   first_hdr_size;
static uchar   *get_ptr;
static UCRC    header_crc;
static long    origsize;
static UCRC    crc;
static UCRC    crctable[UCHAR_MAX + 1];
static ushort  headersize;
static uchar   header[HEADERSIZE_MAX];


void disp_clock()
{	
}



static void error(char *fmt, char *arg)
{
/*
char text[80];
sprintf(text, fmt, arg);
MessageBox(gHWND_APP, text, NULL, MB_OK|MB_ICONWARNING);
PostMessage(gHWND_APP, WM_QUIT, 0, 0);
	*/
}

static void make_table(int nchar, uchar *bitlen, int tablebits, ushort *table)
{
    ushort count[17], weight[17], start[18], *p;
    uint i, k, len, ch, jutbits, avail, nextcode, mask;
	
    for (i = 1; i <= 16; i++)
        count[i] = 0;
    for (i = 0; (int)i < nchar; i++)
        count[bitlen[i]]++;
	
    start[1] = 0;
    for (i = 1; i <= 16; i++)
        start[i + 1] = start[i] + (count[i] << (16 - i));
    if (start[17] != (ushort) (1 << 16))
        error(M_BADTABLE, "");
	
    jutbits = 16 - tablebits;
    for (i = 1; (int)i <= tablebits; i++)
    {
        start[i] >>= jutbits;
        weight[i] = 1 << (tablebits - i);
    }
    while (i <= 16)
    {
        weight[i] = 1 << (16 - i);
        i++;
    }
	
    i = start[tablebits + 1] >> jutbits;
    if (i != (ushort) (1 << 16))
    {
        k = 1 << tablebits;
        while (i != k)
            table[i++] = 0;
    }
	
    avail = nchar;
    mask = 1 << (15 - tablebits);
    for (ch = 0; (int)ch < nchar; ch++)
    {
        if ((len = bitlen[ch]) == 0)
            continue;
        k = start[len];
        nextcode = k + weight[len];
        if ((int)len <= tablebits)
        {
            for (i = start[len]; i < nextcode; i++)
                table[i] = ch;
        }
        else
        {
            p = &table[k >> jutbits];
            i = len - tablebits;
            while (i != 0)
            {
                if (*p == 0)
                {
                    right[avail] = left[avail] = 0;
                    *p = avail++;
                }
                if (k & mask)
                    p = &right[*p];
                else
                    p = &left[*p];
                k <<= 1;
                i--;
            }
            *p = ch;
        }
        start[len] = nextcode;
    }
}

static void strncopy(char *to, char *from, int len)
{
    int i;
	
    for (i = 1; i < len && *from; i++)
        *to++ = *from++;
    *to = NULL_CHAR;
}



static void crc_buf(char *str, int len)
{
    while (len--)
        UPDATE_CRC(crc, *str++);
}

static void fread_crc(uchar *p, int n, FILE *f)
{
    n = fread((char *)p, 1, n, f);
    origsize += n;
    crc_buf((char *)p, n);
}


static int fget_byte(FILE *f)
{
    int c;
	
    if ((c = getc(f)) == EOF)
        error(M_CANTREAD, "");
    return c & 0xFF;
}

static uint fget_word(FILE *f)
{
    uint b0, b1;
	
    b0 = fget_byte(f);
    b1 = fget_byte(f);
    return (b1 << 8) + b0;
}

static ulong fget_longword(FILE *f)
{
    ulong b0, b1, b2, b3;
	
    b0 = fget_byte(f);
    b1 = fget_byte(f);
    b2 = fget_byte(f);
    b3 = fget_byte(f);
    return (b3 << 24) + (b2 << 16) + (b1 << 8) + b0;
}

static uint get_word()
{
    uint b0, b1;
	
    b0 = get_byte();
    b1 = get_byte();
    return (b1 << 8) + b0;
}

static ulong get_longword()
{
	ulong b0, b1, b2, b3;
	
	b0 = get_byte();
	b1 = get_byte();
	b2 = get_byte();
	b3 = get_byte();
	return (b3 << 24) + (b2 << 16) + (b1 << 8) + b0;
}

static void make_crctable()
{
	UINT i, j;
	UCRC r;
	
	for (i = 0; i <= UCHAR_MAX; i++)
	{
		r = i;
		for (j = CHAR_BIT; j > 0; j--)
		{
			if (r & 1)
				r = (r >> 1) ^ CRCPOLY;
			else
				r >>= 1;
		}
		crctable[i] = r;
	}
}

BOOL IsValidHeader(char *filename)
{
	// ARJ 헤더를 비교할 crc값 table을 만든다.
	make_crctable();	

	FILE *fd = fopen( filename, "rb" );

    long arcpos = 0, lastpos = 0;
    int c = 0;
	
    arcpos = ftell(fd);
    fseek(fd, 0L, SEEK_END);
    lastpos = ftell(fd) - 2;
	
    for ( ; arcpos < lastpos; arcpos++)
    {
		fseek(fd, arcpos, SEEK_SET);
        c = fget_byte(fd);
        while (arcpos < lastpos)
        {
            if (c != HEADER_ID_LO)  /* low order first */
                c = fget_byte(fd);
            else if ((c = fget_byte(fd)) == HEADER_ID_HI)
                break;
            arcpos++;
        }
		
        if (arcpos >= lastpos)
            break;

        if ((headersize = fget_word(fd)) <= HEADERSIZE_MAX)
        {
            crc = CRC_MASK;
            fread_crc(header, (int) headersize, fd);
            if ((crc ^ CRC_MASK) == fget_crc(fd))
            {
				fseek(fd, arcpos, SEEK_SET);
				fclose(fd);
				return 1;
            }
        }
    }
	fclose(fd);
    return 0;          /* could not find a valid header */
}

static long find_header(FILE *fd)
{
    long arcpos, lastpos;
    int c;
	
    arcpos = ftell(fd);
    fseek(fd, 0L, SEEK_END);
    lastpos = ftell(fd) - 2;
	
    for ( ; arcpos < lastpos; arcpos++)
    {
        fseek(fd, arcpos, SEEK_SET);
        c = fget_byte(fd);
        while (arcpos < lastpos)
        {
            if (c != HEADER_ID_LO)  //* low order first 
                c = fget_byte(fd);
            else if ((c = fget_byte(fd)) == HEADER_ID_HI)
                break;
            arcpos++;
        }
        if (arcpos >= lastpos)
            break;
        if ((headersize = fget_word(fd)) <= HEADERSIZE_MAX)
        {
            crc = CRC_MASK;
            fread_crc(header, (int) headersize, fd);
            if ((crc ^ CRC_MASK) == fget_crc(fd))
            {
                fseek(fd, arcpos, SEEK_SET);
                return arcpos;
            }
        }
    }
    return -1;         //* could not find a valid header 
}

static void decode_path(char *name)
{
	BOOL     path = FALSE;
	char     *start, *where;

	start = name;
	for ( ; *name; name++)
	{
		if (*name == ARJ_PATH_CHAR)
		{
			*name = PATH_CHAR;
			path = TRUE;
		}
	}

	if( path )
	{
		char dir[_MAX_PATH];
		char subDir[_MAX_PATH];
		memset(dir, 0, _MAX_PATH);
		memset(subDir, 0, _MAX_PATH);
		where = strrchr(start, PATH_CHAR); 
		memcpy(subDir, start, (where-start));
		sprintf(dir, "%s\\%s", gAPP_DIR, subDir);
		_mkdir(dir);
	}
}


static int read_header(int first, FILE *fd, char *name)
{
    ushort extheadersize, header_id;
	
    header_id = fget_word(fd);
    if (header_id != HEADER_ID)
    {
        if (first)
            error(M_NOTARJ, name);
        else
            error(M_BADHEADR, "");
    }
	
    headersize = fget_word(fd);
    if (headersize == 0)
        return 0;               /* end of archive */
    if (headersize > HEADERSIZE_MAX)
        error(M_BADHEADR, "");
	
    crc = CRC_MASK;
    fread_crc(header, (int) headersize, fd);
    header_crc = fget_crc(fd);
    if ((crc ^ CRC_MASK) != header_crc)
        error(M_HEADRCRC, "");
	
    setup_get(header);
    first_hdr_size = get_byte();
    arj_nbr = get_byte();
    arj_x_nbr = get_byte();
    host_os = get_byte();
    arj_flags = get_byte();
    method = get_byte();
    file_type = get_byte();
    (void)get_byte();
    time_stamp = get_longword();
    compsize = get_longword();
    origsize = get_longword();
    file_crc = get_crc();
    entry_pos = get_word();
    file_mode = get_word();
    host_data = get_word();
	
    hdr_filename = (char *)&header[first_hdr_size];
    strncopy(filename, hdr_filename, sizeof(filename));
    if ((arj_flags & PATHSYM_FLAG) != 0)
        decode_path(filename);
	
    hdr_comment = (char *)&header[first_hdr_size + strlen(hdr_filename) + 1];
    strncopy(comment, hdr_comment, sizeof(comment));
	
    /* if extheadersize == 0 then no CRC */
    /* otherwise read extheader data and read 4 bytes for CRC */
	
    while ((extheadersize = fget_word(fd)) != 0)
        fseek(fd, (long) (extheadersize + 4), SEEK_CUR);
	
    return 1;                   /* success */
}

static void skip()
{
    fseek(arcfile, compsize, SEEK_CUR);
}

void fillbuf(int n)                /* Shift bitbuf n bits left, read n bits */
{
    bitbuf = (bitbuf << n) & 0xFFFF;  /* lose the first n bits */
    while (n > bitcount)
    {
        bitbuf |= subbitbuf << (n -= bitcount);
        if (compsize != 0)
        {
            compsize--;
            subbitbuf = (uchar) getc(arcfile);
        }
        else
            subbitbuf = 0;
        bitcount = CHAR_BIT;
    }
    bitbuf |= subbitbuf >> (bitcount -= n);
}

void init_getbits()
{
    bitbuf = 0;
    subbitbuf = 0;
    bitcount = 0;
    fillbuf(2 * CHAR_BIT);
}

static void decode_start()
{
    blocksize = 0;
    init_getbits();
}

ushort getbits(int n)
{
    ushort x;
	
    x = bitbuf >> (2 * CHAR_BIT - n);
    fillbuf(n);
    return x;
}

static void read_pt_len(int nn, int nbit, int i_special)
{
    int i, n;
    short c;
    ushort mask;
	
    n = getbits(nbit);
    if (n == 0)
    {
        c = getbits(nbit);
        for (i = 0; i < nn; i++)
            pt_len[i] = 0;
        for (i = 0; i < 256; i++)
            pt_table[i] = c;
    }
    else
    {
        i = 0;
        while (i < n)
        {
            c = bitbuf >> (13);
            if (c == 7)
            {
                mask = 1 << (12);
                while (mask & bitbuf)
                {
                    mask >>= 1;
                    c++;
                }
            }
            fillbuf((c < 7) ? 3 : (int)(c - 3));
            pt_len[i++] = (uchar)c;
            if (i == i_special)
            {
                c = getbits(2);
                while (--c >= 0)
                    pt_len[i++] = 0;
            }
        }
        while (i < nn)
            pt_len[i++] = 0;
        make_table(nn, pt_len, 8, pt_table);
    }
}

static void read_c_len()
{
    short i, c, n;
    ushort mask;
	
    n = getbits(CBIT);
    if (n == 0)
    {
        c = getbits(CBIT);
        for (i = 0; i < NC; i++)
            c_len[i] = 0;
        for (i = 0; i < CTABLESIZE; i++)
            c_table[i] = c;
    }
    else
    {
        i = 0;
        while (i < n)
        {
            c = pt_table[bitbuf >> (8)];
            if (c >= NT)
            {
                mask = 1 << (7);
                do
                {
                    if (bitbuf & mask)
                        c = right[c];
                    else
                        c = left[c];
                    mask >>= 1;
                } while (c >= NT);
            }
            fillbuf((int)(pt_len[c]));
            if (c <= 2)
            {
                if (c == 0)
                    c = 1;
                else if (c == 1)
                    c = getbits(4) + 3;
                else
                    c = getbits(CBIT) + 20;
                while (--c >= 0)
                    c_len[i++] = 0;
            }
            else
                c_len[i++] = (uchar)(c - 2);
        }
        while (i < NC)
            c_len[i++] = 0;
        make_table(NC, c_len, 12, c_table);
    }
}


static ushort decode_c()
{
    ushort j, mask;
	
    if (blocksize == 0)
    {
        blocksize = getbits(16);
        read_pt_len(NT, TBIT, 3);
        read_c_len();
        read_pt_len(NP, PBIT, -1);
    }
    blocksize--;
    j = c_table[bitbuf >> 4];
    if (j >= NC)
    {
        mask = 1 << (3);
        do
        {
            if (bitbuf & mask)
                j = right[j];
            else
                j = left[j];
            mask >>= 1;
        } while (j >= NC);
    }
    fillbuf((int)(c_len[j]));
    return j;
}

void fwrite_txt_crc(uchar *p, int n)
{
    uchar c;
	
    crc_buf((char *)p, n);
    if(no_output)
        return;
	
    if (file_type == TEXT_TYPE)
    {
        while (n--)
        {
            c = *p++;
            if (putc((int) c, outfile) == EOF)
                error(M_CANTWRIT, "");
        }
    }
    else
    {
        if (fwrite((char *)p, 1, n, outfile) != (size_t)n)
            error(M_CANTWRIT, "");
    }
}

static ushort decode_p()
{
    ushort j, mask;
	
    j = pt_table[bitbuf >> (8)];
    if (j >= NP)
    {
        mask = 1 << (7);
        do
        {
            if (bitbuf & mask)
                j = right[j];
            else
                j = left[j];
            mask >>= 1;
        } while (j >= NP);
    }
    fillbuf((int)(pt_len[j]));
    if (j != 0)
    {
        j--;
        j = (1 << j) + getbits((int)j);
    }
    return j;
}


static void oneDecode()
{
	short i;
	short j;
	short c;
	short r;
	long count;
	
	text = new UCHAR[DDICSIZ];
	
	//disp_clock();
	decode_start();
	count = 0;
	r = 0;
	
	while (count < origsize)
	{
		if ((c = decode_c()) <= UCHAR_MAX)
		{
			text[r] = (uchar) c;
			count++;
			if (++r >= DDICSIZ)
			{
				r = 0;
				//disp_clock();
				fwrite_txt_crc(text, DDICSIZ);
			}
		}
		else
		{
			j = c - (UCHAR_MAX + 1 - THRESHOLD);
			count += j;
			i = decode_p();
			if ((i = r - i - 1) < 0)
				i += DDICSIZ;
			if (r > i && r < DDICSIZ - MAXMATCH - 1)
			{
				while (--j >= 0)
					text[r++] = text[i++];
			}
			else
			{
				while (--j >= 0)
				{
					text[r] = text[i];
					if (++r >= DDICSIZ)
					{
						r = 0;
						//disp_clock();
						fwrite_txt_crc(text, DDICSIZ);
						
					}
					if (++i >= DDICSIZ)
						i = 0;
				}
			}
		}
	}
	if (r != 0)
	{
		fwrite_txt_crc(text, r);
		
	}
	
	delete text;
}

static short decode_len()
{
	short c;
	short width;
	short plus;
	short pwr;
	
	plus = 0;
	pwr = 1 << (STRTL);
	for (width = (STRTL); width < (STOPL) ; width++)
	{
		GETBIT(c);
		if (c == 0)
			break;
		plus += pwr;
		pwr <<= 1;
	}
	if (width != 0)
		GETBITS(c, width);
	c += plus;
	return c;
}

static short decode_ptr()
{
	short c;
	short width;
	short plus;
	short pwr;
	
	plus = 0;
	pwr = 1 << (STRTP);
	for (width = (STRTP); width < (STOPP) ; width++)
	{
		GETBIT(c);
		if (c == 0)
			break;
		plus += pwr;
		pwr <<= 1;
	}
	if (width != 0)
		GETBITS(c, width);
	c += plus;
	return c;
}


static void oneDecode_f()
{
    short i;
    short j;
    short c;
    short r;
    short pos;
    long count;
	
    text = new uchar[DDICSIZ];
	
    //disp_clock();
    init_getbits();
    getlen = getbuf = 0;
    count = 0;
    r = 0;
	
    while (count < origsize)
    {
        c = decode_len();
        if (c == 0)
        {
            GETBITS(c, CHAR_BIT);
            text[r] = (uchar)c;
            count++;
            if (++r >= DDICSIZ)
            {
                r = 0;
                //disp_clock();
                fwrite_txt_crc(text, DDICSIZ);
				
            }
        }
        else
        {
            j = c - 1 + THRESHOLD;
            count += j;
            pos = decode_ptr();
            if ((i = r - pos - 1) < 0)
                i += DDICSIZ;
            while (j-- > 0)
            {
                text[r] = text[i];
                if (++r >= DDICSIZ)
                {
                    r = 0;
                    //disp_clock();
                    fwrite_txt_crc(text, DDICSIZ);
					
                }
                if (++i >= DDICSIZ)
                    i = 0;
            }
        }
    }
    if (r != 0)
    {
		fwrite_txt_crc(text, r);
		
    }
	
	
	delete text;
}


static int check_flags()
{
    if (arj_x_nbr > ARJ_X_VERSION)
    {
        skip();
        return -1;
    }
    if ((arj_flags & GARBLE_FLAG) != 0)
    {
        skip();
        return -1;
    }
    if (method < 0 || method > MAXMETHOD || (method == 4 && arj_nbr == 1))
    {
        skip();
        return -1;
    }
    if (file_type != BINARY_TYPE && file_type != TEXT_TYPE)
    {
        skip();
        return -1;
    }
    return 0;
}

static void unstore()
{
	int n;
	long pos;
	char *buffer;
	
	buffer = new char[BUFFERSIZE];
	pos = ftell(arcfile);
	//disp_clock();
	n = (int)(BUFFERSIZE - (pos % BUFFERSIZE));
	n = compsize > (long)n ? n : (int)compsize;
	
	int size = 0;
	
	while (compsize > 0)
	{
		if(fread(buffer, 1, n, arcfile) != (size_t)n)
			error(M_CANTREAD, "");
		//disp_clock();
		compsize -= n;
		size += n;
		fwrite_txt_crc((uchar *)buffer, n);
		
		n = compsize > BUFFERSIZE ? BUFFERSIZE : (int)compsize;
	}
	delete buffer;
}

static int extract()
{
    char name[_MAX_PATH];
	
    if(check_flags())
        return 0;
	
    no_output = 0;
    sprintf(name, "%s\\", gAPP_DIR);
    strcat(name, filename);
	
    outfile = fopen(name, writemode[file_type & 1]);
    if(outfile == NULL)
    {
        skip();
        return 0;
    }
	
    crc = CRC_MASK;
	
	
    if (method == 0)
        unstore();
    else if (method == 1 || method == 2 || method == 3)
        oneDecode();
    else if (method == 4)
        oneDecode_f();
	
    fclose(outfile);
	
    //set_ftime_mode(name, time_stamp, file_mode, (UINT) host_os);
	
    if((crc ^ CRC_MASK) == file_crc)
    {
        //printf(M_CRCOK);
    }
    else
    {
        //printf(M_CRCERROR);
        //error_count++;
    }
    return 1;
}

static int  execute_cmd()
{
    int file_count = 0;
	
    first_hdr_pos = 0;
    time_stamp = 0;
    first_hdr_size = FIRST_HDR_SIZE;
	
    arcfile = fopen(arc_name, "rb");
	
    first_hdr_pos = find_header(arcfile);
    if (first_hdr_pos < 0)
        error(M_NOTARJ, arc_name);
    fseek(arcfile, first_hdr_pos, SEEK_SET);
    if (!read_header(1, arcfile, arc_name))
        error(M_BADCOMNT, "");
	
    file_count = 0;
    while(read_header(0, arcfile, arc_name))
    {
		switch(command)
		{
		case 'X':
			if(extract())
			{
				disp_clock();
				file_count++;
			}
			break;
		case 'L':
			disp_clock();
			file_count++;
			skip();
			break;
		}
    }
	
    fclose(arcfile);
	
    return file_count;
}

/**
 * 어플리케이션이 위치한 디렉토리를 얻는다.
 */
static void GetApplicationDirectory()
{
	char	szModuleFileName[MAX_PATH];
	GetModuleFileName( NULL, szModuleFileName, MAX_PATH );

	char	szDrive[MAX_PATH];
	char	szDir[MAX_PATH];
	char	szName[MAX_PATH];
	char	szExt[MAX_PATH];
	_splitpath( szModuleFileName, szDrive, szDir, szName, szExt );

	sprintf( gAPP_DIR, "%s%s", szDrive, szDir );
} //GetApplicationDirectory

/**
 *
 */
int UpdateDecode( const char* szFileName, int com )
{
	GetApplicationDirectory();

	command = com;

	strcpy( arc_name, szFileName );
//	make_crctable();

	arcfile = NULL;
	outfile = NULL;

	return execute_cmd();
} //UpdateDecode
