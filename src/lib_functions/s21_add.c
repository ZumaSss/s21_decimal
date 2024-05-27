#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal ten;
  clear_decimal(result);
  s21_from_int_to_decimal(10, &ten);

  int remainder = 0;
  int bit1, bit2;
  int resultBit = 0;
  int sign1 = get_sign(value_1);
  int sign2 = get_sign(value_2);
  int exp1 = get_exp(value_1);
  int exp2 = get_exp(value_2);
  int exp = 0;
  int status;

  if (sign1 == 1 && sign2 == 0) {
    value_1.bits[3] = exp1 << 16;
    status = s21_sub(value_2, value_1, result);
  } else if (sign1 == 0 && sign2 == 1) {
    value_2.bits[3] = exp2 << 16;
    status = s21_sub(value_1, value_2, result);
  } else {
    if (exp1 != exp2 && (exp1 != 0 || exp2 != 0))
      exp = normalize_add(&value_1, &value_2);
    else if (exp1 == exp2 && (exp1 != 0 || exp2 != 0))
      exp = exp1;

    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < 32; i++) {
        bit1 = check_bit(value_1, i, j);
        bit2 = check_bit(value_2, i, j);

        resultBit = addition(bit1, bit2, &remainder);
        if (resultBit == 1) {
          set_bit(result, i, j);
        }
      }
    }

    result->bits[3] = exp << 16 | (sign1 | sign2) << 31;
    if (get_sign(*result) == 1 && remainder == 1) {
      remainder = 2;
    }
    status = remainder;
  }

  return status;
}

int normalize_add(s21_decimal *value_1, s21_decimal *value_2) {
  int grade_1 = 0, grade_2 = 0;
  s21_decimal nullDecimal = {0}, temp;

  int exp_1 = get_exp(*value_1), exp_2 = get_exp(*value_2);
  set_exp(value_1, 0);
  set_exp(value_2, 0);

  unset_bit(value_1, 31, 3);
  unset_bit(value_2, 31, 3);

  if (exp_1 > exp_2) {
    copy_to_buffer_no_exp(*value_2, &temp);
    while (s21_is_greater(temp, nullDecimal)) {
      grade_2++;
      div10(&temp);
    }
    while (exp_1 != exp_2) {
      if (grade_2 == 29) {
        bank_round(value_1);
        exp_1--;
        if (s21_is_equal(*value_1, nullDecimal)) {
          value_2->bits[0]--;
          exp_1 = exp_2;
          break;
        }
      } else {
        decimal_pow(value_2, 1);
        grade_2++;
        exp_2++;
      }
    }
  } else if (exp_1 < exp_2) {
    copy_to_buffer_no_exp(*value_1, &temp);
    while (s21_is_greater(temp, nullDecimal)) {
      grade_1++;
      div10(&temp);
    }
    while (exp_1 != exp_2) {
      if (grade_1 == 29) {
        bank_round(value_2);
        exp_2--;
        if (s21_is_equal(*value_1, nullDecimal)) {
          value_2->bits[0]--;
          exp_1 = exp_2;
          break;
        }
      } else {
        decimal_pow(value_1, 1);
        grade_1++;
        exp_1++;
      }
    }
  }
  set_exp(value_1, exp_1);
  set_exp(value_2, exp_2);

  return exp_1;
}