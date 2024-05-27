#include "../s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int res = 0;

  if (!result) {
    res = 1;
  } else {
    int sign = get_sign(value);
    int exp = get_exp(value);
    *result = value;
    result->bits[3] = 0;

    while (exp > 0) {
      int low = 0;
      unsigned long long temp = 0;
      for (int j = 2; j >= 0; j--) {
        temp = (low * 4294967296 + result->bits[j]);
        low = temp % 10;
        temp = temp / 10;
        result->bits[j] = temp;
      }
      exp--;
    }
    if (sign) {
      set_sign(result);
    }
  }
  return res;
}
