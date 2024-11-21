#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<math.h>
#define USHORT_MAX 0xFFFFUL
#define MAX(a,b) (a) > (b) ? (a) : (b)
#define MIN(a,b) (a) < (b) ? (a) : (b)

typedef struct encryption{
  unsigned long n;
  unsigned long e;
  unsigned long d;
  unsigned long msg;
} encryption;

unsigned leading_zeros(unsigned long value);
int fermat_check (unsigned long p, unsigned long a);
int isPrime (unsigned long p);
unsigned short find_a_prime (unsigned short min, unsigned short max);
encryption RSAencrypt (unsigned long message, unsigned long e);
unsigned long exponentiate (unsigned long base, unsigned long power, unsigned long mod);
unsigned long gcd (unsigned long a, unsigned long b);
