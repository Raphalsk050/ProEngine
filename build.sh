set -e
BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
    echo "build directory don't exists! Creating one..."
    mkdir ${BUILD_DIR}
    echo "build directory has been created!"  
fi

cmake -G Ninja -S . -B ${BUILD_DIR} -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DPROENGINE_ENABLE_EDITOR=ON

cd ${BUILD_DIR}

ninja -j 0
echo "Build complete!"
