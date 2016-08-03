
add_library (tdigest 
    avltree.cpp
    tdigest.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -g -fprofile-arcs -ftest-coverage -DNDEBUG")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -fprofile-arcs -ftest-coverage -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O0 -g -fprofile-arcs -ftest-coverage -DNDEBUG")

add_executable (demo main.cpp)
target_link_libraries (demo tdigest)
set(CMAKE_BUILD_TYPE Debug)
