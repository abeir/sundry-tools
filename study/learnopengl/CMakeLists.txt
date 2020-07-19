cmake_minimum_required(VERSION 3.16)
project(learnopengl)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_DIR)

#link_directories(/home/ubuntu/ch2/useHello)  # 动态库路径

add_executable(${PROJECT_NAME} ${SRC_DIR} main.cpp)

target_link_libraries(${PROJECT_NAME} glfw dl)