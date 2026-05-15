#include "math.h"

uint32_t gcd(uint32_t x, uint32_t y)
{
  if (y == 0)
    return x;
  return gcd(y, x % y);
}


int32_t interpolate(int32_t value, int32_t value_from, int32_t value_to, int32_t from, int32_t to) {
  return from + value * (to - from) / (value_to - value_from); 
}
