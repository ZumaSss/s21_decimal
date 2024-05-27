#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int res = 0;
  if (!result) {
    res = 1;
  } else {
    if (get_sign(value)) {
      s21_decimal one = {{1, 0, 0, 0}};

      s21_negate(value, &value);
      s21_truncate(value, result);
      if (!s21_is_equal(value, *result)) s21_add(*result, one, result);
      set_sign(result);
    } else
      s21_truncate(value, result);
  }
  return res;
}
