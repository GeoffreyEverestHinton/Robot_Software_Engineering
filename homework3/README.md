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
#include "rclcpp/rclcpp.hpp"
#include "homework3/msg/num_str.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <string>

using namespace std::chrono_literals;

class Publisher : public rclcpp::Node
{
public:
    Publisher() : Node("Cpp_Pub_Node"), count_(0)
    {
        publisher_ = this->create_publisher<homework3::msg::NumStr>("num_str_topic", 10);
        timer_ = this->create_wall_timer(500ms, std::bind(&Publisher::timer_callback, this));
        RCLCPP_INFO(this->get_logger(), "C++ 发布节点已启动，话题：num_str_topic");
    }

private:
    void timer_callback()
    {
        auto msg = homework3::msg::NumStr();
        msg.num = count_++;
        msg.str = "Hello from C++ Publisher Node";
        RCLCPP_INFO(this->get_logger(), "发布：num = %ld，str = %s", msg.num, msg.str.c_str());
        publisher_->publish(msg);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<homework3::msg::NumStr>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Publisher>());
    rclcpp::shutdown();
    return 0;
}
```

## 3. Python 订阅者：subscriber.py
```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from homework3.msg import NumStr

class Subscriber(Node):
    def __init__(self):
        super().__init__("Python_Sub_Node")
        self.subscription = self.create_subscription(
            NumStr,
            "num_str_topic",
            self.listener_callback,
            10
        )
        self.get_logger().info("Python 订阅节点已启动，等待消息...")

    def listener_callback(self, msg):
        self.get_logger().info(f'收到消息：num = {msg.num}，str = {msg.str}')

def main(args=None):
    rclpy.init(args=args)
    subscriber = Subscriber()
    rclpy.spin(subscriber)
    subscriber.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

## 4. 编译配置：CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.8)
project(homework3)

# 编译选项
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 依赖包
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# 自定义消息
rosidl_generate_interfaces(${PROJECT_NAME}
  msg/NumStr.msg
)

# C++ 可执行文件
add_executable(publisher src/publisher.cpp)
ament_target_dependencies(publisher rclcpp)
rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")
target_link_libraries(publisher cpp_typesupport_target)

install(TARGETS
  publisher
  DESTINATION lib/${PROJECT_NAME}
)

# Python 脚本
install(PROGRAMS
  scripts/subscriber.py
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
```

## 5. 依赖配置：package.xml
```xml
<?xml version="1.0"?>
<?xml-model href="http://download.ros.org/schema/package_format3.xsd" schematypens="http://www.w3.org/2001/XMLSchema"?>
<package format="3">
  <name>homework3</name>
  <version>0.0.0</version>
  <description>ROS2 C++ Publisher & Python Subscriber</description>
  <maintainer email="todo@todo.todo">todo</maintainer>
  <license>Apache-2.0</license>

  <buildtool_depend>ament_cmake</buildtool_depend>
  <depend>rclcpp</depend>
  <depend>rclpy</depend>
  <build_depend>rosidl_default_generators</build_depend>
  <exec_depend>rosidl_default_runtime</exec_depend>
  <exec_depend>rclpy</exec_depend>

  <test_depend>ament_lint_auto</test_depend>
  <test_depend>ament_lint_common</test_depend>

  <export>
    <build_type>ament_cmake</build_type>
  </export>
</package>
```

---
明白了！**不要工作空间路径，只保留你当前的功能包目录结构**，完全按你要求来，1:1 严格对应！

这是最终纯净版，直接复制：

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

# 六、常见问题与解决方案
1. **编译失败：找不到消息**
   重新编译 + 刷新环境
   ```bash
   colcon build --packages-select homework3
   source install/setup.bash
   ```

2. **Python 脚本无法运行**
   ```bash
   chmod +x subscriber.py
   ```

3. **提示未找到功能包**
   必须执行：`source install/setup.bash`

4. **C++ 与 Python 无法通信**
   - 话题名必须一致：`num_str_topic`
   - 消息类型必须一致：`homework3/msg/NumStr`
   - 两个终端都要 source 环境

---

# 七、核心参数说明
- **C++ 发布节点名**：`Cpp_Pub_Node`
- **Python 订阅节点名**：`Python_Sub_Node`
- **话题名称**：`num_str_topic`
- **消息类型**：`homework3/msg/NumStr`
- **消息字段**：`num(int64)`、`str(string)`
- **发布频率**：500ms/次
- **队列长度**：10

---
