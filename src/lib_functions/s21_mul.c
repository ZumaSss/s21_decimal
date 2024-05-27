#include "../s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int bit;
  s21_decimal mulBuffer;
  s21_decimal zero = {{0, 0, 0, 0}};
  clear_decimal(&mulBuffer);
  clear_decimal(result);
  copy_to_buffer_no_exp(value_2, &mulBuffer);
  s21_decimal resultBuffer;
  clear_decimal(&resultBuffer);

  int sign1 = get_sign(value_1), sign2 = get_sign(value_2);
  int exp1 = get_exp(value_1), exp2 = get_exp(value_2);
  int exp = exp1 + exp2;
  int status = 0;
  int x = normalize_mantissa(&value_1, value_2);

  if (s21_is_equal(value_1, zero) || s21_is_equal(value_2, zero)) {
    status = 0;
  } else {
    if (x == 0) {
      for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 32; i++) {
          bit = check_bit(value_1, i, j);
          if (bit == 1) {
            if (check_bit(*result, 31, 2) && exp1 == 0 && exp2 == 0) {
              status = 1;
              break;
            }
            copy_to_buffer_no_exp(*result, &resultBuffer);
            clear_decimal(result);
            s21_add(mulBuffer, resultBuffer, result);
          }

          left_shift(&mulBuffer, 1);
        }
        if (status) break;
      }
    } else {
      copy_to_buffer_no_exp(value_1, result);
      if (x > exp)
        status = 1;
      else
        exp -= x;
    }

    while (exp > 28) {
      bank_round(result);
      exp--;
    }
    set_exp(result, exp);

    if ((sign1 ^ sign2) == 1) set_bit(result, 31, 3);
    if (get_sign(*result) == 1 && status == 1) status = 2;
  }

  return status;
}

int normalize_mantissa(s21_decimal *value_1, s21_decimal value_2) {
  s21_decimal a, b, p, q, result;
  copy_to_buffer_no_exp(*value_1, &a);
  copy_to_buffer_no_exp(value_2, &b);

  int x = 0, x1;
  while (get_first_bit(a) + get_first_bit(b) >= 96) {
    div10(&a);
    x++;
  }

  if (x != 0) {
    for (int i = 0; i < x; i++) div10(&b);

    p = mod10(*value_1, x);
    q = mod10(value_2, x);

    x1 = normalize_mantissa(&p, q);

    s21_mul(a, b, &result);
    decimal_pow(&result, x);
    s21_mul(a, q, &a);
    s21_mul(b, p, &b);
    s21_mul(p, q, &p);
    for (int i = 0; i < x - x1; i++) div10(&p);

    s21_add(result, a, &result);
    s21_add(result, b, &result);
    s21_add(result, p, &result);

    copy_to_buffer(result, value_1);
  }

  return x;
}