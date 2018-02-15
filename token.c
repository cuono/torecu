#include "token.h"
#include "cutenizer.h" /* CLEANUP */
#include "miscellaneous.h" //errxit
#include <stdlib.h>

/* check ttn of token going left-to-right of token 
   return of 1 on comparison is true
   return of 0 on false 
   NOTE; ttnchk in token.h */
int check_types(Stoken *token, int64_t *ttnlst, int len_ttnlst)
{
  if(token == NULL) errxit("TOKEN IS NULL!!! WTF!!! CHECK_TYPES");
  
  int counter = 0;
  while(counter < len_ttnlst)
    {
      //if(ttnlst[counter ++] != token->type.NUMBER)
      //return 0;
      //if(!(ttnlst[counter ++] & token->type.NUMBER))
      //return 0;
      if(!(ttnlst[counter ++] & ttnof(token)))
	{
	  //printf("return nil on; %d and %d\n",ttnlst[counter-1],
	  //ttn_of_vaoca(token));exit(1);
	  return 0;
	}
      token = token -> nxt;
    }
  return 1;
}

/*
   token is affix in ttn
       l is sequence (ll) of token as argv (unlink_argl())
  return is void but token convertedto callable/calan
*/
void affix2calan_vc(Stoken *token/*affix*/, Stoken *l)
{
  Zcalan calan = {scalan};
  calan.NUMBER = scalan;      //TWICE ?

  if(tta.type == INFIX) // INFIX imply 2 or 4 or 6 or 8 or etc
    calan.argc = tt.affix.arity * 2;
  else
    calan.argc = tt.affix.arity;
  
  calan.argv = malloc(sizeof(Stoken *) * calan.argc);

  int count = 0; //
  do{
    calan.argv[count++] = l; //argv[0]->nxt is already argv[1]->nxt
    l = l->nxt;
  }while(l);

  tt.calan = calan;
}

/* token is affix as PREFIX or SUFFIX or INFIX
   return is sequence (ll) of token as argv */
Stoken *unlink_argl(Stoken *token /* AFFIX */)
{
  Stoken *l, *r;
  int count;
  count = 1;
  
  switch(tt.affix.type)
    { /* PREFIX */
    case PREFIX:{
      l = r = token->nxt; /* 1st argument */
      while(count++ < tt.affix.arity)
	r = r->nxt;
      break;
    } /*  INFIX */
    case  INFIX:{
      l = token->prv;
      r = token->nxt;

      // go to L-most
      count = 1;
      while(count++ < tt.affix.arity)
	l = l->prv;
      
      // removal of token at between
      unlink(token);
      
      count = 1;
      // go to R-most
      while(count++ < tt.affix.arity)
	r = r->nxt;

      // put of token on ll of original
      token -> prv = l -> prv;
      l -> prv -> nxt = token;
      token -> nxt = r -> nxt;
      r -> nxt -> prv = token;

      // unlink of l and r insteadof unlink_lr() as for PREFIX and SUFFIX
      l -> prv = NULL;
      r -> nxt = NULL;
      
      return l;
      
      break;
    } /* SUFFIX */
    case SUFFIX:{
      l = r = token->prv;
      while(count++ < tt.affix.arity)
	l = l->prv;
      break;
    }
    default:
      errxit("unknown type of affix");
      break;
    }
  unlink_lr(l,r);

  return l;
}
