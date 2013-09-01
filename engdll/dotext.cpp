#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <share.h>
#include "textptr.h"
#include <assert.h>
#include "errors.fns"
#include "output.fns"
#include <string.h>
#include "errors.fns"
#include <assert.h>
#include "charcons.h"
#include "paths.hpp"
#error "Make sure DOTEXT should be included"

#define NO_TXTFIL 15
#define SECTOR_SIZE 256
#define TWO_SECTORS 512
/* enough size for trailing '\n' plus '\0' + */
#define BUF_SIZE TWO_SECTORS+3

extern char *data_name( char * );

TXTFIL * file_ptr = NULL;
int no_files = 0;       /* number of positions used */
int used = 0;
char *txtbuf = NULL;	/* text buffer */

/* Kill memory associated to ems files */

void __export kill_txptps( void )
{
 int i;
 TXTFIL *pt;

 if ( file_ptr == NULL )
    return;
 for ( i = 0, pt = file_ptr; i < used; ++i, ++pt ) {
     if ( pt->txptp.is_open == MEMORY ) {
	GlobalUnlock(pt->txptp.handle);
	GlobalFree(pt->txptp.handle);
	pt->txptp.handle = 0;
	pt->txptp.is_open = CLOSED;
	}
     if ( pt->textp.is_open == MEMORY ) {
	GlobalUnlock(pt->textp.handle);
	GlobalFree(pt->textp.handle);
	pt->textp.handle = 0;
	pt->textp.is_open = CLOSED;
	}
     if ( pt->txptp.is_open == DISCARDABLE ) {
	GlobalFree(pt->txptp.handle);
	pt->txptp.handle = 0;
	pt->txptp.is_open = CLOSED;
	}
     if ( pt->textp.is_open == DISCARDABLE ) {
	GlobalFree(pt->textp.handle);
	pt->textp.handle = 0;
	pt->textp.is_open = CLOSED;
	}
     }
 delete file_ptr;
 file_ptr = NULL;
}

/* close all the files associated with a specific text file.
   Marks the file closed. Also if there are no open files, it frees up
   the memory associated with them. Returns 0 for success, 1 for error */

int close_files( int file_num )
{
 int er = 0;
 TXTFIL *pt;

 if ( file_num < 0  )
    return( 1 );
 assert( file_num < NO_TXTFIL && file_num >= 0 );
 pt = file_ptr + file_num;
 if ( pt->txptp.is_open == OPEN ) {
    if ( pt->txptp.fil != -1) {
       close( pt->txptp.fil );
       pt->txptp.fil = -1;
       }
    pt->txptp.is_open = CLOSED;
    }
 else if ( pt->txptp.is_open == MEMORY ) {
    if ( (GlobalFlags(pt->txptp.handle) & GMEM_LOCKCOUNT) ) {
       GlobalUnlock(pt->txptp.handle);
       GlobalLRUNewest(pt->txptp.handle);
       GlobalReAlloc(pt->txptp.handle,0L,GMEM_MODIFY|GMEM_DISCARDABLE);
       }
    pt->txptp.is_open = DISCARDABLE;
    }
 if ( pt->textp.is_open == OPEN ) {
    if ( pt->textp.fil != -1 ) {
       close( pt->textp.fil );
       pt->textp.fil = -1;
       }
    pt->textp.is_open = CLOSED;
    }
 else if ( pt->textp.is_open == MEMORY ) {
    if ( (GlobalFlags(pt->textp.handle) & GMEM_LOCKCOUNT) ) {
       GlobalUnlock(pt->textp.handle);
       GlobalLRUNewest(pt->textp.handle);
       GlobalReAlloc(pt->textp.handle,0L,GMEM_MODIFY|GMEM_DISCARDABLE);
       }
    pt->textp.is_open = DISCARDABLE;
    }
 return( 0 );
}


/* function to allocate space for and read a file into memory for use
   as either index or as text. Always reads in even sectors if at all
   possible. ofs is amount we are into file now. Returns NULL if an
   error exists */

void *x_read_fil( int ofs, int file_num, TFIL *p )
{
 char huge *buf, *bp;
 unsigned rd, siz, bl, rem;
 int red;
 long fs;

 if ( ( fs = filelength( p->fil ) ) == -1L ) {
    do_error( p->name );
    close_files( file_num );
    return( NULL );
    }
 fs -= ofs;
 if ( fs % SECTOR_SIZE ) {
    bl = fs / SECTOR_SIZE;
    rd = (unsigned int) fs;
    rem = fs % SECTOR_SIZE;
    fs = (bl+1) * SECTOR_SIZE;
    }
 if ( fs < 65530 ) 
    siz = fs;
 else
    siz = 8192;
 if ( p->handle == 0 ) {
    if ( ( p->handle = GlobalAlloc(GMEM_MOVEABLE,fs) ) == NULL )
       return( NULL );
    }
 else {
    if ( ( p->handle = GlobalReAlloc(p->handle,fs,
	  GMEM_MODIFY|GMEM_NODISCARD|GMEM_MOVEABLE) ) == NULL )
       return( NULL );
    }
 if ( ( buf = GlobalLock( p->handle ) ) == NULL )
    return( NULL );
 p->is_open = MEMORY;
 if ( fs < 65530 ) {
    red = read( p->fil, buf, siz );
    if ( red == -1 || ( (unsigned int) red < rd ) ) {
       do_error( p->name );
       close_files( file_num );
       return( NULL );
       }
    }
 else {
    bp = buf;
    do {
       if ( fs < 8192 )
	  siz = fs;
       red = read( p->fil, buf, siz );
       if ( red == -1 || ( (unsigned int) red < siz ) ) {
	  do_error( p->name );
	  close_files( file_num );
	  return( NULL );
	  }
       bp += 8192;
       fs -= 8192;
       } while ( fs );
    }
 close( p->fil );
 p->fil = -1;
 return( buf );
}


void *x_undiscard_fil( int ofs, int file_num, TFIL *p )
{
 void huge *ptr;

 if ( GlobalFlags(p->handle) & GMEM_DISCARDED )
    return( x_read_fil( ofs, file_num, p ) );
 else if ( !(GlobalFlags(p->handle) & GMEM_LOCKCOUNT) ) {
    GlobalReAlloc(p->handle,0L,GMEM_MODIFY|GMEM_NODISCARD|GMEM_MOVEABLE);
    ptr =(void huge *) GlobalLock(p->handle);
    }
 return( ptr );
}

/* setup and open files associated with base_name. Opens a text file and
   and index file. If space is not allocated for files via file_ptr then
   that space is allocated. Also reads number of cols and rows and marks
   the record as open signifying used. Returns -1 if an error occurs.
   Otherwise this returns an integer handle to the files involved. This
   is not a true handle but an index in an array of the structure holding
   the handles and other data. If enough memory exists to read the txtpt
   file then that is attempted. If that is not possible pt->ptr contains
   NULL. If enough memory exist for the entire text file to be read directly
   into memory then that is done. If space cannot be allocated for the
   text file in memory and the txt_ptr is NULL then space is allocated for
   the text buffer. Closes all files in that block, marks pt->is_open as closed
   and returns -1, if an error exists */

int __export x_open_files( char *base_name, int num )
{
 int file_num = 0, h1, h2, flag;
 char *fi, name[15];
 TXTFIL *pt;

 if ( file_ptr == NULL ) {
    file_ptr = new TXTFIL[NO_TXTFIL];
    if ( file_ptr == NULL ) {
       do_mem_error( 0 );
       return( -1 );
       }
    no_files = 0;
    used = 0;
    memset( file_ptr, 0, NO_TXTFIL * sizeof( TXTFIL ) );
    pt = file_ptr;
    }
 else {
    assert( no_files < NO_TXTFIL && used < NO_TXTFIL );
    flag = 0;
    for ( file_num = 0, pt = file_ptr; file_num < used; ++file_num, ++pt ) {
	if ( !strcmpi(base_name, pt->base) ) {
	   flag = 1;
	   break;
	   }
	}
    if ( !flag ) {
       for ( file_num = 0, pt = file_ptr; file_num < NO_TXTFIL; ++file_num, ++pt ) {
	   if ( pt->txptp.is_open == CLOSED && pt->textp.is_open == CLOSED )
	      break;
	   }
       }
    }
 assert( file_num < NO_TXTFIL );
 if ( pt->txptp.is_open == CLOSED && pt->textp.is_open == CLOSED ) {
    strcpy( pt->base, base_name );
    ++no_files;
    }
 if ( file_num == used )
    used++;
 if ( pt->txptp.is_open != MEMORY ) {
    strcpy( name, base_name );
    strcat( name, ".NDX" );
    fi = data_name( name );
    assert( fi != NULL && strlen( fi ) < 150 );
    strcpy( pt->txptp.name, fi );
    if ( ( h1 = sopen( fi, O_BINARY|O_RDONLY, SH_DENYNO ) ) == -1 ) {
       do_error( fi );
       return( -1 );
       }
    if ( read( h1, (char *)&pt->rows, 2 ) != 2 ) {
       do_error( fi );
       close( h1 );
       return( -1 );
       }
    if ( read( h1, (char *)&pt->cols, 2 ) != 2 ) {
       do_error( fi );
       close( h1 );
       return( -1 );
       }
    if ( num == 3 ) {
       if ( read( h1, (char *)&pt->deep, 2 ) != 2 ) {
	  do_error( fi );
	  close( h1 );
	  return( -1 );
	  }
       }
    else
       pt->deep = 0;
    }
 pt->mode = num;
 if ( pt->textp.is_open != MEMORY ) {
    strcpy( name, base_name );
    strcat( name, ".TXT" );
    fi = data_name( name );
    assert( fi != NULL && strlen( fi ) < 67 );
    strcpy( pt->textp.name, fi );
    if ( ( h2 = sopen( fi, O_BINARY|O_RDONLY, SH_DENYNO ) ) == -1 ) {
       close( h1 );
       do_error( fi );
       return( -1 );
       }
    }
 if ( pt->textp.is_open == MEMORY && pt->txptp.is_open == MEMORY )
    return( file_num );
 if ( pt->textp.is_open == DISCARDABLE ) {
    pt->textp.is_open = OPEN;
    pt->textp.fil = h2;
    pt->text =(char *) x_undiscard_fil( 0, file_num, &pt->textp );
    }
 else {
    pt->textp.is_open = OPEN;
    pt->textp.fil = h2;
    pt->text =(char *) x_read_fil( 0, file_num, &pt->textp );
    if ( pt->textp.is_open == CLOSED )
       return( -1 );
    }
 if ( pt->txptp.is_open == DISCARDABLE ) {
    pt->txptp.is_open = OPEN;
    pt->txptp.fil = h1;
    pt->ptr =(TXTPT *) x_undiscard_fil( 0, file_num, &pt->txptp );
    }
 else {
    pt->txptp.is_open = OPEN;
    pt->txptp.fil = h1;
    pt->ptr =(TXTPT *) x_read_fil( sizeof(int) * num, file_num, &pt->txptp );
    if ( pt->txptp.is_open == CLOSED )
       return( -1 );
    }
 if ( txtbuf == NULL && pt->text == NULL ) {
    txtbuf = new char[BUF_SIZE];
    if ( txtbuf == NULL ) {
       do_mem_error( 0 );
       close_files( file_num );
       return( -1 );
       }
    }
 return( file_num );
}

/* function that gets a text ptr. Determines the offset of the txtpt
   in the file or buffer, depending on how many dimensions are being
   used. If the txtpt is in memory then just a pointer to that txtpt
   is returned otherwise the txtpt is read into the space provided
   in the structure. If an error exists this function returns NULL.
   col, row and deep are self explanatory. Though file_num is an
   index into the array of TXFIL, not a file handle */


TXTPT *get_text_ptr( int col, int row, int deep, TXTFIL *pt )
{
 long offset = 0;
 short ofs;

 assert( file_ptr != NULL );
 assert( pt->txptp.is_open == OPEN || pt->txptp.is_open == MEMORY );
 if ( row >= pt->rows )
    return( NULL );
 else if ( col >= pt->cols )
    return( NULL );
 else if ( pt->mode == 3 && deep >= pt->deep )
    return( NULL );
 if ( pt->mode == 3 )
    ofs = (pt->deep * (( col * pt->rows )+row ))+ deep;
 else
    ofs = ( ( col * pt->rows ) + row );
 if ( pt->ptr == NULL && pt->txptp.is_open != MEMORY ) {
    offset = (ofs * sizeof( TXTPT )) + pt->mode * sizeof( int );
    if ( lseek( pt->txptp.fil, offset, SEEK_SET ) == -1L ) {
       do_error( pt->txptp.name );
       return( NULL );
       }
    if ( read( pt->txptp.fil, (char *) &pt->txtpt,
            sizeof( TXTPT ) ) != sizeof(TXTPT) ) {
       do_error( pt->txptp.name );
       return( NULL );
       }
    return( &pt->txtpt );
    }
 else {
    return( pt->ptr+ofs );
    }
}

/* function to get the text at row,col in file_num and read it and
   write it back out to stream. Calls get_text_ptr for the txtptr
   for the text block to transfer. If enough space does not exist
   to read the entire text block into txtbuf at one time then the
   textblock is read in multiple reads each one or two sectors each.
   Returns 0 for ok success 1 for error.  Always reads in at least
   one sector or two at a time for speed, minimum one sector since
   each text block is apt, most of the time to be a little less than
   one sector in size, utilizing direct dma read to the buffer, avoiding
   the intermediate dos buffer where possible, increasing speed.
   if text is found in memory though that whole block is written to
   the stream directly bypassing txtbuf in one fwrite opperation. */


int __export get_transfer_text_asp( int row, int col, int deep,
                                         int file_num, FILE *stream )
{
 char *buf;
 TXTPT *ptr;
 int reads, rem, x, lsiz, nr;
 TXTFIL *pt;

 assert( stream != NULL );
 if ( file_num == -1 )
    return( 1 );
 assert( file_num < NO_TXTFIL );
 pt = file_ptr + file_num;
 if ( ( ptr = get_text_ptr( col, row, deep, pt ) ) == NULL )
    return( 1 );
 if ( ptr->length < 1 )
    return( 1 );
 if ( pt->text == NULL ) {
    if ( lseek( pt->textp.fil, ptr->starts_at, SEEK_SET ) == -1L ) {
       do_error( pt->textp.name );
       return( 1 );
       }
    }
 else {
    if ( ( nr = fwrite( pt->text + ptr->starts_at, sizeof( char ),
          ptr->length, stream ) ) < ptr->length ) {
       return( 1 );
       }
    else
       return( 0 );
    }
 if ( ptr->length > TWO_SECTORS ) {
     reads = ptr->length / TWO_SECTORS;
     lsiz = ptr->length % TWO_SECTORS;
     if ( lsiz > SECTOR_SIZE )
        rem = TWO_SECTORS;
     else
        rem = SECTOR_SIZE;
     }
 else if ( ptr->length > SECTOR_SIZE ) {
     lsiz = ptr->length;
     rem = TWO_SECTORS;
     reads = 0;
     }
 else {
     lsiz = ptr->length;
     rem = SECTOR_SIZE;
     reads = 0;
     }
 for ( x = 1; x <= reads; ++x ) {
     if ( ( nr = read( pt->textp.fil, txtbuf, TWO_SECTORS )) != TWO_SECTORS ) {
	do_error( pt->textp.name );
        return( 1 );
        }
     *(txtbuf+TWO_SECTORS) = '\0';
     if ( fputs( txtbuf, stream ) )
        return( 1 );
     }
 if ( rem ) {
     if ( ( nr = read( pt->textp.fil, txtbuf, rem )) < lsiz ) {
	do_error( pt->textp.name );
        return( 1 );
        }
     *(txtbuf+lsiz)= '\0';
     if ( fputs( txtbuf, stream ) )
        return( 1 );
    }
 return( 0 );
 }
