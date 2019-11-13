#include "keygen.h"

#include <bsd/stdlib.h> 

int* generate_ecb_key(int key_size) {
	void *ret = (void *) malloc(key_size / 8);
	arc4random_buf(ret, key_size / 8);
	return (int *) ret;
}