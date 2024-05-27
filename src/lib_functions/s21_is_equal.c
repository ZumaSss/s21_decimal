#include "../s21_decimal.h"

int s21_is_equal(s21_decimal d1, s21_decimal d2) {
  int res = TRUE, status = 0;

  if (d1.bits[0] == 0 && d1.bits[1] == 0 && d1.bits[2] == 0 &&
      d2.bits[0] == 0 && d2.bits[1] == 0 && d2.bits[2] == 0)
    res = 1;
  else if (get_sign(d1) == get_sign(d2)) {
    int exp1 = get_exp(d1), exp2 = get_exp(d2);
    if (exp1 < exp2)
      status = decimal_pow(&d1, exp2 - exp1);
    else if (exp1 > exp2)
      status = decimal_pow(&d2, exp1 - exp2);

    if (status > 0) {
      res = FALSE;
    } else {
      for (int i = 2; i >= 0; i--) {
        if (d1.bits[i] != d2.bits[i]) {
          res = FALSE;
          break;
        }
      }
    }
  } else
    res = FALSE;

  return res;
}