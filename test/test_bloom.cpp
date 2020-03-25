#include <iostream>
#include <cstdlib>
#include <vector>
#include "bloom.h"
#include "gtest/gtest.h"

using namespace std;


TEST(fnv_multiply, fnv_multiply_test1) {
    int32_t actual = fnv_multiply(1);
    int32_t expected = 16777619;
    EXPECT_EQ(actual, expected);
}

TEST(fnv_multiply, fnv_multiply_test2) {
    int32_t actual = fnv_multiply(-2128831068);
    int32_t expected = -4763932372;
    EXPECT_EQ(actual, expected);
}

TEST(fnv_mix, fnv_mix_test1) {
    int32_t actual = fnv_mix(10);
    int32_t expected = 24334695;
    EXPECT_EQ(actual, expected);
}

TEST(fnv_mix, fnv_mix_test2) {
    int32_t actual = fnv_mix(-2128831068);
    int32_t expected = -651461121;
    EXPECT_EQ(actual, expected);
}

TEST(fnv_1a, fnv_1a_test1) {
    char *v = "a";
    int32_t actual = fnv_1a(v, 1, 0);
    int32_t expected = -146810011;
    EXPECT_EQ(actual, expected);
}

TEST(fnv_1a, fnv_1a_test2) {
    char *v = "abcd";
    int32_t actual = fnv_1a(v, 4, 0);
    int32_t expected = 1676827413;
    EXPECT_EQ(actual, expected);
}

TEST(fnv_1a, fnv_1a_test3) {
    char *v = "abcd";
    int32_t actual = fnv_1a(v, 4, 1576284489);
    int32_t expected = -1628957343;
    EXPECT_EQ(actual, expected);
}

TEST(calculate_locations, calculate_locations_test1) {
    char *key = "abcd";
    int len = 4;
    uint32_t locations[7] = {0, 0, 0, 0, 0, 0, 0};
    int num_hashes = 7;
    int m = 9600;

    calculate_locations(locations, num_hashes, m, key, len);

    uint32_t expected[7] = { 5013, 4470, 3927, 3384, 2841, 2298, 1755};

    for (int i = 0; i < num_hashes; i++) {
        EXPECT_EQ(locations[i], expected[i]);
    }
}

TEST(bf_add, bf_add_test1) {
    int num_hashes = 7;
    uint32_t locations[7] = { 5013, 4470, 3927, 3384, 2841, 2298, 1755 };
    int n = 300;
    vector<int32_t> buckets(n, 0);

    bloom_add(locations, num_hashes, &buckets[0]);

    EXPECT_EQ(buckets[54], 134217728);
    EXPECT_EQ(buckets[71], 67108864);    
    EXPECT_EQ(buckets[88], 33554432);
    EXPECT_EQ(buckets[105], 16777216);
    EXPECT_EQ(buckets[122], 8388608);
    EXPECT_EQ(buckets[139], 4194304);
    EXPECT_EQ(buckets[156], 2097152);
}

TEST(bf_test, bf_test_test1) {
    int num_hashes = 7;
    uint32_t locations[7] = { 5013, 4470, 3927, 3384, 2841, 2298, 1755 };
    int n = 300;
    vector<int32_t> buckets(n, 0);

    bloom_add(locations, num_hashes, &buckets[0]);

    bool actual = bloom_test(locations, num_hashes, &buckets[0]);
    bool expected = true;
    EXPECT_EQ(actual, expected);
}