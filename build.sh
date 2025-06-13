BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
    echo "build directory don't exists! Creating one..."
    mkdir ${BUILD_DIR}
    echo "build directory has been created!"  
fi

cmake -G Ninja -B ${BUILD_DIR} -Wdev, --debug-output and --trace -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DCMAKE_POLICY_VERSION_MINIMUM=3.5

cd ${BUILD_DIR}

ninja -j 0
echo "Build complete!"

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi
