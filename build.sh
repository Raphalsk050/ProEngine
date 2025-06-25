set -e
BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
    echo "build directory does not exist! Creating one..."
    mkdir ${BUILD_DIR}
    echo "build directory has been created!"  
fi

cmake -G Ninja -S . -B ${BUILD_DIR} \
    -DCMAKE_CXX_COMPILER=g++ \
    -DCMAKE_C_COMPILER=gcc \
    -DCMAKE_BUILD_TYPE=Debug \
    -DPROENGINE_ENABLE_EDITOR=ON

cd ${BUILD_DIR}

ninja -j 0
echo "Build complete!"
