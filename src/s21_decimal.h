#include "float.h"
#include "math.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#define FALSE 0
#define TRUE 1

#define S21_INT_MAX 4294967295
#define MAX_FLOAT_TO_CONVERT 79228157791897854723898736640.0f
#define MIN_FLOAT_TO_CONVERT \
  0.00000000000000000000000000010000000031710768509710513471352647538147514756461109f
#define MAX_DEC powf(2.0f, 96.0f) - 1.0
#define MIN_DEC -powf(2.0f, 96.0f) + 1.0

#define POS_INF 1.0 / 0.0
#define NEG_INF -1.0 / 0.0
#define S21_NAN 0.0 / 0.0

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  uint64_t bits[4];
} s21_big_decimal;

// Арифметические операции
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Операции сравнения
int s21_is_less(s21_decimal d1, s21_decimal d2);
int s21_is_less_or_equal(s21_decimal d1, s21_decimal d2);
int s21_is_greater(s21_decimal d1, s21_decimal d2);
int s21_is_greater_or_equal(s21_decimal d1, s21_decimal d2);
int s21_is_equal(s21_decimal d1, s21_decimal d2);
int s21_is_not_equal(s21_decimal d1, s21_decimal d2);

// Преобразователи
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Другие функции
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);

// Вспомогательные функции
int check_bit(s21_decimal value, const int pos, int num);
void set_bit(s21_decimal *value, const int pos, int num);
void unset_bit(s21_decimal *value, const int pos, int num);
int get_first_bit(s21_decimal value);
void clear_decimal(s21_decimal *dec);
int copy_to_buffer_no_exp(s21_decimal value, s21_decimal *dest);
int copy_to_buffer(s21_decimal value, s21_decimal *dest);
int get_sign(s21_decimal value);
int set_sign(s21_decimal *value);
int get_exp(s21_decimal value);
int set_exp(s21_decimal *value, int exp);
int left_shift(s21_decimal *value, int n);
int right_shift(s21_decimal *value, int n);
int decimal_pow(s21_decimal *value, int n);
int addition(int bit1, int bit2, int *remainder);
int subtraction(int bit1, int bit2, int *remainder);
int invert_decimal(s21_decimal *value);
int div10(s21_decimal *value);
s21_decimal mod10(s21_decimal value, int n);
int normalize_mantissa(s21_decimal *value_1, s21_decimal value_2);
int normalize_add(s21_decimal *value_1, s21_decimal *value_2);
void bank_round(s21_decimal *value);

// Удлиненный децимал для функции div10
void dec_to_big_dec(s21_decimal src, s21_big_decimal *dst);
void big_dec_to_dec(s21_big_decimal src, s21_decimal *dst);