##  一个基于OpenCV的图片显示Demo



## 一、项目功能

实现图片的读取和显示，等待按键退出

## 二、环境依赖

操作系统：Ubuntu24.02
OpenCV版本：4.13.0
Cmake:3.28.3
编译器：G++


## 三、实验代码

#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main()
{
	Mat img = imread("agi.jpg");
	namedWindow("Picture", WINDOW_NORMAL);
	imshow("Picture", img);
	waitKey(0);
	return 0;
}

cmake_minimum_required(VERSION 3.10)
project(MyOpenCVTest)
set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS})


## 四、项目文件
build
main.cpp          主程序
CMakeLists.txt    CMake 编译配置
agi.jpg           测试图片

## 五、编译方法

cd 项目路径
mkdir build 
cd build 
cmake ..
make
运行：./main

## 六、重要说明

1. 请将“agi.jpg”放在build文件夹内，否则程序无法找到图片。
2. 确保系统已正确配置 OpenCV 环境。

## 七、常见问题

1. 无法打开图片：检查agi.jpg是否在build文件夹中
2. 编译失败：检查OpenCV路径是否正确
3. 显示失败：确保图片路径无误，OpenCV正常安装
