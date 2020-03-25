# LIBBLOOM-Python

Fast bloom filter implementation with Python bindings 

The implementation is based on [bloomfilter.js](https://github.com/jasondavies/bloomfilter.js) - and it's (almost) a direct translaslation form JavaScript to C.


## Building and installing

### CMake 
To build the binaries, you need to have CMake. If you don't have it:

```
mkdir cmake && cd cmake
wget https://cmake.org/files/v3.10/cmake-3.10.0-Linux-x86_64.sh
bash cmake-3.10.0-Linux-x86_64.sh --skip-license
export CMAKE_HOME=`pwd`
export PATH=$PATH:$CMAKE_HOME/bin
```


### Bulding it

To build the binaries:

```
mkdir build && cd build
cmake ..
make
```

Run some tests:

```
./test/testrun
```

If everything is fine, copy them:

```
cd ..
cp build/bloom/libbloom.so pyfvnbloom/
```

Now let's test it:

```
nosetests pytest/bloom_tests.py 
```

If everything is fine, you can install it:

```
python setup.py install
```


### Wheel 

You can build a wheel and then use it for installing the package instead of building it from the sources

```
python setup.py sdist bdist_wheel
```

The result will be in the `dist` folder:

```
dist/pyfvnbloom-0.0.1-cp36-cp36m-linux_x86_64.whl
```