project(cpptdigest-tests)

enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -g -fprofile-arcs -ftest-coverage -DNDEBUG")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -fprofile-arcs -ftest-coverage -DNDEBUG ")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O0 -g -fprofile-arcs -ftest-coverage -DNDEBUG")

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable (AvlTreeTest avltree.cpp)

target_link_libraries (AvlTreeTest
    tdigest
    ${GTEST_BOTH_LIBRARIES}
    pthread
)

add_executable (TDigestTest tdigest_test.cpp)

target_link_libraries (TDigestTest
    tdigest
    ${GTEST_BOTH_LIBRARIES}
    pthread
)

add_test(TestAvlTree 
  AvlTreeTest
  TDigestTest)
