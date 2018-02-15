#ifndef RETOKENIZER_H
#define RETOKENIZER_H
#include "token.h"

/* run upon ambiguity
   lorr defined in main.c */
#define LorR int lorr(Stoken *l, Stoken *r)

/* RE interface 
   a RE is C-function named retokenizer
   a retokenizer operate on token and/or token->type
   shortcut to token is err... token
   shortcut to token->type is tt
   eg tt.number = ttn_affix (ttn is token type number)
   a ttn is number for a tt
   convention is name ttn_xxx for tt.xxx
   ttn_yyy for tt.yyy
   xxx may tt.xxx.aaa
   yyy may tt.yyy.bbb
   importance is tt.number is ttn_xxx or ttn_yyy
   if ttn is affix and affix's arity > 0 then rea2ca
   for 0 arity variadic arity is unimplemented
   but a macro like recall may be possible
   eg ( and ) delimited arity */
#define RE(foo) Stoken *RE##foo (Stoken *token)
/* INFIX can PREFIX if ttn of token->prv
   is not TTN */
#define reicanp(TTN)\
  { if(!(ttnof(token->prv) & (TTN))) tta.type = PREFIX; }
#define reaffix2calan\
  { un_slash_ambiguity(token);\
    affix2calan_vc(token,unlink_argl(token)); }
#define recalan(RETU,CUTENIZER)\
  { ttc.retu = RETU;\
    ttc.cutenizer = CUTENIZER; }

/* C-function declaration */
Stoken *unambiguity(Stoken *root);
Stoken *un_slash_ambiguity(Stoken *root);
Stoken *randlrun(Stoken *token);

#endif
