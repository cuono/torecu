#include <string.h> //strcmp()
#include "miscellaneous.h"

/* str terminated by non 
   numerical literal or \0 is expected
   return int i */
int stoi(char *s)
{
  //int len = strlen(s);
  int len = 0;
  while(s[len] >= '0' && s[len] <= '9')
    ++len;
  int i = 0;
  int p = 1;
  while(len){
    i += (s[--len] - 48) * p;
    p *= 10;
  }
  return i;
}

/* count 2 node from base_address nd return it 
   the nxtptr_address can be a prvptr_address */
void *nxtsll(void *base_address,void **nxtptr_address,int count);

int STRICMP(const char *str1,const char *str2)
{
  while(*str1 && *str2 && tolower(*str1) == tolower(*str2))
    str1++,str2++;
  if (*str1 < *str2)
    return -1;
  //printf("%d\n", 'a' - 'A');
  //printf("%c = %c\n", tolower(*str1), tolower(*str2));
  return *str1 != *str2;
}

int scmpa(void *str1, void *str2, int sizeof_type, int numof_indices, int(*strcmp)(const char*,const char*))
{
  uintptr_t ptr = (uintptr_t)str2;
  /*ptr += sizeof_type * 3;
  printf("%d\n",sizeof_type);
  printf("%s\n",*(void**)ptr);
  return 0;*/
  int n;
  for(n = 0; n < numof_indices; ++n, ptr += sizeof_type)
    if(*(void**)ptr == NULL) //will account for `(null)' member
      ;
    else
    if(!strcmp(str1,*(void**)ptr))
      return n;
  return -1;
}

/*
    Reverse a generic singly linked list
    ------------------------------------
    The algorithm
    -------------
    list_t *reverse(list_t *a)
    {
      list_t *progress = NULL;
      list_t *b;
      while(a)
      {
        b = a->next;
        a->next = progress;
        progress = a;
        a = b;
      }
      return progress;
    }
    --------------------------
    base_address must be the starting location in memory of an instance of a structure.
    nextptr_address must be the address to this particular instance's NEXT pointer.
    
      list = rsll(list,&(list->next));
      //list's last ->next is assumed to be NULL
    -------------------------------------------------------------------------------------
*/
void *rsll(void *base_address, void *nextptr_address /*(void **)*/)
{
  uintptr_t offset, holder;
  offset = nextptr_address - base_address;

  void **p = base_address, *progress = NULL;
  while(p)
  {
    void *b;
    holder = (uintptr_t)p;
    holder += offset;
    p = (void**)holder; //&(N->next)
    b = *p; //(N->next)
    *p = progress; //(N->next)
    holder = (uintptr_t)p;
    holder -= offset;
    p = (void**)holder; //N
    progress = p;
    p = b;
  }
  return progress;
}

//toggle null on p based on q (which is the regex_cmp)
uintptr_t regex_cmp_toggle_null_terminator_on_string(char *p/*string*/, char *q/*regex*/)
{
  static char c, *P = NULL;
  if(p)
  {
    int part = regex_cmp(p,q,&P,NULL);
    if(part)
    {
      c = *P;
      *P = '\0';
    }
    else
      P = NULL;
    return part;
  }
  else
  {
    if(!P)
    {
      return 0;
    }
    *P = c;
    q = P;
    P = NULL;
    return (uintptr_t)q;
  }
}

/*
  WILDCARD [NO] eg ".+"
  i deleted all the commenting, i rewrote it just below.
  read the code to get a grasp of it.
  
  char **P, char **Q; //are used during the recursive call to regex_cmp during sundry
  char *str, char *regex; //are self-explainatory
*/
int regex_cmp(char *str, char *regex, char **P, char **Q)
{
  /*
    the code structure of the algorithm in chronological order
      1. TYPE
      2. COMPARISON
      3. OPERATION
     [4. COMPARISON] #this stage does not always happen, i think

    a type is either
      
      sundry ;; a string enclosed () passed for comparison to regex_cmp
      ranged ;; two characters separated by hyphen to compare true on
                any character inclusively inbetween the two characters, eg,
                a-z will compare true on any lowercase alphabetic letter
      single ;; a single character
                NOTE: a character must be one byte long

    an operation is either
      
      repeat     ;; a + sign specified after a type to indicate that the
                    comparison will continue to compare true forever until
                    it compares false NOTE; atleast one true comparison is enough
		    if it doesn't compares true atleast once then this operation fails
      logical or ;; a | sign specified inbetween types to indicate
                    that the comparison will continue until any type,
                    part of the logical or sequence compares true
                    chronologically. if no type compares true, then fail otherwise success.
  */
  char *qstash;   //during sundry, qstash is (HERE), while q is ()HERE
  char *Qstash;   //during sundry, Qstash is ()HERE, while q is ()+|HERE
  int typetype;   //type of type, which is the type of the currently processing type, ie, sundry or ranged or single
                  #define sundry 0
                  #define ranged 1
                  #define single 2
  int repeattrue;     //continue repetition of comparison while this flag is true
  int pipe;           //maybe later, there might be operations after '|' ('|' is after '+')
  int orsequence = 0; //whether we're currently processing a logical OR sequence
  int comparison = 0; //whether the comparison was true
  int comparison_wildcard_dot = 0; //NON-IMPLEMENTED special case comparison like ".+"
  int partA = 0;      //index into the last type of regex that matched, but 0 is none
  int partB = 0;      //i don't remember what this is for??
  int partC = 0;      //i don't remember what this is for??

  char *p = str, *q = regex, a, b;
  while(/**p &&*/ *q)
  {
    //"\(" , "\)"
    if(q[0] == ')')
    {
      ++q;
      comparison = 1;
      goto exit;
    }
    if(q[0] == '(')
    {
      typetype = sundry;
      qstash = ++q;//(HERE
      int n;
      n = 1;
      for(;n;++q)
        if(*q == '(')
          ++n;
        else
        if(*q == ')')
          --n;
      Qstash = q;
      //++q;//)HERE
      //printf("%c\n",37);
    }
    else
      {
	/*
	  when a backslash is encountered, then, increase q by 1; in effect allowing '(' and ')' to occur
	  within a ranged or as a single. there is no need to escape ranged's 2nd operand, and it is unsupported.
	  '+' can be escaped as +-+ or \+, however, '(' and ')', must escape using a backslash.
	 */
	if(q[0] == '\\')
	  {
	    ++q;
	  }
	if(q[1] == '-')
	  {
	    typetype = ranged;
	    if(q[0] > q[2])
	      a = q[2], b = q[0];
	    else
	      a = q[0], b = q[2];
	    q += 3;//a-zHERE
	  }
	else
	  if(q[1] != '-')
	    {
	      typetype = single;
	      a = b = q[0];
	      q += 1;//aHERE
	    }
      }

    repeattrue = *q == '+' && ++q;
          pipe = *q == '|' && ++q;
    

    if(pipe && !orsequence) //firs element of orsequence
    {
      orsequence = 1;
      partC = partA;
    }

    if(orsequence == 2)
      ;//printf("2: %s\n",q);//skip rest of comparison operations
    else
    if(typetype == ranged || typetype == single)
      for(comparison = 0;*p;++p)
        if(*p >= a && *p <= b)
        {
          //printf("true: %c >= %c && %c <= %c\n",*p, a, *p, b);
          comparison = 1;
          if(orsequence == 1)
            orsequence = 2, partB = partA;
          if(repeattrue)//progress p until false
            continue;
          ++p;
          break;
        }
        else
        {
          //printf("false: %c >= %c && %c <= %c\n",*p, a, *p, b);
          break;
        }
    else
    if(typetype == sundry)
    {
      char *tmp, *tmpQ;
      int result;
      comparison = 0; //important, forgot it
      for(;*p;)
      {
        /*result = */regex_cmp(p,qstash,&tmp,&tmpQ/*q*/);
        /*
        if(*(tmpQ-1) == ')') //this means result is true (on first thought, but be 100% sure before assuming it)
          puts("YE");
            01(a(bc(d)e)f)23
            there are 3 sundries
            if the 3rd is successfull, but the single 'e' is not
            Q = e)f)
            *(Q-1) == ')' is true HOWEVER the sundry failed because 'e' was untrue...
            so, if un1r returns part, then a sundry success cannot be determined by *(tmpQ-1) == ')'.
            Qstash is set to ()HERE, qstash is (HERE), q is ()[+|]HERE.
            if Qstash == tmpQ
              comparison = 1

            result != 0 would be sufficient, but not if un1r returns part (which can be positive regardless)

            why not communicate comparison as a function argument instead?
        */
        if(tmpQ == Qstash)
        {
          //puts("UES");
          result = 1;
        }
        else
          result = 0;
        if(result)
        {
          comparison = 1;
          if(orsequence == 1)
            orsequence = 2, partB = partA;
          p = tmp;
          if(repeattrue)
            continue;
          break;
        }
        else
        {
          break;
        }
      }
    }
    /*
        1:
          B 1
          A 1
        2:
          B 1
          A 2
        3:
          B 1
          A 3
        0:
          A - B = 3 - 1 = 2
          B - A = 1 - 3 = -2

          A + (B - A) = 3 +(1 - 3) = 3 +(-2) = 3 -2 = 1
    */

    if(!pipe && orsequence) //last element of orsequence
    {
      if(!comparison)
      {
        //if(*q)
        //  partA = partA + (partB - partA);
        partA = partC;
        goto exit;
      }
      orsequence = 0;
      if(!*q || !*p) //is this reliable way to determine end of regex and end of p?
                     //not sure about: || !*p
      {
        //puts("OK");
        partA = partA + (partB - partA);
      }
    }

    if(!comparison && !orsequence)
      goto exit;

    ++partA;
  }
  
  exit:

    if(P)
      *P = p;
    if(Q)
      *Q = q;
    //if(part)
    //{
    //  //printf("B: %d\n",partB);
    //  //printf("A: %d\n",partA);
    //  *part = partA;
    //}
    //if(!*p && *q)//
    //  comparison = 0;
    return partA;
    return comparison;
}
