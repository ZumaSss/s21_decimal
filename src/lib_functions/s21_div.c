#include "../s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  clear_decimal(result);
  if (s21_is_equal(value_2, *result)) return 3;

  int status = 0;

  int sign1 = get_sign(value_1), sign2 = get_sign(value_2);
  int exp_divisible = get_exp(value_1), exp_divider = get_exp(value_2);
  int exp = exp_divisible - exp_divider;
  copy_to_buffer_no_exp(value_1, &value_1);
  copy_to_buffer_no_exp(value_2, &value_2);

  int pos;
  int first_pos = get_first_bit(value_2);
  int num;

  s21_decimal quot;
  clear_decimal(&quot);
  s21_decimal divider;
  clear_decimal(&divider);
  s21_decimal nullDecimal;
  clear_decimal(&nullDecimal);

  while (1) {
    while (s21_is_less_or_equal(value_2, value_1)) {
      pos = 0;
      num = 0;

      while (s21_is_less_or_equal(value_2, value_1) && first_pos + pos < 95) {
        left_shift(&value_2, 1);
        pos++;
      }

      right_shift(&value_2, pos);

      if (first_pos + pos != 95) pos--;
      if (pos > 31 && pos < 64) {
        pos -= 32;
        num = 1;
      } else if (pos > 63 && pos < 96) {
        pos -= 64;
        num = 2;
      }
      set_bit(&nullDecimal, pos, num);

      copy_to_buffer_no_exp(value_2, &divider);
      left_shift(&divider, num > 0 ? pos + 32 * num : pos);
      s21_sub(value_1, divider, &value_1);
    }
    status = s21_add(quot, nullDecimal, &quot);
    if (status > 0) break;
    clear_decimal(&nullDecimal);

    if (s21_is_not_equal(value_1, nullDecimal) && exp < 28) {
      decimal_pow(&value_1, 1);
      decimal_pow(&quot, 1);
      exp++;
    } else
      break;
  }

  if (status == 0) {
    while (exp < 0) {
      if (get_first_bit(quot) == 95) {
        status = 1;
        break;
      }
      status = decimal_pow(&quot, 1);
      if (status > 0) {
        return status;
      }
      exp++;
    }
    set_exp(&quot, exp);
    copy_to_buffer(quot, result);
  }

  if ((sign1 ^ sign2) == 1) set_bit(result, 31, 3);
  if (get_sign(*result) == 1 && status == 1) status = 2;

  return status;
}