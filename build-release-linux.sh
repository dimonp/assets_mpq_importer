cmake -B ./out/build/unixlike-gcc -S . \
    -G "Ninja Multi-Config" \
    -DENABLE_CLANG_TIDY_DEFAULT=OFF \
    -Dassets_mpq_importer_ENABLE_IPO=ON \
    -Dassets_mpq_importer_PACKAGING_MAINTAINER_MODE=ON \
    -Dassets_mpq_importer_ENABLE_COVERAGE=OFF \
    -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"

cmake --build ./out/build/unixlike-gcc  --config Release

cd ./out/build/unixlike-gcc
cpack -G TGZ 
#-DCPACK_PACKAGE_FILE_NAME="assets_mpq_importer-linux"
