set -e
BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
    echo "build directory does not exist! Creating one..."
    mkdir ${BUILD_DIR}
    echo "build directory has been created!"  
fi

if [[ "$(uname)" == "Linux" ]] || [[ -n "${GITHUB_ACTIONS}" ]]; then
    CXX=g++
    CC=gcc
else
    CXX=clang++
    CC=clang
fi

cmake -G Ninja -S . -B ${BUILD_DIR} \
    -DCMAKE_CXX_COMPILER=${CXX} \
    -DCMAKE_C_COMPILER=${CC} \
    -DCMAKE_BUILD_TYPE=Debug \
    -DPROENGINE_ENABLE_EDITOR=ON

cd ${BUILD_DIR}

ninja -j 0
echo "Build complete!"
