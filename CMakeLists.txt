cmake_minimum_required(VERSION 3.15.0)
project(turing)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -Ofast)


include_directories(${PROJECT_SOURCE_DIR}/turing-project)

aux_source_directory(${PROJECT_SOURCE_DIR}/turing-project CPP_SRC)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(turing ${CPP_SRC})

