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

unsigned long gcd (unsigned long a, unsigned long b){
  unsigned long bigger = MAX(a,b);
  unsigned long smaller = MIN(a,b);

  unsigned bigpad = trailing_zeros(bigger);
  unsigned smallpad = trailing_zeros(smaller);

  unsigned k = MIN(bigpad, smallpad);
  long long c = 0;
  long long d = 1;

  bigger >>= bigpad;
  smaller >>= smallpad;

  do{
    if (bigger < smaller){ 
      unsigned long temp = bigger;
      bigger = smaller;
      smaller = temp; 
      long long tempc = c;
      c = d - (bigger / smaller) * c;
      d = tempc;
    }

    bigger = bigger - smaller;
    bigpad = trailing_zeros(bigger);
    bigger >>= bigpad;
  }
  while (bigger > 0);

  return smaller << k;
}

encryption RSAencrypt (unsigned long message, unsigned long e){
  unsigned short p = find_a_prime(USHORT_MAX/2,USHORT_MAX);
  unsigned short q = find_a_prime(USHORT_MAX/2,USHORT_MAX);

  unsigned long n = p*q;
  unsigned long phi = (p-1)*(q-1);

  assert (phi % e == 0 && "e does not divide phi");

  unsigned long d = phi/e;
  unsigned long encrypted_msg = exponentiate(message, e, n);
  encryption encryption_bundle = {.e = e, .n = n, .d = d, .msg = encrypted_msg};
  return encryption_bundle;
}
