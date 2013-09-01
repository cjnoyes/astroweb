/* structures */ 

typedef struct HEM {
           COUNT east;
           COUNT west;
           COUNT south;
           COUNT north;
           } HEM;    

typedef struct GEN {
           COUNT feminine;
           COUNT masculine;
           } GEN;

typedef struct ELE {
           COUNT fire;
           COUNT water;
           COUNT earth;
           COUNT air;
           } ELE;

typedef struct QUAL {
           COUNT cardinal;
           COUNT fixed;
           COUNT _mutable;
           } QUAL;

typedef struct MDL {
           COUNT angular;
           COUNT succedent;
           COUNT cadent;
           } MDL;

typedef struct FUNL {
           COUNT individual;
           COUNT temporal;
           COUNT relative;
           COUNT terminal;
           } FUNL;   

extern HEM   hemispheres, comp_hemispheres;
extern GEN   genders, comp_genders;
extern ELE   elements, comp_elements;
extern QUAL  qualities, comp_qualities;
extern MDL   modals, comp_modals;
extern FUNL  functionals, comp_functionals;
extern COUNT  aspect_count[], comp_aspect_count[];
extern NUM    scout_planet, comp_scout_planet;
extern COUNT  sign_count[], comp_sign_count[];
extern COUNT  house_count[], comp_house_count[];
