cmake_minimum_required(VERSION 3.1...3.27)

set(CMAKE_BUILD_TYPE Debug)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

project(
    GameEngineGL
    VERSION 1.0
    LANGUAGES CXX C
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)

# add source files
file(GLOB_RECURSE SRCS "src/*.cpp" "src/*.c")

# add headers
file(GLOB_RECURSE HDRS "src/*.h" "src/*.hpp")

# add vulkan libs
# file(GLOB_RECURSE VLK "lib/vulkan/*")

# build a executable target
add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} "src/glad.c" "include/glad/glad.h")

#find_package(Vulkan REQUIRED)
# target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})

find_package(SDL2 REQUIRED)
find_package(OpenAL REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${SDL2_INCLUDE_DIRS}"
    "${OPENAL_INCLUDE_DIR}")

# link lib path
# link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/vulkan")
if (LINUX)
    find_library(LIB_IRRKLANG 
        NAMES libIrrKlang irrklang irrKlang IrrKlang libirrklang libirrKlang
        HINTS "${CMAKE_CURRENT_SOURCE_DIR}/lib")

    target_link_libraries(${PROJECT_NAME} PRIVATE glfw
        "${LIB_IRRKLANG}"
        "${SDL2_LIBRARIES}"
        "${OPENAL_LIBRARY}"
        #"${CMAKE_CURRENT_SOURCE_DIR}/lib/libglfw3.a"
        #"${CMAKE_CURRENT_SOURCE_DIR}/lib/libfreetype.a")
         )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw 
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib" 
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/irrKlang.lib"
        "${SDL2_LIBRARIES}"
        "${OPENAL_LIBRARY}"
        #"${Vulkan_LIBRARIES}"
        # "${VLK}"
        opengl32.lib)
endif()

# set c++ standard
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# add compiler flags
if(LINUX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall) 
endif()

