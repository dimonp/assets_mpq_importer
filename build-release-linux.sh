cmake -B ./out/build/unixlike-gcc -S . \
    -G "Ninja Multi-Config" \
    -DENABLE_CLANG_TIDY_DEFAULT=OFF \
    -Dassets_mpq_importer_ENABLE_IPO=ON \
    -Dassets_mpq_importer_PACKAGING_MAINTAINER_MODE=ON \
    -Dassets_mpq_importer_ENABLE_COVERAGE=OFF \
    -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=./cmake/conan_provider.cmake

cmake --build ./out/build/unixlike-gcc  --config Release

cd ./out/build/unixlike-gcc
cpack -G TGZ
