mkdir -p build
cd build
cmake ..
# rm -rf CMakeCache.txt CMakeFiles/
make
cd build
./OpenGL
cd ..