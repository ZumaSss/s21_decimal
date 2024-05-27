#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int res = 0;
  if (!dst)
    res = 1;
  else {
    double temp = (double)*dst;
    for (int j = 0; j < 3; j++)
      for (int i = 0; i < 32; i++)
        temp += check_bit(src, i, j) * pow(2, j > 0 ? i + j * 32 : i);
    temp *= pow(10, -get_exp(src));
    if (get_sign(src)) temp *= -1;
    *dst = temp;
  }
  return res;
}