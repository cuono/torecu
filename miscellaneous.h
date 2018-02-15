#ifndef MISCELLANEOUS_H
#define MISCELLANEOUS_H

/* welcome to hell */

#include <stdint.h> //uintptr_t
#include <stdio.h>

int stoi(char *s);

//exit the program after displaying a cool error message
#define errxit(MSG) fprintf(stderr,"!OMG says file \"%s\" line %d function %s; %s\n",__FILE__,__LINE__,__func__,MSG),exit(1);

//like strcmp() but case-insensitive
int STRICMP(const char *str1,const char *str2);

//compare str1 with str2 where str2 is the address to a char * in an array index. sizeof_type is the size of an index of the array. numof_indices is the total number of indices in the array. strcmp is a function pointer to either strcmp() or stricmp().
int scmpa(void *str1, void *str2, int sizeof_type, int numof_indices, int(*strcmp)(const char*,const char*));

//reverse singly linked list. base_address is the address to the linked list's struct's 1st member. nextptr_address is the address to the next member pointing to the next node within the linked list. both base_address and nextptr must refer to the same structure in memory.
void *rsll(void *base_address, void *nextptr_address);

/*
  struct name{
    type some_member; //base_address (the first member/memory address)
    type a;
    type b;
    type c; //etc ...
    struct name *nxt; //it's singly linked list but you can have a prv member and call it on that which would go the opposite direction instead of the right direction
  }
 */

//toggle the null terminator on p by a regex
uintptr_t regex_cmp_toggle_null_terminator_on_string(char *p/*string*/, char *q/*regex*/);

//return a number up to which type within regex that compared true on string, ie, an index where each quantity is an increment counting a type that was chronologically specified within char *regex. P and Q can be left NULL; they're used for recursiveness
int regex_cmp(char *str, char *regex/*, int *part*/, char **P, char **Q);

#endif
