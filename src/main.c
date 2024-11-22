#include<stdio.h>
#include<math.h>
#include "rsa.h"

int main (int argc, char *argv[]){
  encryption encryption = rsa_encrypt(404);
  printf("encrypted message: %lu \ne: %lu \nn: %lu \nd: %lu\n",
      encryption.msg, encryption.e, encryption.n, encryption.d);
  printf("decrypted message: %lu", exponentiate(encryption.msg, encryption.d, encryption.n));
  return 0;
}
