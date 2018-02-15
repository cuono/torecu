#ifndef CUTENIZER_H
#define CUTENIZER_H
#include "token.h"

/* CU interface 
   a CU is C-function named cutenizer */
#define CU(foo) Stoken *CU##foo (Stoken *token)
#define curetun(TTN)\
  static Stoken retun;\
  TT(&retun).NUMBER = TTN;
#define ttnof(TOKEN)\
  (TT(TOKEN).NUMBER & scalan ? TT(TOKEN).calan.retu : TT(TOKEN).NUMBER)

/* C-function declaration */
Stoken *cuono(Stoken *token);

#endif
