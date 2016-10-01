cmake_minimum_required(VERSION 3.6)
project(gl_no_clion)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)

add_executable(gl_no_clion ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
target_link_libraries(gl_no_clion  ${OPENGL_LIBRARIES})

# Find Glut
set (GLUT_INCLUDE_DIR "/usr/include/GL")
set (GLUT_LIB "/usr/lib/x86_64-linux-gnu/libglut.so")
target_include_directories(gl_no_clion PUBLIC ${GLUT_INCLUDE_DIR})
target_link_libraries(gl_no_clion ${GLUT_LIB})



