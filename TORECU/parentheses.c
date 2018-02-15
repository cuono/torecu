#include "parentheses.h"


RE(parenR)
{
  Stoken *R; int count;
  R = token; count = -1;
  do{ // find L
    token = token -> prv;
    ++count;
  }while(!(tt.NUMBER & srandl));
  /* remove R and L 
     and create list or 
     remain the 1st
     token between R and L */
  if(count == 1)
    {
      unlink(token); unlink(R);
      free(token); free(R);
    }
  else
    {
      errxit("count != 1 is unimplemented");
    }
}
TO(parenR)
{
  totoken(srandl);
  torandl(randl_parenthesisR,
	  REparenR);
}
TO(parenL)
{
  totoken(srandl);
  torandl(randl_parenthesisL,
	  REparenR);
}
