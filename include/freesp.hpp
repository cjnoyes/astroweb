#ifndef _FREESP___HPP_
#define _FREESP___HPP_


typedef struct FREE_SP {
	long ofs;
	short  len;
	short  used;
	} FREE_SP;

class FreeBlock {
      short nfree;
      FREE_SP frees[20];
public:
      FreeBlock() { nfree = 0; memset( frees, 0, sizeof( FREE_SP ) * 20 ); }
      ~FreeBlock() {}
      short write( char *file );
      short read( char *file );
      FREE_SP *find( short len );
      short getsp( short len, long *ofs);
      short add( short len, long ofs );
      long do_free( short width, short *widthp, short *fail_len, long *fail_rec );
      };

int extern free_cmp(const void *p1,const void *p2 );

#endif /* _FREESP___HPP_ */

