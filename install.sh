mkdir build
cd build
cmake ..
make
./test/testrun
cd ..
cp build/bloom/libbloom.so pyfvnbloom/
nosetests pytest/bloom_tests.py 
python setup.py install

