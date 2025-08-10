cmake -S . -B build -G "Visual Studio 17 2022" ^
      -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ^
      -DPROENGINE_ENABLE_EDITOR=ON