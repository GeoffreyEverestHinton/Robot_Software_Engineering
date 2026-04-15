#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from homework3.msg import NumStr

class PythonSubNode(Node):
    def __init__(self):
        super().__init__("Python_Sub_Node")
        self.sub_ = self.create_subscription(NumStr, "num_str_topic", self.callback, 10)

    def callback(self, msg):
        self.get_logger().info(f"收到: num={msg.num}, str={msg.str}")

def main():
    rclpy.init()
    node = PythonSubNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()
