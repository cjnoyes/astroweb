#define sd2m(s,d,m) (((s)*1800)+((d)*60)+m)

#define N_FIXED_STAR 58

extern AS_INF *fixed_stars;
extern int fixed_star_count;

extern AS_INF *getFixedStars();
extern int getFixedStarCount();
