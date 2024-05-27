#include "../s21_decimal.h"

int s21_is_greater(s21_decimal d1, s21_decimal d2) {
  return s21_is_less(d1, d2) | s21_is_equal(d1, d2) ? FALSE : TRUE;
}