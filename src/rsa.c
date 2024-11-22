#include"rsa.h"


unsigned long exponentiate (unsigned long base, unsigned long power, unsigned long mod){
  unsigned long long square = base;
  unsigned long long total = 1;
  unsigned padding = leading_zeros(power);

  for (unsigned i = 0; i < 32 - padding; i++){
    unsigned long long bit = ((power >> i) & 1) * square;
    total *= bit != 0 ? bit : 1;
    total %= mod;
    square = (square * square) % mod;
  }

  return (unsigned long) total;
}

unsigned leading_zeros(unsigned long value) {
  unsigned count = 0;
  if ((value & 0xffff0000u) == 0) {
    count += 16;
    value <<= 16;
  }
  if ((value & 0xff000000u) == 0) {
    count += 8;
    value <<= 8;
  }
  if ((value & 0xf0000000u) == 0) {
    count += 4;
    value <<= 4;
  }
  if ((value & 0xc0000000u) == 0) {
    count += 2;
    value <<= 2;
  }
  if ((value & 0x80000000u) == 0) {
    count += 1;
  }
  return count;
}

unsigned trailing_zeros(unsigned long value) {
  unsigned count = 0;
  if ((value & 0x0000ffffu) == 0) {
    count += 16;
    value <<= 16;
  }
  if ((value & 0x000000ffu) == 0) {
    count += 8;
    value <<= 8;
  }
  if ((value & 0x0000000fu) == 0) {
    count += 4;
    value <<= 4;
  }
  if ((value & 0x00000003u) == 0) {
    count += 2;
    value <<= 2;
  }
  if ((value & 0x00000001u) == 0) {
    count += 1;
    value <<= 1;
  }
  return count;
}

int fermat_check (unsigned long p, unsigned long a){
  unsigned long total = exponentiate (a, p-1, p);
  return total == 1 ? 1 : 0;
}

// this is enough to check a 32 bit int for primality
int isPrime (unsigned long p){
  if (p == 2 || p == 3 || p == 5 || p == 7) { return 1; }
  if (!fermat_check (p, 2)){ return 0; }
  if (!fermat_check (p, 3)){ return 0; }
  if (!fermat_check (p, 5)){ return 0; }
  if (!fermat_check (p, 7)){ return 0; }
  return 1;
}

unsigned short find_a_prime (unsigned short min, unsigned short max){
  assert (max > min && "max must be greater than min");
  unsigned short candidate = (unsigned short) rand() % (max - min) + min;

  while (!isPrime(candidate)){
    candidate += 1;
  }

  if (candidate > max) { return 0; }
  return candidate;
}

static void swap_unsigned (unsigned long * a, unsigned long * b){
  unsigned long temp = *b;
  *b = *a;
  *a = temp;

}

static void swap_signed (long long * a, long long * b){
  unsigned long temp = *b;
  *b = *a;
  *a = temp;
}

bezout gcd (unsigned long a, unsigned long b){
  if (a == 0){
    bezout result = {.gcd = b, .x = 0, .y = !!b};
    return result; 
  }
  if (b == 0){
    bezout result = {.gcd = a, .x = 1, .y = 0};
    return result; 
  }

  int swapped = 0;
  if (a > b) {
    swapped = 1;
    swap_unsigned(&a,&b);
  }

  unsigned r = trailing_zeros(a | b);
  a >>= r;
  b >>= r;

  long long x = (long long)a;
  long long y = (long long)b;
  long long s = 1;
  long long t = 0;
  long long u = 0;
  long long v = 1;

  while (x) {
    while ((x & 1) == 0) { // a is even
      x /= 2;
      if (((s | t) & 1) == 0) {
        s /= 2;
        t /= 2;
      } else {
        s = s + (long long)b;
        t = t - (long long)a;
      }
    }
    while ((y & 1) == 0) { // b is even
      y /= 2;
      if (((u | v) & 1) == 0) {
        u /= 2;
        v /= 2;
      } else {
        u = (u + (long long)b)/2;
        v = (v - (long long)a)/2;
      }
    }
    if (x >= y) {
      x -= y;
      s -= u;
      t -= v;
    } else {
      y -= x;
      u -= s;
      v -= t;
    }
  }

  if (swapped) {
    swap_unsigned(&a,&b);
    swap_signed(&u,&v);
    swap_signed(&s, &t);
  }
  bezout result = {.gcd = (unsigned long)y << r, .x = (long long)u, .y = (long long)v};
  return result; 
}

encryption RSAencrypt (unsigned long message){
  unsigned short p = find_a_prime(USHORT_MAX/2,USHORT_MAX);
  unsigned short q = find_a_prime(USHORT_MAX/2,USHORT_MAX);

  unsigned long n = p*q;
  unsigned long phi = (p-1)*(q-1);
  unsigned long e = 3;

  bezout ed = gcd(e, phi);
  while (ed.gcd != 1){
    e += 2;
    ed = gcd(e, phi);
  }

  unsigned long d = ed.x;
  assert(d*e % phi == 1 && "d is not the modular inverse of e");
  unsigned long encrypted_msg = exponentiate(message, e, n);
  encryption encryption_bundle = {.e = e, .n = n, .d = d, .msg = encrypted_msg};
  return encryption_bundle;
}
