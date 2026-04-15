#include "rclcpp/rclcpp.hpp"
#include "homework3/msg/num_str.hpp"

using namespace std::chrono_literals;

class CppPubNode : public rclcpp::Node
{
public:
    CppPubNode() : Node("Cpp_Pub_Node"), count_(0)
    {
        pub_ = this->create_publisher<homework3::msg::NumStr>("num_str_topic", 10);
        timer_ = this->create_wall_timer(500ms, std::bind(&CppPubNode::timer_callback, this));
    }

private:
    void timer_callback()
    {
        auto msg = homework3::msg::NumStr();
        msg.num = count_++;
        msg.str = "Hello from Cpp_Pub_Node";
        
        RCLCPP_INFO(this->get_logger(), "发布: num=%ld | %s", msg.num, msg.str.c_str());
        pub_->publish(msg);
    }

    rclcpp::Publisher<homework3::msg::NumStr>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    size_t count_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CppPubNode>());
    rclcpp::shutdown();
    return 0;
}
