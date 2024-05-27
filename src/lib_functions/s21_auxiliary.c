#include "../s21_decimal.h"

int check_bit(s21_decimal value, const int pos, int num) {
  return ((value.bits[num] & (1 << pos)) != 0);
}

void set_bit(s21_decimal *value, const int pos, int num) {
  value->bits[num] |= (1 << pos);
}

void unset_bit(s21_decimal *value, const int pos, int num) {
  value->bits[num] &= ~(1 << pos);
}

int get_first_bit(s21_decimal value) {
  int bit = 31;
  int num = 2;
  for (; bit >= 0 && check_bit(value, bit == 32 ? bit-- : bit, num) == 0;
       bit--) {
    if (bit == 0 && num > 0) {
      bit = 32;
      num -= 1;
    }
  }
  if (num > 0) bit += num * 32;
  return bit;
}

void clear_decimal(s21_decimal *dec) {
  dec->bits[0] = dec->bits[1] = dec->bits[2] = dec->bits[3] = 0;
}

int copy_to_buffer_no_exp(s21_decimal value, s21_decimal *dest) {
  dest->bits[0] = value.bits[0];
  dest->bits[1] = value.bits[1];
  dest->bits[2] = value.bits[2];
  dest->bits[3] = 0;

  return 0;
}

int copy_to_buffer(s21_decimal value, s21_decimal *dest) {
  dest->bits[0] = value.bits[0];
  dest->bits[1] = value.bits[1];
  dest->bits[2] = value.bits[2];
  dest->bits[3] = value.bits[3];

  return 0;
}

int get_sign(s21_decimal value) { return (value.bits[3] & 0x80000000) >> 31; }

int set_sign(s21_decimal *value) {
  value->bits[3] = value->bits[3] | (1UL << 31);
  return 0;
}

int get_exp(s21_decimal value) { return (value.bits[3] & 0x00FF0000) >> 16; }

int set_exp(s21_decimal *value, int exp) {
  value->bits[3] &= (1UL << 31);
  value->bits[3] = (value->bits[3] | (exp << 16));
  return 0;
}

int left_shift(s21_decimal *value, int n) {
  s21_decimal result;
  int first_bit = check_bit(*value, 31, 2);
  if (first_bit) return first_bit;

  int last_bit = 0;
  int bit;
  while (n > 0) {
    clear_decimal(&result);
    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < 32; i++) {
        bit = check_bit(*value, i, j);
        if (last_bit == 1) {
          set_bit(&result, i, j);
        }
        last_bit = bit;
      }
    }
    copy_to_buffer(result, value);
    n--;
  }

  return last_bit;
}

int right_shift(s21_decimal *value, int n) {
  s21_decimal result;

  int last_bit = 0;
  int bit;
  while (n > 0) {
    clear_decimal(&result);
    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < 32; i++) {
        bit = check_bit(*value, 31 - i, 2 - j);
        if (last_bit == 1) {
          set_bit(&result, 31 - i, 2 - j);
        }
        last_bit = bit;
      }
    }
    copy_to_buffer(result, value);
    n--;
  }

  return last_bit;
}

int decimal_pow(s21_decimal *value, int n) {
  s21_decimal result;
  s21_decimal ten;
  int status = 0;

  s21_from_int_to_decimal(10, &ten);
  clear_decimal(&result);
  for (int i = 0; i < n; i++) {
    status = s21_mul(*value, ten, &result);
    if (status > 0) break;
    copy_to_buffer(result, value);
  }

  return status;
}

int addition(int bit1, int bit2, int *remainder) {
  int result = bit1 + bit2 + (*remainder);

  if (result == 1) {
    *remainder = 0;
    result = 1;
  } else if (result == 2) {
    *remainder = 1;
    result = 0;
  } else if (result == 3) {
    *remainder = 1;
    result = 1;
  } else {
    *remainder = 0;
    result = 0;
  }

  return result;
}

int subtraction(int bit1, int bit2, int *remainder) {
  int result = bit1 - bit2 - (*remainder);

  if (result == 1) {
    *remainder = 0;
    result = 1;
  } else if (result == -2) {
    *remainder = 1;
    result = 0;
  } else if (result == -1) {
    *remainder = 1;
    result = 1;
  } else {
    *remainder = 0;
    result = 0;
  }

  return result;
}

int invert_decimal(s21_decimal *value) {
  s21_decimal result;
  clear_decimal(&result);
  int bit;

  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 32; i++) {
      bit = check_bit(*value, i, j);
      if (bit == 0) {
        set_bit(&result, i, j);
      }
    }
  }

  value->bits[0] = result.bits[0];
  value->bits[1] = result.bits[1];
  value->bits[2] = result.bits[2];

  return 0;
}

void dec_to_big_dec(s21_decimal src, s21_big_decimal *dst) {
  dst->bits[0] = src.bits[0];
  dst->bits[1] = src.bits[1];
  dst->bits[2] = src.bits[2];
  dst->bits[3] = src.bits[3];
}

void big_dec_to_dec(s21_big_decimal src, s21_decimal *dst) {
  dst->bits[0] = src.bits[0];
  dst->bits[1] = src.bits[1];
  dst->bits[2] = src.bits[2];
  dst->bits[3] = src.bits[3];
}

int div10(s21_decimal *value) {
  long int temp1, temp2;
  s21_big_decimal temp;
  dec_to_big_dec(*value, &temp);

  temp2 = (temp.bits[2] % 10);
  temp.bits[2] /= 10;
  temp2 <<= 32;

  temp.bits[1] += temp2;
  temp1 = (temp.bits[1] % 10);
  temp.bits[1] /= 10;
  temp1 <<= 32;

  temp.bits[0] += temp1;
  temp.bits[0] /= 10;

  big_dec_to_dec(temp, value);

  return 0;
}

s21_decimal mod10(s21_decimal value, int n) {
  long int temp1, temp2;
  s21_decimal result = {0}, digit;
  s21_big_decimal temp;
  dec_to_big_dec(value, &temp);

  for (int i = 0; i < n; i++) {
    temp2 = (temp.bits[2] % 10);
    temp.bits[2] /= 10;
    temp2 <<= 32;

    temp.bits[1] += temp2;
    temp1 = (temp.bits[1] % 10);
    temp.bits[1] /= 10;
    temp1 <<= 32;

    temp.bits[0] += temp1;
    temp2 = (temp.bits[0] % 10);
    s21_from_int_to_decimal(temp2, &digit);
    temp.bits[0] /= 10;

    decimal_pow(&digit, i);
    s21_add(result, digit, &result);
  }

  big_dec_to_dec(temp, &value);

  return result;
}

void bank_round(s21_decimal *value) {
  s21_decimal lastDigit = mod10(*value, 1), fiveDecimal = {{5, 0, 0, 0}},
              oneDecimal = {{1, 0, 0, 0}};
  int exp = get_exp(*value);
  set_exp(value, 0);

  div10(value);
  if (s21_is_greater(lastDigit, fiveDecimal))
    s21_add(*value, oneDecimal, value);
  else if (s21_is_equal(lastDigit, fiveDecimal) && check_bit(*value, 0, 0))
    s21_add(*value, oneDecimal, value);

  set_exp(value, exp);
}