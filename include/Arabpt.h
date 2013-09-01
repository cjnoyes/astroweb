
#pragma pack(1)
typedef struct ARAB_PART {
       char base;
       char plus;
       char minus;
       } ARAB_PART;
#pragma pack()

#define N_ARAB_PART 25

extern AS_INF *arabic_parts;
extern int arabic_count;

extern AS_INF *getArabicParts(void);
extern int getArabicCount(void);
