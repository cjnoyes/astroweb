

#define AND 1
#define OR  2
#define NOT 3
#define FIRST_RESULT 5

#define HOUSE   1
#define ASPECT  2
#define SIGN    3
#define SIGNHSE 4
#define HSECSP  5
#define PLAHSE  6
#define PLASIGN 7

#ifdef _MSC_VER
#pragma pack(1)
#endif
typedef struct ASTR_PROBLEM {
	char planet;
	char arg_type;
	char arg2;
	char arg3;
	unsigned char result;
	} ASTR_PROBLEM;

#ifdef _MSC_VER
#pragma pack()
#endif


