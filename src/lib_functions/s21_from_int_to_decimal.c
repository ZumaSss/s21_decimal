#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int res;
  if (dst == NULL)
    res = 1;
  else {
    clear_decimal(dst);
    dst->bits[0] = src < 0 ? src * (-1) : src;
    if (src < 0) set_sign(dst);
    res = 0;
  }
  return res;
}