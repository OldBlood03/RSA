#include<stdio.h>
#include<math.h>
#include "rsa.h"

int main (int argc, char *argv[]){
  unsigned long divisor = gcd(18, 22);
  printf("gcd: %lu", divisor);
  return 0;
}
