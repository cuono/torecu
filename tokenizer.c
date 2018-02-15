#include "miscellaneous.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* allocate Stoken and prepend
   to ll tokenizer->token
   NOTE tokenizer.h totoken */
void to_new_token(Stokenizer *tokenizer){
  Stoken *token = malloc(sizeof(Stoken));
  token->nxt = tokenizer->token;
  tokenizer->token = token;
}

/* what does this routine do?
   allocate enough memory
   copy string at tokenizer->p
   null terminate and return (tokenizer->p remain unmodified) */
char *to_get_match(Stokenizer *tokenizer){
  char *s = malloc(tokenizer->length + 1);
  strncpy(s,tokenizer->p,tokenizer->length);
  s[tokenizer->length] = '\0';
  return s;
}

void sort(Stok_dclr *tok_dclr,int len_tok_dclr)
{
  int j, k;
  for(j = 0; j < len_tok_dclr; ++j)
    for(k = 0; k < len_tok_dclr; ++k)
    {
      //#define rtd(N) tok_dclr[N].tok_dclr
#define rtd(N) tok_dclr[N]
      int J = rtd(j).length != 0 ? rtd(j).length : (rtd(j).length = strlen(rtd(j).regex));
      int K = rtd(k).length != 0 ? rtd(k).length : (rtd(k).length = strlen(rtd(k).regex));
#undef  rtd
      if(J > K)
      {
        Stok_dclr tdj = tok_dclr[j];
        tok_dclr[j] = tok_dclr[k];
        tok_dclr[k] = tdj;
      }
    }
}

/*<#!
' tokenize the input string
' p, q is strlen(p) + 1
\\*/
Stoken *tokenize(char *p,char *q,Stok_dclr *tok_dclr,int len_tok_dclr)
{
  /*
    sort() is required for tok_dclr so 
   */

  sort(tok_dclr,len_tok_dclr);

  //{
  //  int n = 0;
  //  for(;n < len_tok_dclr;++n)
  //    printf("%s\n",tok_dclr[n].regex);
  //}
  
  Stokenizer tokenizer = { .p = p, .q = q };

  int j;
  int maxLevel;
  for(j = 0, maxLevel = 0; j < len_tok_dclr; ++j)
    if(maxLevel < tok_dclr[j].level)
      maxLevel = tok_dclr[j].level;
  maxLevel += 1; // highest possible level + 1

  Stoken *token = NULL; //TODO X,Y

  /*
    go through p to q and tokenize using tok_dclr as base
   */
  int nevermatch;
  nevermatch = 1; //comparison will never be true on any identifier
  while(tokenizer.p < tokenizer.q)
  {
    int level;
    for(level = 0; level < maxLevel; ++level)
    {
      for(j = 0; j < len_tok_dclr; ++j)
      {
	//#define td(N) tok_dclr[N]
        if(level == tok_dclr[j].level)
        {
          char *P; uintptr_t typecount;
	  //skepticism that typecount can > 0 if regex did not compare true 100% (miscellaneous.c)
	  /*
	    BUG "p" matches "print" but typecount is 1
	    "p" matches "a-z" and typecount is 1
	    "print" matches "print" and typecount is 5
	    if actually "p" matched "print" then 
	    typecount should be 5 and not 1!!
	    so either make distiction of regex and nonregex
	    or somehow make up for it in regex_cmp
	    HERE
	    quick-fix don't know if it works so good
	    get typecount1 to check if the typecount
	    gotten is 100%
	    TODO do it better or ensure it is good
	    XXX: it didn't work so i commented it out
	    i just add a nonregex variable to tok_dclr
	    or rather add an isreg
	    maybe better solution in the future
	    with typecount in regex_cmp to check
	    100% this would be for better interface
	    rather than an ugly isreg variable
	    could just be a regex variable
	    and it would do comparison correctly
	    but now the solution is isreg condition
	  */
	  if(tok_dclr[j].isreg)
	    typecount = regex_cmp(tokenizer.p,tok_dclr[j].regex,&P,NULL);
	  else
	    {
	      int len = strlen(tok_dclr[j].regex);
	      if(0 == strncmp(tokenizer.p,tok_dclr[j].regex,
			      len))
		{
		  /* set p correctly */
		  P = tokenizer.p + len;
		  typecount = 1;
		}
	      else
		typecount = 0;
	    }
          if(typecount)
          {
            tokenizer.length = P - tokenizer.p;
            P = tokenizer.p;
            tokenizer.tok_dclr = &(tok_dclr[j]);
    	    tok_dclr[j].tokenizer(&tokenizer);
	    /* check if tokenizer.token was modified 
	       and if then set its X and Y */
	    //if(tokenizer.token != token)
	    //  {
	    //	token->x = tox;
	    //	token->y = toy;
	    //	token = tokenizer.token;
	    //  }
            /* check if tokenizer.p.p was not modified
	       else set it to P which is where upto regex compared true */
            if(tokenizer.p == P)
              tokenizer.p += tokenizer.length;

            nevermatch = 0;
            level = maxLevel;
            break; //break to level < maxLevel for loop
                   //so it can exit and go to the outermost loop
                   //so we then can repeat everything (unless p >= q)
                   //which is tested by the outermost loop
                   //goto'd may be clearer to use here

            //if we do not break here we won't restart
            //the level stuff to 0 and we need to set level
            //to maxLevel so not level < maxLevel
            //level = maxLevel;
            //break;
          }
          else
            ; //do nothing?
        }
	//#undef td
      }
    }

    if(nevermatch)//none of the levels did compare true on p
    {
      fprintf(stderr,"<-error;tokenize; inrecognizable character/string '%c' (typo?)...\n",*tokenizer.p);
      
      //      printf(
      //parse_error(&tokenizer);
      return NULL;
    }
    nevermatch = 1;
  }
  if(!(tokenizer.token))
  {
    fprintf(stderr,"<-error;tokenize; ll of token is void (inexecutable)...\n");
    //parse_error(&tokenizer);
    return NULL;
  }
  /*--------------------------------------------------------*\
  '[OK] reverse singly linked list to get chronological order
  \*--------------------------------------------------------*/
  tokenizer.token = rsll(tokenizer.token,&((tokenizer.token)->nxt));
  /*-------------------- *\
  ' [OK] convert to doubly
  ' d -> c -> b -> a
  ' a -> b -> c -> d
  ' a <- b <- c <- d
  ' a <-> b <-> c <-> d
  \*-------------------- */
  //Stoken *token = tokenizer.token;
  token = tokenizer.token;
  token->prv = NULL;
  while(token->nxt)
  {
    token->nxt->prv = token;
    token = token->nxt;
  }
  return tokenizer.token;
}
