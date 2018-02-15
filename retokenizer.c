#include "miscellaneous.h"
#include "retokenizer.h"
#include <stdlib.h>
#include <stdio.h>

extern int lorr(Stoken *l, Stoken *r);

// unambiguity cando BASICness of AFFIX
Stoken *unambiguity(Stoken *root/*affix*/)
{
  /* ? */ /* any affix toward l-side is solved 
     SUFFIX is unambiguous dueto left-to-right
     SUFFIX can swallow argl */
  if(TT(root).affix.type == SUFFIX) return root;
  
  Stoken *token; int count;

  token=root->nxt; count = 0;
  while(count < TTA(root).arity)
    {
      //if(tt.NUMBER == saffix)
      if(tt.NUMBER & saffix)
	{
	  tt.affix.retokenizer(token);
	  /* retokenizer may've swallowed
	     eg, PREFIX . . SUFFIX . .
	     say PREFIX got 3 arity
	     . . SUFFIX = 1
	     so PREFIX C . .
	     thus recount so count is C not . 
	     otherworldy recount so count align token in relation to arity
	     token at end of arity is unknown due to nature of algorithm */
	  Stoken *tmp; int recount;
	  tmp=root; recount = 0;
	  do { ++ recount; }while((tmp=tmp->nxt) != token);
	  count = recount - 1; /* NOTE; -1 or not ? 
				  count startas zero ? */
	}
      token=token->nxt; ++ count;
    }
}

/* version of old of un_slash_ambiguity in DEPRECATED.c
   un_slash_ambiguity cando ADVANCEDness of affix */
Stoken *un_slash_ambiguity(Stoken *root/*affix*/)
{
  /* ? */ /* any affix toward l-side is solved 
	     SUFFIX is unambiguous
	     SUFFIX can swallow argl 
	     why is suffix unambigous?
	     the algorithm goes
	     left-to-right
	     if it had gone 
	     right-to-left then
	     PREFIX would be unambigious */
  if(TT(root).affix.type == SUFFIX) return root;
  
  Stoken *token; int count;

  /*
    ADD ADD 1 1 3
  */
  
  // case 2nd is affix
 repeat_unambiguity:
  //puts("unambiguity");
  token=root->nxt; count = 0;
  while(count < TTA(root).arity)
    {
      //if(tt.NUMBER == saffix)
      if(tt.NUMBER & saffix)
	{
	  tt.affix.retokenizer(token);
	  /* retokenizer may've swallowed
	     eg, PREFIX . . SUFFIX . .
	     say PREFIX got 3 arity
	     . . SUFFIX = 1
	     so PREFIX C . .
	     thus recount so count is C not . 
	     otherworldy recount so count align token in relation to arity
	     token at end of arity is unknown due to nature of algorithm */
	  Stoken *tmp; int recount;
	  tmp=root; recount = 0;
	  do { ++ recount; }while((tmp=tmp->nxt) != token);
	  count = recount - 1; /* NOTE; -1 or not ? 
				  count startas zero ? */
	}
      token=token->nxt; ++ count;
    }

  //puts("test for ambiguity");
  // test for ambiguity or return of root
  /* token is assoc */ ; count = 0; /* count as 1 or 0 ? */
  while(1)
    {
      if(token == NULL || (tt.NUMBER & seol)) // end of list ?
	{ /* puts("null or EOL"); */ return root; }

      if(tt.NUMBER & saffix)
	{
	  //NOTE; case of 1 + 2 2 * 3 //if count >= tt.affix then return root
	  //NOTE; case of 1 + 2 2 2 * //* as suffix ?
	  if(tt.affix.type == PREFIX || count /* ? */ >= tt.affix.arity)
	    return root;

	  //NOTE; case of 1 + 2 * 3
	  //           to 1 + (* 2 3)
	  //return of 1 if R-side was solved in lorr
	  //return of 0 on otherwise
	  //NOTE; L-side is solved after break after return of root
	  //NOTE; R-side must solve in lorr
	  /* solve L-side or R-side by lorr */
	  if(lorr(root,token))
	    goto repeat_unambiguity;

	  /* else */
	  //puts("else no ambiguity");
	  break;
	}
      token=token->nxt; ++ count;
    }
  
  return root;
}

/* algorithm for R and L like ( and )
   forward to R
   bakward to L
   go between R and L use tt.retokenizer(token) until R
*/
Stoken *randlrun(Stoken *token)
{
  Stoken *first_token = token;
  while(token){ //go to R
    if((tt.NUMBER & srandl)
       && tt.randl.rorl % 2 == 0){
      int rorl; Stoken *R;
      rorl = tt.randl.rorl; R = token;
      while(token){ // go to match of L to R
	if((tt.NUMBER & srandl)
	   && tt.randl.rorl % 2 != 0){
	  if(rorl + 1 != tt.randl.rorl)
	    errxit("invalid L for R");
	  Stoken *L;
	  L = token;
	  /* retokenization between R and L */
	  do{
	    token = token -> nxt;
	    if(tt.NUMBER & saffix)
	      tta.retokenizer(token);
	  }while(token != R);
	  /* retokenizer for R
	     is supposed to 
	     remove R and L
	     in substitution for
	     a single token 
	     thus,
	     token = token -> nxt
	     to be on a token
	     which won't be removed
	     by retokenizer for R
	     and then,
	     token = token -> prv
	     to switch to the supposedly
	     token substituted for R and L */
	  token = token -> nxt;
	  R->type.randl.retokenizer(R);
	  token = token -> prv;
	  break;
	}
	token=token->prv;
      } /* else // no match of L to R */
      if(token == NULL)
	errxit("no L for R");
    }
    token=token->nxt;
  }
  return first_token; // this ought to be a token like seol at beginning of ll
}
