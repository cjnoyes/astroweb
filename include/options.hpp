#ifndef _OPTIONS__HPP_
#define _OPTIONS__HPP_
/*
class OptionsDlgf : public FormDlgf {
int side;
unsigned int *pOptions;
BOOL house, aspects, ext_asp, summary,
   misc, extc, ver_east, text,
   bas_mid, half_mid, full_mid, mid_asp;
short *pHouse_proc;
zAstComboBoxStatic *pCbhpr;
public:
      OptionsDlgf(zWindow *pPar, char *title, short *hsprc, unsigned int *pOpt );
      ~OptionsDlgf() {}
      selchg( zEvent *e );
      void buildFlags();
      };


class CustomDlgf : public FormDlgf {
zAstComboBoxStatic *pCbdf;
Options *pOptions;
int _datafile;
char *_title;
int reset_value;
BOOL reloc, prog, prhs, graph,
  text, data;
public:
      CustomDlgf(zWindow *pPar, char *title, Options *op, int cst );
      ~CustomDlgf() {}
      selchg( zEvent *e );
      getDataFile() { return _datafile; }
      virtual int command( zCommandEvt * );
      void buildFlags();
      };

*/

// Useful Macros

#define is_set(x,n)   (((x)&(n))?1:0)
#define set_it(v,n,x) v|=((x)?(n):0)


#endif /* _OPTIONS__HPP_  */
