echo "-- Installing CMake"
DEPS_DIR="${TRAVIS_BUILD_DIR}/deps"
mkdir -p ${DEPS_DIR} && cd ${DEPS_DIR}
if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
      CMAKE_URL="https://cmake.org/files/v3.12/cmake-3.12.2-Linux-x86_64.tar.gz"
      mkdir cmake && travis_retry wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C cmake
      export PATH=${DEPS_DIR}/cmake/bin:${PATH}
fi
cd ${TRAVIS_BUILD_DIR}/
echo "-- Done installing CMake"
