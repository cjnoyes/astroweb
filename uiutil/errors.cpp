#include "stdafx.h"
#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include "errors.fns"
#include "charcons.h"
#include <stdio.h>
#include "files.h"
#include <string.h>

static char *_ERROR2 = "ERROR!!";

/* routine to handle out of memory errors */

void do_mem_error( int fatal )
{
 fprintf(getErrorFile(),"Out of Memory\n");
 if (fatal) {
	 exit(99);
   }
}
#define NON_FATAL 0

static char buf[500];
static char a_title[] = "Error";
static char stdmsg[] = { "%s : System Error #%d--%s" };

//#define stdmsg RESSTR_E_STD /*"%s : System Error #%d--%s"*/

#ifdef _MSC_VER
char *errors[] = { stdmsg, NULL, "File or Directory Does Not Exist!",
   NULL, NULL, NULL,  NULL, stdmsg,
   "File is Not an executable File!", stdmsg, NULL, NULL,
   "Not Enough Memory Exists!", "Access To File or Device Denied!",
   NULL, NULL, NULL, "File Exists!", "Trying to Rename File to Another"
   "Directory or Drive!", NULL, NULL, NULL, stdmsg, NULL,
   "Not Enough Files in CONFIG.SYS need at least 12", NULL, NULL, NULL,
   "No Space Left on Drive", NULL, NULL, NULL, NULL, stdmsg, stdmsg,
   NULL, stdmsg, NULL, "Error or Printer Not Ready!" };
#else
char *errors[] = { "No Error!", stdmsg, "File or Directory Does Not Exist!",
   "File or Directory Does Not Exist!", "Too Many Open Files!",
   "Access To File or Device Denied!", stdmsg, "Memory Blocks Destroyed!",
   "Not Enough Memory Exists!", stdmsg, stdmsg, stdmsg, stdmsg, stdmsg,
   stdmsg, "No Such Device!", stdmsg, stdmsg, "Not Enough Files in CONFIG.SYS!",
   stdmsg, stdmsg, stdmsg, stdmsg, stdmsg, stdmsg, NULL, NULL, NULL, NULL,
   NULL, NULL, NULL, NULL, "Bad Math Argument!", "File Already Exists!",
   "File Locking Error!", NULL, NULL, NULL, NULL };
#endif

/*
int errors[] = {stdmsg, 0, RESSTR_E_3, 0, 0, 0, 0, stdmsg, RESSTR_E_4,
 stdmsg, 0, 0, RESSTR_E_5, RESSTR_E_6, 0, 0, 0, RESSTR_E_7, RESSTR_E_8,
 0, 0, 0, stdmsg, 0, RESSTR_E_9, 0, 0, 0, RESSTR_E_10, 0, 0, 0, 0, stdmsg,
 stdmsg, 0, stdmsg, 0, RESSTR_E_11 };

int alert_box( int id )
{
return( alert_box( "", CResStr(id) );
}


int alert_box( char *title, int id )
{
return( alert_box( title, CResStr(id) );
}
*/

int alert_box( char *title, char *fmt, ... )
{
 int result;
 char *a_title = /*CResStr(RESSTR_E_MSG)*/ "Error";
 va_list marker;

 if ( title == NULL || !strcmp(title, "")  )
    title = a_title;
 va_start( marker, fmt );
 vsprintf( buf, fmt, marker );
 va_end( marker );
 assert( strlen( buf ) < sizeof( buf ) );
 fprintf(getErrorFile(),"%s--%s\n",title,buf); 
 return( 1 );
}

int alert_box_ext( char *title, char *fmt, ... )
{
 int result;
 char *a_title = /*CResStr(RESSTR_E_MSG)*/ "Error";
 va_list marker;

 if ( title == NULL || !strcmp(title, "") )
    title = a_title;
 va_start( marker, fmt );
 vsprintf( buf, fmt, marker );
 va_end( marker );
 assert( strlen( buf ) < sizeof( buf ) );
 fprintf(getErrorFile(),"%s--%s\n",title,buf);
 return( 1 );
}

/* function to handle file and other errors reported in errno */

void do_error( const char *file )
{
 static char spmsg[] = { "Call Technical Support With This Error!" };

 if ( errno == ENOMEM ) {
    do_mem_error( FALSE );
    return;
    }
 else if ( errno == 0 )
    return;
 if ( errno > 40 )
    alert_box_ext( ERROR2, /*CResStr(RESSTR_E_ILL)*/ "ILLEGAL ERROR #%d--%s", errno, /*CResStr(RESSTR_E_CAL)*/spmsg );
 else if ( errors[errno] == NULL )
    alert_box_ext( ERROR2, /*CResStr(RESSTR_E_UND)*/ "UNDEFINED ERROR #%d--%s", errno, /*CResStr(RESSTR_E_CAL)*/spmsg );
 else if ( errors[errno] == stdmsg )
    alert_box_ext( ERROR2, /*CResStr(RESSTR_E_STD)*/ stdmsg, file, errno, /*CResStr(RESSTR_E_CAL)*/ spmsg );
 else
    alert_box_ext( ERROR2, "%s : %s ", /*CResStr(*/errors[errno]/*)*/, file );
 errno = 0;
}
