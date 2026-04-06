cmake_minimum_required(VERSION 3.10)

# 项目名字
project(MyOpenCVTest)
set(CMAKE_CXX_STANDARD 11)

# 寻找 OpenCV 库
find_package(OpenCV REQUIRED)

# 包含 OpenCV 头文件
include_directories(${OpenCV_INCLUDE_DIRS})

# 编译 main.cpp 生成可执行文件 main
add_executable(main main.cpp)

# 链接 OpenCV 库
target_link_libraries(main ${OpenCV_LIBS})

