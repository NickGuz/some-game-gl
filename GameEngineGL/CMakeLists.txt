cmake_minimum_required(VERSION 3.1...3.27)

project(
    GameEngineGL
    VERSION 1.0
    LANGUAGES CXX
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

# add include path
# include_directories("include")
# target_include_directories(${PROJECT_NAME} INTERFACE 
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/glad"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/GLFW"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/glm"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/glm/detail"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/glm/ext"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/glm/gtc"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/glm/gtx"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/glm/simd"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/KHR"
# )
target_include_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

# link lib path
# link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/vulkan")
target_link_libraries(${PROJECT_NAME} PRIVATE glfw 
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib" 
    #"${Vulkan_LIBRARIES}"
    # "${VLK}"
    opengl32.lib)

# set c++ standard
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# add compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall) 

