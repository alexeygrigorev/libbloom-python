import os
import math
import json
import base64

import ctypes

import numpy as np

path = os.path.dirname(__file__)
lib_path = path + '/libbloom.so'


_lib = ctypes.cdll.LoadLibrary(lib_path)

_lib.calculate_locations.argtypes = [
    ctypes.POINTER(ctypes.c_uint32),
    ctypes.c_int32,
    ctypes.c_int32,
    ctypes.POINTER(ctypes.c_char),
    ctypes.c_int32,
]

_lib.bloom_test.restype = ctypes.c_bool
_lib.bloom_test.argtypes = [
    ctypes.POINTER(ctypes.c_uint32),
    ctypes.c_int32,
    ctypes.POINTER(ctypes.c_int32),
]

_lib.bloom_add.argtypes = [
    ctypes.POINTER(ctypes.c_uint32),
    ctypes.c_int32,
    ctypes.POINTER(ctypes.c_int32),
]

_lib.bloom_union.argtypes = [
    ctypes.POINTER(ctypes.c_int32),
    ctypes.POINTER(ctypes.c_int32),
    ctypes.c_int32,
]



def create_empty(capacity, error_rate=0.001):    
    if not (0 < error_rate < 1):
        raise ValueError("Error_Rate must be between 0 and 1.")
    if not capacity > 0:
        raise ValueError("Capacity must be > 0")

    num_bits = (-capacity * math.log(error_rate) / (math.log(2) * math.log(2)))
    num_hashes = max(1, round(num_bits / capacity * math.log(2)))
    num_hashes = int(num_hashes)

    n = int(math.ceil(num_bits / 32))
    buckets = np.zeros(n, dtype='int32')

    return BloomFilter(num_hashes, buckets)


class BloomFilter(object):
    def __init__(self, num_hashes, buckets):
        self._init_internal(num_hashes, buckets)

    def _init_internal(self, num_hashes, buckets):
        self.buckets = buckets
        self._p_buckets = self.buckets.ctypes.data_as(ctypes.POINTER(ctypes.c_int32))
        self.num_hashes = num_hashes
        self.n = len(buckets)
        self.m = self.n * 32
        self.locations = np.zeros(self.num_hashes, dtype='uint32')
        self._p_locations = self.locations.ctypes.data_as(ctypes.POINTER(ctypes.c_uint32))

    def _calculate_locations(self, key):
        _lib.calculate_locations(self._p_locations, self.num_hashes, self.m, key, len(key))
        return self._p_locations

    def _calculate_key(self, key):
        bkey = key.encode()
        return bkey

    def test(self, key):
        key = self._calculate_key(key)
        ploc = self._calculate_locations(key)
        return _lib.bloom_test(ploc, self.num_hashes, self._p_buckets)

    def __contains__(self, key):
        return self.test(key)

    def add(self, key):
        key = self._calculate_key(key)
        ploc = self._calculate_locations(key)
        _lib.bloom_add(ploc, self.num_hashes, self._p_buckets)

    def union(self, other):
        assert self.num_hashes == other.num_hashes
        assert self.n == other.n
        _lib.bloom_union(self._p_buckets, other._p_buckets, self.n)
        return self

    def save(self, file):
        save(self.num_hashes, self.buckets, file)

    def __getstate__(self):
        return (self.num_hashes, self.buckets)

    def __setstate__(self, state):
        num_hashes, buckets = state
        self._init_internal(num_hashes, buckets)


def save(num_hashes, buckets, file):
    buckets = np.array(buckets, dtype='int32')
    b64 = base64.b64encode(buckets.tobytes()).decode()

    d = dict(num_hashes=num_hashes, buckets=b64)

    with open(file, 'w') as f_out:
        json.dump(d, f_out)


def load(file):
    with open(file, 'r') as f_in:
        d = json.load(f_in)

    b64 = d['buckets']
    buckets = np.frombuffer(base64.b64decode(b64), dtype='int32')

    return BloomFilter(d['num_hashes'], buckets)