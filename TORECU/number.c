#include "number.h"

TO(number)
{
  totoken(snumber | stype);
  tt.number.n = stoi(to->p);
}
