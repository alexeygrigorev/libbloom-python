#ifndef BLOOM_LIBRARY_H
#define BLOOM_LIBRARY_H

#include <stdint.h>

extern "C" {

int32_t fnv_multiply(int32_t a);
int32_t fnv_mix(int32_t a);
int32_t fnv_1a(const char *v, int32_t len, int32_t seed);

void calculate_locations(uint32_t *locations, int num_hashes, int m, char *key, int len);
bool bloom_test(const uint32_t *locations, int num_hashes, const int32_t *buckets);
void bloom_add(const uint32_t *locations, int num_hashes, int32_t *buckets);
void bloom_union(int32_t *buckets1, const int32_t *buckets2, int n);

};

#endif