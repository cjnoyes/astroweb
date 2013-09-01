#ifndef _PATHS____HPP_
#define _PATHS____HPP_

#define PROGRAM_F   0
#define UTILITY_F   1
#define DATA_F      2
#define HELP_F      3
#define USER_F      4
#define TEMP_F      5
#define DATABASE_F  6
#define TEXT_F      7
#define EDIT_F      8
#define CURRENT_F   9
#define DEFAULT_FLD 10

#ifndef WINVER
#define DIRSEPCH  '/'
#define DIRSEP    "/"
#else
#define DIRSEPCH  '\\'
#define DIRSEP    "\\"
#endif

extern char *set_path( char *name, int dir );
extern char *replace_ext( char *buf, char *name, char *ext );
extern char *make_name( char *buf, char *name, int dir );
extern void setHomeDir(char *pHome);

#endif /*  _PATHS____HPP_  */

