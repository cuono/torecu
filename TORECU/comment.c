#include "comment.h"

TO(comment)
{
  while(*(to->p++) != '\n');
  to->p -= 1;
}
