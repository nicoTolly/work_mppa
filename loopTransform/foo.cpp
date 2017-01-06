#include <iostream>

int main()
{
  int a = 1;
#ifdef DIV_ZERO
  a = 21 / 0;
#endif
  return a;
}
