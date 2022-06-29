#include <cstdio>
#include "mav/mavlink_control.h"
#include "service_interface/srv/location.hpp"
#include "msg_interface/msg/control.hpp"
#include <rclcpp/rclcpp.hpp>

void
parse_commandline(int argc, char **argv, char *&uart_name, int &baudrate,
		bool &use_udp, char *&udp_ip, int &udp_port, bool &autotakeoff)
{

	// string for command line usage
	const char *commandline_usage = "usage: mavlink_control [-d <devicename> -b <baudrate>] [-u <udp_ip> -p <udp_port>] [-a ]";

	// Read input arguments
	for (int i = 1; i < argc; i++) { // argv[0] is "mavlink"

		// Help
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printf("%s\n",commandline_usage);
			throw EXIT_FAILURE;
		}

		// UART device ID
		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--device") == 0) {
			if (argc > i + 1) {
				i++;
				uart_name = argv[i];
			} else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}

		// Baud rate
		if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--baud") == 0) {
			if (argc > i + 1) {
				i++;
				baudrate = atoi(argv[i]);
			} else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}

		// UDP ip
		if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--udp_ip") == 0) {
			if (argc > i + 1) {
				i++;
				udp_ip = argv[i];
				use_udp = true;
			} else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}

		// UDP port
		if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) {
			if (argc > i + 1) {
				i++;
				udp_port = atoi(argv[i]);
			} else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}

		// Autotakeoff
		if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--autotakeoff") == 0) {
			autotakeoff = true;
		}

	}
	// end: for each input argument

	// Done!
	return;
}
struct px4_node : rclcpp::Node
{
  px4_node() : rclcpp::Node("px4_node")
  {
    char *uart_name = (char *)"/dev/ttyACM0";
    int baudrate = 57600;

    bool use_udp = false;
    char *udp_ip = (char *)"127.0.0.1";
    int udp_port = 14540;
    bool autotakeoff = false;
    char* argv[2] = {"mavlink_control",NULL};
    // do the parse, will throw an int if it fails
    parse_commandline(1, argv, uart_name, baudrate, use_udp, udp_ip, udp_port, autotakeoff);
    port = new Serial_Port(uart_name, baudrate);
    autopilot_interface = new Autopilot_Interface(port);
    port->start();
	  autopilot_interface->start();
    cg1 = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    cg2 = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    x_ptr = &x;
    y_ptr = &y;
    yaw_ptr = &yaw;
    auto opt = rclcpp::SubscriptionOptions();
    opt.callback_group = cg1;
    location_service = this->create_service<service_interface::srv::Location>("/service/odom", std::bind(&px4_node::location_callback, this, std::placeholders::_1, std::placeholders::_2),rmw_qos_profile_services_default, cg2);
    control_sub = this->create_subscription<msg_interface::msg::Control>("/control",10, std::bind(&px4_node::control_callback, this, std::placeholders::_1), opt);
  }
  ~px4_node()
  {

    autopilot_interface->stop();
    port->stop();

    delete port;
  }
  Generic_Port* port;
  Autopilot_Interface* autopilot_interface;
  double x,y,yaw;
  rclcpp::Service<service_interface::srv::Location>::SharedPtr location_service;
  rclcpp::Subscription<msg_interface::msg::Control>::SharedPtr control_sub;
  rclcpp::CallbackGroup::SharedPtr cg1;
  rclcpp::CallbackGroup::SharedPtr cg2;
  void location_callback(const std::shared_ptr<service_interface::srv::Location::Request> request,
                          std::shared_ptr<service_interface::srv::Location::Response> response)
  { 
    read_location_msg_mtx.lock();
    response->x = x;
    response->y = y;
    response->yaw = yaw;
    read_location_msg_mtx.unlock();
  }
  void control_callback(const msg_interface::msg::Control::SharedPtr msg)
  {
    return;
  }
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::executors::MultiThreadedExecutor executor;
  auto node = std::make_shared<px4_node>();
  executor.add_node(node);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}
