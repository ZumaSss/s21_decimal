#include "../s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int result = 0;
  if (!dst)
    result = 1;
  else if (isinf(src) || isnan(src) || (fabsf(src) >= MAX_DEC))
    result = 1;
  else if (fabsf(src) <= MIN_DEC) {
    clear_decimal(dst);
    result = 1;
  } else {
    clear_decimal(dst);
    unsigned int float_bits = *(int *)(void *)&src;

    int exp_src = ((float_bits << 1) >> 24) - 127;
    unsigned int mantissa = ((float_bits << 9) >> 8) | (1 << 24);
    while ((mantissa & 0x1) == 0) mantissa >>= 1;

    unsigned int mantissa_temp = mantissa;
    while (mantissa_temp != 1) {
      exp_src--;
      mantissa_temp >>= 1;
    }

    int isNeg = 0;
    if (exp_src < 0) {
      isNeg = 1;
      exp_src *= -1;
    }
    s21_decimal dec1 = {{1, 0, 0, 0}}, dec2 = {{mantissa, 0, 0, 0}};
    left_shift(&dec1, exp_src > 95 ? 95 : exp_src);

    if (isNeg)
      s21_div(dec2, dec1, dst);
    else
      s21_mul(dec1, dec2, dst);

    s21_decimal nullDecimal = {0}, temp;
    copy_to_buffer_no_exp(*dst, &temp);
    int exp = 0;

    while (s21_is_not_equal(nullDecimal, temp)) {
      div10(&temp);
      exp++;
    }
    exp -= 10;
    int exp_dst = get_exp(*dst);
    for (int i = 0; i < exp; i++) {
      bank_round(dst);
      exp_dst--;
    }

    if (exp_dst < 0) {
      decimal_pow(dst, -exp_dst);
      exp_dst = 0;
    }
    if (dst->bits[0] == 0 && dst->bits[1] == 0 && dst->bits[2] == 0)
      set_exp(dst, 0);
    else
      set_exp(dst, exp_dst);
    if (src < 0) set_sign(dst);
  }

  return result;
}