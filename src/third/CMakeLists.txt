cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)

set(PROJECT_NAME "Third")

project(${PROJECT_NAME})

#生成一个可执行文件${PROJECT_NAME}
add_executable(${PROJECT_NAME} main.cpp ../glad.c)


#给${PROJECT_NAME}添加一个lib查询目录
target_link_directories(${PROJECT_NAME} PUBLIC ../../libs)

#给${PROJECT_NAME}添加一个头文件查询目录
target_include_directories(${PROJECT_NAME} PUBLIC ../../includes)

#链接一个glfw3.lib
target_link_libraries(${PROJECT_NAME} PUBLIC glfw3)

#链接一个opengl32.lib
target_link_libraries(${PROJECT_NAME} PUBLIC opengl32)


