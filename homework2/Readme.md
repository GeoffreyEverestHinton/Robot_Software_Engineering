# 基于Socket和多线程编成的双向聊天程序

## 一、项目简介

本项目为机器人软件工程课程第二次作业，采用C++语言实现基于TCP协议的客户端/服务器（C/S）架构聊天系统。通过多线程并发设计，实现消息发送与接收的异步执行，支持服务端与客户端双向实时通信，具备完整的编译构建流程和优雅退出机制。


## 二、项目结构



```
homework2/

├── server.cpp        # 服务端核心代码

├── client.cpp        # 客户端核心代码

├── CMakeLists.txt    # 跨平台构建配置文件

└── README.md         # 项目说明文档
```

## 三、核心实现细节

### 1.多线程设计

服务端与客户端均采用 双线程分离模型，通过C++11`std::thread`实现：


* 接收线程（recv\_msg）：循环监听网络套接字，实时读取对端消息并打印

* 发送线程（send\_msg）：循环监听标准输入，读取键盘输入并发送至对端

#### （1）核心代码片段


```
// 服务端线程创建（server.cpp）

thread t1(recv\_msg, client\_socket);  // 接收消息线程

thread t2(send\_msg, client\_socket);  // 发送消息线程

t1.join();  // 等待线程执行完成

t2.join();

// 客户端线程创建（client.cpp）

thread t1(recv\_msg, sock);  // 接收消息线程

thread t2(send\_msg, sock);  // 发送消息线程

t1.join();

t2.join();
```

### 2.TCP Socket通信流程

#### （1）服务端流程



1. 创建套接字（`socket()`）

2. 绑定端口与 IP（`bind()`）

3. 监听连接请求（`listen()`）

4. 接受客户端连接（`accept()`）

5. 启动双线程处理收发逻辑

6. 关闭套接字（`close()`）

#### （2）客户端流程



1. 创建套接字（`socket()`）

2. 连接服务端（`connect()`）

3. 启动双线程处理收发逻辑

4. 关闭套接字（`close()`）

#### （3）核心代码片段



```
// 服务端 Socket 初始化（server.cpp）

int server\_fd = socket(AF\_INET, SOCK\_STREAM, 0);  // 创建TCP套接字

sockaddr\_in addr{};

addr.sin\_family = AF\_INET;         // IPv4协议

addr.sin\_port = htons(9999);       // 绑定9999端口（网络字节序）

addr.sin\_addr.s\_addr = INADDR\_ANY; // 监听所有本地IP

bind(server\_fd, (sockaddr \*)\&addr, sizeof(addr));  // 绑定

listen(server\_fd, 5);             // 最大等待队列长度5

int client\_socket = accept(server\_fd, nullptr, nullptr);  // 接受连接

// 客户端 Socket 初始化（client.cpp）

int sock = socket(AF\_INET, SOCK\_STREAM, 0);

sockaddr\_in addr{};

addr.sin\_family = AF\_INET;

addr.sin\_port = htons(9999);

inet\_pton(AF\_INET, "127.0.0.1", \&addr.sin\_addr);  // 连接本地服务端

connect(sock, (sockaddr \*)\&addr, sizeof(addr));   // 发起连接
```

### 3.CMake 构建配置（CMakeLists.txt）



```
//最低CMake版本要求

cmake\_minimum\_required(VERSION 3.10)

//项目名称

project(ChatSystem)

//指定C++标准（C++11支持std::thread）

set(CMAKE\_CXX\_STANDARD 11)

set(CMAKE\_CXX\_STANDARD\_REQUIRED ON)

//生成可执行文件

add\_executable(server server.cpp)  # 服务端可执行文件

add\_executable(client client.cpp)  # 客户端可执行文件

//链接线程库（Linux下多线程需链接pthread）

target\_link\_libraries(server pthread)

target\_link\_libraries(client pthread)
```

## 四、编译与运行步骤

### 1.前置依赖



* 操作系统：Ubuntu 24.04（或其他 Linux 发行版）
* cmake:3.28.3
* 编译工具：g++
* 依赖库：pthread

### 2.编译命令



```
//进入作业目录

cd homework2

//创建构建目录（推荐，避免污染源码）

mkdir build && cd build

//生成Makefile

cmake ..

make 
```

### 3.运行步骤



#### （1）启动服务端（终端 1）：



```
cd homework2/build

./server
```

输出提示：`等待客户端连接...`（表示服务端已就绪）



#### （2）启动客户端（终端 2）：



```
cd homework2/build

./client
```

输出提示：`已连接服务器`（表示连接成功）



#### （3）开始聊天：

* 服务端与客户端可互相发送文字消息

* 输入 `exit` 并回车，可关闭当前程序并断开连接

## 五、注意事项



1. 运行时需保证服务端先启动，再启动客户端

2. 端口号固定为 9999，若端口被占用，可修改代码中 `htons(9999)` 为其他未占用端口（如 8888）



