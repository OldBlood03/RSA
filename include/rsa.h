#ifndef RSA
#define RSA
#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
#define USHORT_MAX 0xFFFFUL

typedef struct encryption{
  uint32_t n;
  uint32_t e;
  uint32_t d;
  uint32_t msg;
} encryption;

typedef struct bezout{
  uint32_t gcd;
  int64_t x;
  int64_t y;
} bezout;

uint32_t leading_zeros(uint32_t value);
int fermat_check (uint32_t p, uint32_t a);
int isPrime (uint32_t p);
uint16_t find_a_prime (uint16_t min, uint16_t max);
encryption RSAencrypt (uint32_t message);
uint32_t exponentiate (uint32_t base, uint32_t power, uint32_t mod);
bezout gcd (uint32_t a, uint32_t b);
#endif
