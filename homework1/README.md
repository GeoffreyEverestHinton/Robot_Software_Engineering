#            一个基于OpenCV的图片显示Demo

## 一、项目功能

本项目为机器人软件工程课程第一次作业。实现图片的读取和显示。

## 二、环境依赖

- 操作系统：Ubuntu24.02
- OpenCV版本：4.13.0
- Cmake:3.28.3
- 编译器：G++

## 三、实验代码

Ubuntu安装OpenCV
```bash
# 更新软件源
sudo apt update

# 安装 OpenCV 核心库 + 开发依赖
sudo apt install libopencv-dev python3-opencv -y

# 验证是否安装成功
pkg-config --modversion opencv4
```
展示图片的C++代码
```bash
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
```
CmkeLists.txt
```bash
# 设定CMake的最低版本要求，低于3.10将无法编译本项目
cmake_minimum_required(VERSION 3.10)

# 定义项目名称为MyOpenCVTest，可根据需求自定义
project(MyOpenCVTest)

# 设置C++编译标准为C++11，强制启用该标准（避免编译器兼容问题）
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找系统中安装的OpenCV库，REQUIRED表示找不到则直接终止编译
find_package(OpenCV REQUIRED)

# 将OpenCV的头文件目录添加到项目的包含路径，让编译器能找到OpenCV头文件
include_directories(${OpenCV_INCLUDE_DIRS})

# 生成可执行文件，指定编译main.cpp生成名为main的可执行程序
add_executable(main main.cpp)

# 将OpenCV的库文件链接到可执行程序main，实现项目对OpenCV库的调用
target_link_libraries(main ${OpenCV_LIBS})

```

## 四、项目文件

| 文件名         | 功能说明               |
|:---------------|:----------------------|
| build/          | 编译产物存放目录       |
| main.cpp        | 项目主程序（核心代码） |
| CMakeLists.txt  | CMake编译配置文件      |
| agi.jpg         | 测试图片文件           |


## 五、编译方法
```bash
# 1. 进入项目根目录（请替换成你自己的项目路径）
cd 项目路径

# 2. 创建 build 文件夹（用于存放编译文件）
mkdir build

# 3. 进入 build 文件夹
cd build

# 4. CMake 生成 Makefile 编译配置文件
cmake ..

# 5. 编译项目，生成可执行文件
make

# 6. 运行程序（显示图片）
./main
```

## 六、重要说明

- 第一条 请将“agi.jpg”放在build文件夹内，否则程序无法找到图片。
- 第二条 确保系统已正确配置 OpenCV 环境。

## 七、常见问题

- 第一条 无法打开图片：检查agi.jpg是否在build文件夹中
- 第二条 编译失败：检查OpenCV路径是否正确
- 第三条 显示失败：确保图片路径无误，OpenCV正常安装
