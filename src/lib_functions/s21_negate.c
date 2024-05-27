#include "../s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int res = 0;
  if (!result)
    res = 1;
  else {
    *result = value;
    if (get_sign(*result))
      result->bits[3] &= ~(1UL << 31);
    else
      result->bits[3] |= (1UL << 31);
  }

  return res;
}