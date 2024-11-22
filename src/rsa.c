#include"rsa.h"

uint32_t exponentiate (uint32_t base, uint32_t power, uint32_t mod){
  uint64_t square = base;
  uint64_t total = 1;
  unsigned padding = leading_zeros(power);

  for (unsigned i = 0; i < 32 - padding; i++){
    uint64_t bit = ((power >> i) & 1) * square;
    total *= bit != 0 ? bit : 1;
    total %= mod;
    square = (square * square) % mod;
  }

  return (uint32_t) total;
}

uint32_t leading_zeros(uint32_t value) {
  uint32_t count = 0;
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

unsigned trailing_zeros(uint32_t value) {
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

int fermat_check (uint32_t p, uint32_t a){
  uint32_t total = exponentiate (a, p-1, p);
  return total == 1 ? 1 : 0;
}

// this is enough to check a 32 bit int for primality
int isPrime (uint32_t p){
  if (p == 2 || p == 3 || p == 5 || p == 7) { return 1; }
  if (!fermat_check (p, 2)){ return 0; }
  if (!fermat_check (p, 3)){ return 0; }
  if (!fermat_check (p, 5)){ return 0; }
  if (!fermat_check (p, 7)){ return 0; }
  return 1;
}

uint16_t find_a_prime (uint16_t min, uint16_t max){
  assert (max > min && "max must be greater than min");
  uint16_t candidate = (uint16_t) rand() % (max - min) + min;

  while (!isPrime(candidate)){
    candidate += 1;
  }

  if (candidate > max) { return 0; }
  return candidate;
}

static void swap_unsigned (uint32_t * a, uint32_t * b){
  uint32_t temp = *b;
  *b = *a;
  *a = temp;
}

static void swap_signed (int64_t * a, int64_t * b){
  uint32_t temp = *b;
  *b = *a;
  *a = temp;
}

bezout gcd (uint32_t a, uint32_t b){
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

  int64_t x = (int64_t)a;
  int64_t y = (int64_t)b;
  int64_t s = 1;
  int64_t t = 0;
  int64_t u = 0;
  int64_t v = 1;

  while (x) {
    while ((x & 1) == 0) { // a is even
      x /= 2;
      if (((s | t) & 1) == 0) {
        s /= 2;
        t /= 2;
      } else {
        s = s + (int64_t)b;
        t = t - (int64_t)a;
      }
    }
    while ((y & 1) == 0) { // b is even
      y /= 2;
      if (((u | v) & 1) == 0) {
        u /= 2;
        v /= 2;
      } else {
        u = (u + (int64_t)b)/2;
        v = (v - (int64_t)a)/2;
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
  bezout result = {.gcd = (uint32_t)y << r, .x = (int64_t)u, .y = (int64_t)v};
  return result; 
}

encryption rsa_encrypt (uint32_t message){
  uint16_t p = find_a_prime(USHORT_MAX/2,USHORT_MAX);
  uint16_t q = find_a_prime(USHORT_MAX/2,USHORT_MAX);

  uint32_t n = p*q;
  uint32_t phi = (p-1)*(q-1);
  uint32_t e = 3;

  bezout ed = gcd(e, phi);
  while (ed.gcd != 1){
    e += 2;
    ed = gcd(e, phi);
  }

  uint32_t d = ed.x;
  assert(d*e % phi == 1 && "d is not the modular inverse of e");
  uint32_t encrypted_msg = exponentiate(message, e, n);
  encryption encryption_bundle = {.e = e, .n = n, .d = d, .msg = encrypted_msg};
  return encryption_bundle;
}
