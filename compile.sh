#!/usr/bin/env bash

#CLANG=$(find /usr /home ~/ -type f -name clang 2>/dev/null)
#if [[ "$CLANG" == "" ]]; then
#  echo "can't find clang compiler! please, instatll it!"
#  exit 1
#fi
#
#CMAKE=$(find /usr /home ~/ -type f -name cmake 2>/dev/null)
#if [[ "$CMAKE" == "" ]]; then
#  echo "can't find cmake! please, instatll it!"
#  exit 1
#fi

# pinned submodule commits (not --remote: upstream master may not build)
if [[ ! -f "libft/Makefile" || ! -f "lib/jansson/CMakeLists.txt" ]]; then
  git submodule update --init
fi
# -fcommon: libft/ft_printf define the global `g_pf` in a header; modern clang
# defaults to -fno-common and the final link fails with a duplicate symbol.
cd libft || echo "can't find libft directory!"
make COMP="clang -Wall -Wextra -Werror -O2 -fPIC -fcommon -I ./includes" \
  FLAGS="-Wall -Wextra -Werror -O2 -fPIC -fcommon"
cd ..

# CMAKE_POLICY_VERSION_MINIMUM: jansson requires CMake 3.1, which CMake >= 4.0
# (current Homebrew) refuses to configure without this flag.
if [[ ! -f "lib/jansson/build/lib/libjansson.a" ]]; then
  cmake -S lib/jansson -B lib/jansson/build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
    -DJANSSON_BUILD_DOCS=OFF \
    -DJANSSON_EXAMPLES=OFF \
    -DJANSSON_WITHOUT_TESTS=ON
  cmake --build lib/jansson/build --target jansson -j 4
fi

#cdmkdir build
#cd build || echo "can't make build directory!"
#cmake -D CMAKE_C_COMPILER="$CLANG" -DCMAKE_BUILD_TYPE=Release ..
#make RT -j 4
#mv RT ..
