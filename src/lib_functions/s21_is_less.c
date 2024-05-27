#include "../s21_decimal.h"

int s21_is_less(s21_decimal d1, s21_decimal d2) {
  int res = FALSE;
  int sign1 = get_sign(d1), sign2 = get_sign(d2);
  int exp1 = get_exp(d1), exp2 = get_exp(d2);
  if (exp1 < exp2)
    decimal_pow(&d1, exp2 - exp1);
  else if (exp1 > exp2)
    decimal_pow(&d2, exp1 - exp2);

  if (s21_is_not_equal(d1, d2)) {
    if (sign1 > sign2)
      res = TRUE;
    else if (sign1 == sign2) {
      if (!sign1) {
        for (int i = 2; i >= 0; i--) {
          if (d1.bits[i] < d2.bits[i]) {
            res = TRUE;
            break;
          } else if (d1.bits[i] > d2.bits[i])
            break;
        }
      } else {
        for (int i = 2; i >= 0; i--) {
          if (d1.bits[i] > d2.bits[i]) {
            res = TRUE;
            break;
          } else if (d1.bits[i] < d2.bits[i])
            break;
        }
      }
    }
  }

  return res;
}