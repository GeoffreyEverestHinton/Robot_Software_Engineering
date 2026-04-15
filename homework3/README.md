# 基于ROS2的跨语言话题通信过程



---

# 一、项目简介
本项目是《机器人软件工程学》第三次作业。基于ROS2实现C++节点定时发布消息，Python节点订阅并打印消息的通信功能。
# 二、项目结构
```
homework3/                # 功能包根目录
        ├── msg/
        │   └── NumStr.msg        # 自定义消息文件
        ├── publisher.cpp         # C++ 发布者节点
        ├── subscriber.py         # Python 订阅者节点
        ├── CMakeLists.txt        # 编译配置文件
        ├── package.xml           # 依赖配置文件
        └── README.md             # 本说明文档
```

---

# 三、核心代码
## 1. 自定义消息：msg/NumStr.msg
```
int64 num
string str
```

## 2. C++ 发布者：publisher.cpp
```cpp
// 包含ROS2核心功能库
#include "rclcpp/rclcpp.hpp"
// 包含自定义消息类型NumStr
#include "homework3/msg/num_str.hpp"
// 时间库，用于定时发布
#include <chrono>
// 函数绑定库
#include <functional>
// 智能指针库
#include <memory>
// 字符串库
#include <string>

// 使用时间字面量
using namespace std::chrono_literals;

// 定义发布者类，继承ROS2节点类
class Publisher : public rclcpp::Node
{
public:
    // 构造函数：初始化节点名、计数器
    Publisher() : Node("Cpp_Pub_Node"), count_(0)
    {
        // 创建发布者：话题名 num_str_topic，队列长度10
        publisher_ = this->create_publisher<homework3::msg::NumStr>("num_str_topic", 10);
        // 创建定时器：500ms触发一次回调函数
        timer_ = this->create_wall_timer(500ms, std::bind(&Publisher::timer_callback, this));
        // 打印节点启动日志
        RCLCPP_INFO(this->get_logger(), "C++ 发布节点已启动，话题：num_str_topic");
    }

private:
    // 定时器回调函数：定时发布消息
    void timer_callback()
    {
        // 定义自定义消息对象
        auto msg = homework3::msg::NumStr();
        // 给消息的num字段赋值
        msg.num = count_++;
        // 给消息的str字段赋值
        msg.str = "Hello from C++ Publisher Node";
        // 打印发布内容日志
        RCLCPP_INFO(this->get_logger(), "发布：num = %ld，str = %s", msg.num, msg.str.c_str());
        // 发布消息到话题
        publisher_->publish(msg);
    }

    // 定时器智能指针
    rclcpp::TimerBase::SharedPtr timer_;
    // 发布者智能指针
    rclcpp::Publisher<homework3::msg::NumStr>::SharedPtr publisher_;
    // 计数器，用于生成序号
    size_t count_;
};

// 主函数
int main(int argc, char * argv[])
{
    // 初始化ROS2环境
    rclcpp::init(argc, argv);
    // 循环运行节点
    rclcpp::spin(std::make_shared<Publisher>());
    // 关闭ROS2环境
    rclcpp::shutdown();
    // 程序正常退出
    return 0;
}

```

## 3. Python 订阅者：subscriber.py
```python
#!/usr/bin/env python3
# 导入ROS2 Python核心库
import rclpy
# 从rclpy中导入节点基类
from rclpy.node import Node
# 导入自定义消息类型NumStr
from homework3.msg import NumStr

# 定义订阅者类，继承ROS2的Node类
class Subscriber(Node):
    # 构造函数
    def __init__(self):
        # 初始化节点，节点名称：Python_Sub_Node
        super().__init__("Python_Sub_Node")
        
        # 创建订阅者，订阅话题num_str_topic
        self.subscription = self.create_subscription(
            NumStr,                # 消息类型
            "num_str_topic",       # 订阅的话题名称
            self.listener_callback,# 收到消息后的回调函数
            10                    # 消息队列长度
        )
        
        # 打印节点启动日志
        self.get_logger().info("Python 订阅节点已启动，等待消息...")

    # 消息回调函数：收到消息时自动调用
    def listener_callback(self, msg):
        # 打印收到的消息内容
        self.get_logger().info(f'收到消息：num = {msg.num}，str = {msg.str}')

# 主函数
def main(args=None):
    # 初始化ROS2 Python环境
    rclpy.init(args=args)
    
    # 创建订阅者节点实例
    subscriber = Subscriber()
    
    # 循环运行节点，保持程序不退出
    rclpy.spin(subscriber)
    
    # 销毁节点
    subscriber.destroy_node()
    
    # 关闭ROS2环境
    rclpy.shutdown()

# 程序入口
if __name__ == '__main__':
    main()

```

## 4. 编译配置：CMakeLists.txt
```cmake
# 指定CMake最低版本要求
cmake_minimum_required(VERSION 3.8)

# 定义项目名称
project(homework3)

# GNU/Clang编译器开启警告选项
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找ROS2编译依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# 声明并编译自定义消息文件
rosidl_generate_interfaces(${PROJECT_NAME}
  msg/NumStr.msg
)

# 编译C++发布者节点
add_executable(publisher src/publisher.cpp)

# 添加C++节点依赖
ament_target_dependencies(publisher rclcpp)

# 链接自定义消息C++支持库
rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")
target_link_libraries(publisher cpp_typesupport_target)

# 安装C++可执行文件到功能包目录
install(TARGETS
  publisher
  DESTINATION lib/${PROJECT_NAME}
)

# 安装Python订阅者脚本
install(PROGRAMS
  scripts/subscriber.py
  DESTINATION lib/${PROJECT_NAME}
)

# 测试配置
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

# 生成ROS2功能包配置
ament_package()

```

## 5. 依赖配置：package.xml
```xml
<?xml version="1.0"?>
<?xml-model href="http://download.ros.org/schema/package_format3.xsd" schematypens="http://www.w3.org/2001/XMLSchema"?>

<!-- ROS2 功能包配置文件 package.xml -->
<package format="3">

  <!-- 功能包名称 -->
  <name>homework3</name>

  <!-- 版本号 -->
  <version>0.0.0</version>

  <!-- 功能包描述 -->
  <description>ROS2 C++ Publisher & Python Subscriber</description>

  <!-- 维护者信息 -->
  <maintainer email="todo@todo.todo">todo</maintainer>

  <!-- 许可证 -->
  <license>Apache-2.0</license>

  <!-- 编译工具依赖 -->
  <buildtool_depend>ament_cmake</buildtool_depend>

  <!-- C++ 核心依赖 -->
  <depend>rclcpp</depend>

  <!-- Python 核心依赖 -->
  <depend>rclpy</depend>

  <!-- 自定义消息编译依赖 -->
  <build_depend>rosidl_default_generators</build_depend>

  <!-- 自定义消息运行依赖 -->
  <exec_depend>rosidl_default_runtime</exec_depend>

  <!-- Python 运行依赖 -->
  <exec_depend>rclpy</exec_depend>

  <!-- 测试依赖 -->
  <test_depend>ament_lint_auto</test_depend>
  <test_depend>ament_lint_common</test_depend>

  <!-- 导出编译类型 -->
  <export>
    <build_type>ament_cmake</build_type>
  </export>

</package>

```

---

# 四、编译与运行步骤
## 1. 创建功能包
```bash
ros2 pkg create --build-type ament_cmake homework3
```


## 2. 给Python脚本加执行权限
```bash
chmod +x subscriber.py
```

## 3. 安装依赖
```bash
sudo apt update
sudo apt install ros-humble-rosidl-default-generators ros-humble-rosidl-default-runtime
```

## 4. 编译功能包
```bash
colcon build --packages-select homework3
```

## 5. 刷新环境变量
```bash
source install/setup.bash
```

---

# 五、运行节点
## 终端1：启动C++发布者
```bash
ros2 run homework3 publisher
```

## 终端2：启动Python订阅者
```bash
ros2 run homework3 subscriber.py
```
---
# 六、结果展示
![跨语言话题通信展示](./pre.png)

# 七、常见问题与解决方案
1. 编译失败：找不到自定义消息/头文件

   解决方案：重新编译并刷新环境,确保消息生成生效
   ```bash
   colcon build --packages-select homework3
   source install/setup.bash
   ```

2. Python脚本无法运行/提示权限不足

   解决方案：添加执行权限
   ```
   cd ~/homework3
   chmod +x subscriber.py
   ```

4. C++与Python无法通信/收不到消息

   解决方案：
   - 确认消息类型一致：`homework3/msg/NumStr`
   - 两个终端都必须执行 `source install/setup.bash`
   - 先启动发布者，再启动订阅者

---

# 八、核心参数说明
| 参数类型       | 具体内容                                                                 |
|----------------|--------------------------------------------------------------------------|
| C++ 发布节点名 | `Cpp_Pub_Node`                                                           |
| Python 订阅节点名 | `Python_Sub_Node`                                                       |
| 话题名称       | `num_str_topic`                                                          |
| 消息类型       | `homework3/msg/NumStr`                                                   |
| 消息字段       | `num(int64)`：自增序号<br>`str(string)`：固定文本消息                     |
| 发布频率       | 500ms/次（每秒2次）                                                     |
| 消息队列长度   | 10（缓存10条未处理消息，避免丢失）                                       |

---
