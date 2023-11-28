cmake_minimum_required(VERSION 3.1...3.27)

project(
    GameEngineGL
    VERSION 1.0
    LANGUAGES CXX
)

# search all .cpp files with src recursively
file(GLOB_RECURSE SRCS "src/*")

# build a executable target
add_executable(${PROJECT_NAME} ${SRCS})

# add include path
include_directories("include")

# link lib path
link_directories("lib")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# add compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall) 

