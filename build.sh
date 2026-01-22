mkdir -p build
cd build
# cmake ..
cmake -DCMAKE_BUILD_TYPE=Debug ..
# rm -rf CMakeCache.txt CMakeFiles/
make
./OpenGL
cd ..