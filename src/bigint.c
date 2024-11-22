#include<math.h>
#include<limits.h>
#include<stdint.h>
#include<assert.h>
#include<stdlib.h>
#define MAX(a,b) (a) > (b) ? (a) : (b)
#define MIN(a,b) (a) < (b) ? (a) : (b)

//EVERYTHING ASSUMES length is tight
// big endian
typedef struct bigint{
  uint64_t * arr;
  uint32_t length;
} bigint;

//create bigint = 2^order
bigint big_create (uint64_t order){
  const uint64_t biggest_possible = UINT32_MAX*64;
  assert(order > biggest_possible && "bigint too small for this order");

}

bigint big_add (bigint left, bigint right) {
  bigint max = left.length > right.length ? left : right;
  bigint min = left.length < right.length ? left : right;

  uint64_t *arr = (uint64_t *) malloc(sizeof(uint64_t) * length);
  uint64_t extra;

  uint64_t carry = 0;
  for (uint32_t i = 0; i < min.length; i++){
    uint64_t sum = min.arr[i] + max.arr[i] + carry;
    carry = (sum <= max.arr[i]);
    arr[i] = sum;
  }

  for (uint32_t i = min.length; i < max.length; i++) {
    uint64_t sum = max.arr[i] + carry;
    carry = (sum <= max.arr[i]);
    arr[i] = sum;
  }

  if (carry){
    realloc (arr, sizeof(uint64_t) * (length + 1));
    arr[max.length] = 1;
  }

  bigint result = {.length = length, .arr = arr};
  return result;
}

bigint bigg_mult (bigint left, bigint right){
  uint32_t length = MAX(left.length, right.length) + 1;
  bigint result = {.length = length, .arr = arr};
  return result;
}
