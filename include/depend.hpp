#ifndef _DEPEND___HPP_
#define _DEPEND___HPP_

class DependSourceItem {
   char _name[10];
   char _description[60];
   int _type;
   zString _buildCommands;
 public:
   DependSourceItem() {}
   ~DependSourceItem() {}
   zString &buildCommands() { return _buildCommands; }
   char *name() { return _name; }
   char *description() { return _description; }
   int &type() { return _type; }
   };

class DependItem {
   char _name[10];
   char _description[60];
   int _type;
   int _buildWhen;
   zString _beforeCommands;
   zString _afterCommands;
   DependSourceItemSlist sourceList;
   char symbols[10];
 public:
   DependSourceItem() {}
   ~DependSourceItem() {}
   zString &buildCommands() { return _buildCommands; }
   char *name() { return _name; }
   char *description() { return _description; }
   int &type() { return _type; }
   };

#define DT_2DTEXT  0
#define DT_3DTEXT  1
#define DT_REPORT  2
#define DT_HELP    3
#define DT_SYMBOL  4
#define DT_RSYMBOL 5
#define DT_STRTAB  6

#endif /* _DEPEND___HPP_ */

