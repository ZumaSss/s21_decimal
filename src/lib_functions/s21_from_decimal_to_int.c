#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int res = 0;
  if (!dst)
    res = 1;
  else {
    int exp = get_exp(src);
    if (exp > 28)
      res = 1;
    else {
      for (int i = 0; i < exp; i++) div10(&src);
      if (src.bits[1] == 0 && src.bits[2] == 0) {
        *dst = src.bits[0];
        if (get_sign(src)) *dst *= -1;
      } else
        res = 1;
    }
  }
  return res;
}