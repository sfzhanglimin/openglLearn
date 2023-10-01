cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)

set(PROJECT_NAME "LeanOpenGL")

#设置c++17支持(为了使用filesystem)
set(CMAKE_CXX_STANDARD 17)
#设定位c++17为必须的
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${PROJECT_NAME})


#生成一个可执行文件${PROJECT_NAME}
add_executable(${PROJECT_NAME} src/main.cpp src/glad.c)

#link全局引入linb
link_directories(${PROJECT_NAME} PUBLIC libs)
#添加全局查询路径 includes
include_directories(${PROJECT_NAME} PUBLIC includes)
#添加全局查询路径 common
include_directories(${PROJECT_NAME} PUBLIC src/common)


#引入second项目
add_subdirectory(src/second)
add_subdirectory(src/third)
add_subdirectory(src/fourth)


#给${PROJECT_NAME}添加一个lib查询目录
#target_link_directories(${PROJECT_NAME} PUBLIC libs)

#给${PROJECT_NAME}添加一个头文件查询目录
#target_include_directories(${PROJECT_NAME} PUBLIC includes)

#给${PROJECT_NAME}查询common目录
#target_include_directories(${PROJECT_NAME} PUBLIC src/common)


#链接一个glfw3.lib
target_link_libraries(${PROJECT_NAME} PUBLIC glfw3)
#链接一个opengl32.lib
target_link_libraries(${PROJECT_NAME} PUBLIC opengl32)

