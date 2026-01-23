TARGET=${1:-OpenGL}  # Default to OpenGL if no argument provided

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
./$TARGET
cd ..