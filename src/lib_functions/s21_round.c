#include "../s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int res = 0;

  if (!result)
    res = 1;
  else {
    s21_decimal one = (s21_decimal){{1, 0, 0, 0}};
    s21_decimal tmp_int;
    s21_decimal tmp_float;
    s21_decimal half = (s21_decimal){{5, 0, 0, 0x00010000}};

    int sign = get_sign(value);

    value.bits[3] &= ~(1UL << 31);
    s21_truncate(value, &tmp_int);
    s21_sub(value, tmp_int, &tmp_float);

    if (s21_is_greater_or_equal(tmp_float, half))
      s21_add(tmp_int, one, result);
    else
      *result = tmp_int;
    if (sign) {
      set_sign(result);
    }
  }

  return res;
}
