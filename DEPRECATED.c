/*
  do not use this function on an affix of 0 arity
  what to do about 0 affix? don't call affix2callable within retokenizer affix2callable require arity > 0
*/
/*
  take an affix that occurs as ll; PREFIX -> a -> b -> c -> d
  make a callable that *swallow* a -> b -> c -> d as an argv
  convert affix to callable / make a callable out of affix
 */
void affix2callable(Stoken *token /* affix/prefix/infix/suffix */) /* token.c */
{
  Stoken *l, *r;
  int count = 1; /* atleast 1 argument
		    an affix or callable without argument
		    got nothing to affix or call on */

  /*
    A  // affix
    a  // argument
    -> // nxt
    A -> a -> a -> a -> a
    c // callable
    c a a a a
   */
  // obtain arguments
  switch(tt.affix.type)
    {
    case PREFIX:{
      /* echo 'hello' */
      l = r = token->nxt; /* 1st argument */
      /*
	1 < 2 == true
	r = 2nd-argument
	2 < 2 == false
	break
	unlink_lr(l,r)
       */
      while(count++ < tt.affix.arity)
	r = r->nxt;
      break;
    }
    case  INFIX:{
      // obtain r
      l = token->prv;
      r = token->nxt;
      //l = r = token->nxt;
      while(count++ < tt.affix.arity)
	r = r->nxt;
      
      unlink(token);
      //ERROR CHECK REMOVE FOLLOWING IF STATEMENT
      //if(token->prv == NULL && token->nxt == NULL){
      //	printf("affix2callable; NULL token\n");
      //}
      //else printf("affix2callable; NULL token\n");
      // obtain l
      count = 1;
      while(count++ < tt.affix.arity)
	l = l->prv;

      /*
	linkin_at_lr(a,b,c) may be linkin_lr(a,b,c,d)
	but linkin_at_lr is not defined anywhere and linkin_lr
	accepts an additional argument
	TODO inspect what exactly linkin_lr does and whether that one works for
	the scenario here
	maybe linkin(src,l,r) is sufficient?
	there were some changes to those macros unlink* and linkin*
	and they require further work too <- TODO
       */
      //linkin_at_lr(l,r,token); //PREVIOUS PREVIOUS
      //linkin_lr(token,token,l,r); //PREVIOUS
      /*
	l = l-most-arg of infix token
	r = r-most-arg of infix token
	
       */
      
      //linkin(token,l,r); //UNCOMMENT???????????? IMPORTANT
      

      // linkin token
      //r->nxt->prv = token;
      //l->prv->nxt = token;
      //token->nxt = r->nxt;
      //token->prv = l->prv;
      
      break;
    }
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

  //EOF
  /* 
     some kind of ttn_eol / ttn_eof / NULL condition is required for PREFIX a
   */

  /* 
     l to r supposed to be cut off from the ll which is token
     token alone currently of ttn affix
     then changed to ttn callable 
     and put the *cut off* l to r ll at callable's argv
   */
  
  //token->nxt = r->nxt;//it would be NULL or ttn_eof/ttn_eol/ttn_nil
  //token->prv = l; //????? for PREFIX this doesn't make sense
  /* l->prv->nxt token->nxt == l->nxt r->nxt->prv */
  unlink_lr(l,r); /* segmentation fault FIX ttn_eof/ttn_eol/ttn_nil READ unlink_lr*/
  //puts("hello");exit(1);

  /* divide into another step / routine */
  
  Zcallable callable = {ttn_callable};
  callable.argc = tt.affix.arity;
  callable.argv = malloc(sizeof(Stoken *) * callable.argc);

  count = 0; //
  do{
    callable.argv[count++] = l; //argv[0]->nxt is already argv[1]->nxt
    l = l->nxt;
  }while(l);

  tt.callable = callable;
}

/* deprecated for unambiguity and ambiguity */
Stoken *un_slash_ambiguity(Stoken *initial /* the affix which needs to solve its argument list */)
{
  if(TT(initial).affix.type == SUFFIX)
    //return token;
    return initial;

  Stoken *token;
  int count;
  //printf("ttn; %d\n",tt.number);
 repeat:
  token = initial->nxt; count = 0; /* 0 is 1st argument, 1 is 2nd argument */
  do{
    if(tt.number == eof_ttn)
      {
	fprintf(stderr,"<-error;un_slash_ambiguity; affix expected an arity of %d, but an arity of %d was provided\n",tt.affix.arity,count);
	exit(1);
      }
    
    if(tt.number == ttn_affix)
      {
	tt.affix.retokenizer(token);
	/* the retokenizer()' affix may've
	   swallowed (made itself callable) counted arguments,
	   thus we need to recount how many there are */
	{Stoken *tmp = initial->nxt; int recount = 0; /* 0 is 1st argument, 1 is 2nd argument */
	  while(tmp != token)
	    { tmp = tmp->nxt; ++recount; }
	  if(recount != count)
	    count = recount;}
      }
    token = token->nxt;
    //if(token == NULL){
    //  printf("here\n");
    //  exit(1);
    //}
      
  }while(++count < TT(initial).affix.arity);


  //printf "TEXT" NULL?????????????????TODO PREPEND/APPEND or this
  //EOF?
  //if(token == NULL){/* TODO eof_ttn / eol_ttn and NULL */
  //  return initial;}
  if(tt.number == ttn_eof){
    return initial;}
  //printf("hello1\n");exit(1);
  
  /*<#!
  ' test for ambiguity
  \\*/
  //if(token != NULL) /* CASE PREFIX ARITY 1 EOF tt.number != ttn_eof eof */
  for(count = 0;1;++count)
    {
      token = token->nxt; //token is already at the last argument to initial, so this will set it to the next token

      if(tt.number == ttn_eof)
	{ return initial; }

      /* infix of arity 1 implies 2 argument? */
      if(tt.number == ttn_affix)
	{
	  /*
	    if it(token) is a PREFIX
	    then no more is to be done
	    a PREFIX on R(initial) does not operate on a PREFIX on L(current/tt/token)
	    if count(initial's) >= tt(token/current).affix.arity 
	  */
	  /* infix has arity number on each side of the affix */
	  if(tt.affix.type == PREFIX || count >= tt.affix.arity){
	    /* 
	       0 1
	       (P A B) [Z X]     1 >= 1 == true -> unambiguity
	       1ARITY
	       0 1
	       (P A [B) Z X]     1 >= 2 == false -> ambiguity
	       2ARITY
            
	    */
	    return initial;
	  }
      
	  /* solve right side first */
	  if(lorr(initial,token)){
	    goto repeat;
	  }

	  /* else solve left side first */
	  break;
	}
    }

  return initial;
}

Stoken *ambiguity(Stoken *root,Stoken *end_of_argl ) /* IGNORE FOR NOW */
{
  /*<#!
    ' test for ambiguity
  \\*/
  //if(token != NULL) /* CASE PREFIX ARITY 1 EOF tt.number != ttn_eof eof */
  for(count = 0;1;++count)
    {
      token = token->nxt; //token is already at the last argument to initial, so this will set it to the next token

      if(tt.number == ttn_eof)
	{ return initial; }

      /* infix of arity 1 implies 2 argument? */
      if(tt.number == ttn_affix)
	{
	  /*
	    if it(token) is a PREFIX
	    then no more is to be done
	    a PREFIX on R(initial) does not operate on a PREFIX on L(current/tt/token)
	    if count(initial's) >= tt(token/current).affix.arity 
	  */
	  /* infix has arity number on each side of the affix */
	  if(tt.affix.type == PREFIX || count >= tt.affix.arity){
	    /* 
	       0 1
	       (P A B) [Z X]     1 >= 1 == true -> unambiguity
	       1ARITY
	       0 1
	       (P A [B) Z X]     1 >= 2 == false -> ambiguity
	       2ARITY
            
	    */
	    return initial;
	  }
      
	  /* solve right side first */
	  if(lorr(initial,token)){
	    goto repeat;
	  }

	  /* else solve left side first */
	  break;
	}
    }
  return root;
}

Stoken *un_slash_ambiguity(Stoken *root)
{
}
