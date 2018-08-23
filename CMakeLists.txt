cmake_minimum_required(VERSION 2.6)

project(Funcgen)

SET(EXTRA_CXX_COMPILE_FLAGS "-std=c++11 -I./src -I./test -I./benchmarks -I/opt/local/include -O2 -Werror -Wall -fPIC")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_COMPILE_FLAGS}")

SET(CXX_OCL_LINK_FLAGS "-L/Users/dillon/CppWorkspace/coreir/lib/ -Lrelease/lib")
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${CXX_OCL_LINK_FLAGS}")


INCLUDE_DIRECTORIES(./src/)

SET(CPP_FILES ./src/language.cpp
              ./src/simulator.cpp)

add_library(funcgen ${CPP_FILES})

# Test executables
SET(ALL_TEST_FILES ./test/language_tests.cpp)

add_executable(all-tests test/test_main.cpp ${ALL_TEST_FILES})
target_link_libraries(all-tests funcgen)
