#include <stdint.h>
#include <cstdio>
#include "bloom.h"

//# a * 16777619 mod 2**32
int32_t fnv_multiply(int32_t a) {
    return a + (a << 1) + (a << 4) + (a << 7) + (a << 8) + (a << 24);
}


// See https://web.archive.org/web/20131019013225/http://home.comcast.net/~bretm/hash/6.html
int32_t fnv_mix(int32_t a) {
    a += a << 13;
    a ^= a >> 7;
    a ^= a >> 7;
    a += a << 3;
    a ^= a >> 17;
    a ^= a >> 17;
    a += a << 5;
    a = a & 0xffffffff;
    return a;
}


// Fowler/Noll/Vo hashing.
// Nonstandard variation: this function optionally takes a seed value that is incorporated
// into the offset basis. According to http://www.isthe.com/chongo/tech/comp/fnv/index.html
// "almost any offset_basis will serve so long as it is non-zero".
int32_t fnv_1a(const char *v, int32_t len, int32_t seed) {
    int32_t a = (int32_t) 2166136261 ^seed;

    for (int i = 0; i < len; i++) {
        char c = v[i];
        a = fnv_multiply(a ^ c & 0xff);
    }

    return fnv_mix(a);
}


void calculate_locations(uint32_t *locations, int num_hashes, int m, char *key, int len) {
    int32_t a = fnv_1a(key, len, 0);
    int32_t b = fnv_1a(key, len, 1576284489); // The seed value is chosen randomly

    int32_t x = a % m;

    for (int i = 0; i < num_hashes; i++) {
        int32_t val = x < 0 ? (x + m) : x;
        locations[i] = (uint32_t) val;
        x = (x + b) % m;
    }
}


bool bloom_test(const uint32_t *locations, int num_hashes, const int32_t *buckets) {
    for (int i = 0; i < num_hashes; i++) {
        uint32_t b = locations[i];

        if ((buckets[b / 32] & (1 << (b % 32))) == 0) {
            return false;
        }
    }

    return true;
}


void bloom_add(const uint32_t *locations, int num_hashes, int32_t *buckets) {
    for (int i = 0; i < num_hashes; i++) {
        uint32_t b = locations[i];
        buckets[b / 32] |= 1 << (b % 32);
    }
}


void bloom_union(int32_t *buckets1, const int32_t *buckets2, int n) {
    for (int i = 0; i < n; i++) {
        buckets1[i] = buckets1[i] | buckets2[i];
    }
}