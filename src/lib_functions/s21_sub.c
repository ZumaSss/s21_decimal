#include "../s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal temp, nullDecimal = {0};
  clear_decimal(result);

  int sign1 = get_sign(value_1);
  int sign2 = get_sign(value_2);
  int exp1 = get_exp(value_1);
  int exp2 = get_exp(value_2);
  int exp = 0;
  int status = 0;

  int remainder = 0;
  int bit1, bit2;
  int resultBit = 0;

  if (sign1 == 1 && sign2 == 0) {
    set_sign(&value_2);
    status = s21_add(value_1, value_2, result);
  } else if (sign1 == 0 && sign2 == 1) {
    value_2.bits[3] = exp2 << 16;
    status = s21_add(value_1, value_2, result);
  } else {
    if (exp1 != exp2 && (exp1 != 0 || exp2 != 0))
      exp = normalize_add(&value_1, &value_2);
    else if (exp1 == exp2 && (exp1 != 0 || exp2 != 0))
      exp = exp1;

    unset_bit(&value_1, 31, 3);
    unset_bit(&value_2, 31, 3);

    if (s21_is_less(value_1, value_2)) {
      copy_to_buffer(value_1, &temp);
      copy_to_buffer(value_2, &value_1);
      copy_to_buffer(temp, &value_2);
      if (!sign1) set_sign(result);
    } else {
      if (sign1) set_sign(result);
    }

    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < 32; i++) {
        bit1 = check_bit(value_1, i, j);
        bit2 = check_bit(value_2, i, j);

        resultBit = subtraction(bit1, bit2, &remainder);
        if (resultBit == 1) {
          set_bit(result, i, j);
        }
      }
    }
    copy_to_buffer_no_exp(*result, &temp);
    if (s21_is_equal(temp, nullDecimal))
      result->bits[3] = 0;
    else
      result->bits[3] = exp << 16 | get_sign(*result) << 31;
    if (get_sign(*result) == 1 && status == 1) status = 2;
  }

  return status;
}