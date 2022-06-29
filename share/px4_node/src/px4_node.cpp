#include <cstdio>
#include "mav/mavlink_control.h"
#include "service_interface/srv/location.hpp"
#include "msg_interface/msg/control.hpp"
#include "std_msgs/msg/empty.hpp"
#include <rclcpp/rclcpp.hpp>
#include <fstream>
#include <cassert>

void parse_commandline(int argc, char **argv, char *&uart_name, int &baudrate,
					   bool &use_udp, char *&udp_ip, int &udp_port, bool &autotakeoff)
{

	// string for command line usage
	const char *commandline_usage = "usage: mavlink_control [-d <devicename> -b <baudrate>] [-u <udp_ip> -p <udp_port>] [-a ]";

	// Read input arguments
	for (int i = 1; i < argc; i++)
	{ // argv[0] is "mavlink"

		// Help
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
		{
			printf("%s\n", commandline_usage);
			throw EXIT_FAILURE;
		}

		// UART device ID
		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--device") == 0)
		{
			if (argc > i + 1)
			{
				i++;
				uart_name = argv[i];
			}
			else
			{
				printf("%s\n", commandline_usage);
				throw EXIT_FAILURE;
			}
		}

		// Baud rate
		if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--baud") == 0)
		{
			if (argc > i + 1)
			{
				i++;
				baudrate = atoi(argv[i]);
			}
			else
			{
				printf("%s\n", commandline_usage);
				throw EXIT_FAILURE;
			}
		}

		// UDP ip
		if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--udp_ip") == 0)
		{
			if (argc > i + 1)
			{
				i++;
				udp_ip = argv[i];
				use_udp = true;
			}
			else
			{
				printf("%s\n", commandline_usage);
				throw EXIT_FAILURE;
			}
		}

		// UDP port
		if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0)
		{
			if (argc > i + 1)
			{
				i++;
				udp_port = atoi(argv[i]);
			}
			else
			{
				printf("%s\n", commandline_usage);
				throw EXIT_FAILURE;
			}
		}

		// Autotakeoff
		if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--autotakeoff") == 0)
		{
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
		// The pointers for reaching into the location variables inside the autopilot interface
		x_ptr = &x;
		y_ptr = &y;
		yaw_ptr = &yaw;
		init_x_ptr = &i_x;
		init_y_ptr = &i_y;
		init_yaw_ptr = &i_yaw;
		// pointers end.
		char *uart_name = (char *)"/dev/ttyACM0";
		int baudrate = 115200;
		// logging of velocity commands to check for erronous control outputs after runs.
		try
		{
			f.open("/root/host_ws/src/a.txt", std::ofstream::out | std::ofstream::trunc);
			f << "START" << std::endl;
		}
		catch (std::ofstream::failure e)
		{
			std::cerr << "Exception opening/reading/closing file\n";
			if (f.rdstate() & std::ifstream::failbit)
			{
				std::cerr << "Logical:err\n";
			}
		}
		bool use_udp = false;
		char *udp_ip = (char *)"127.0.0.1";
		int udp_port = 14540;
		bool autotakeoff = false;
		char *argv[2] = {"mavlink_control", NULL};
		// do the parse, will throw an int if it fails
		parse_commandline(1, argv, uart_name, baudrate, use_udp, udp_ip, udp_port, autotakeoff);
		//open the serial port and the interface
		port = new Serial_Port(uart_name, baudrate);
		autopilot_interface = new Autopilot_Interface(port);
		port->start();
		autopilot_interface->start();
		
		//subscription callback group
		cg1 = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
		//service callback group
		cg2 = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

		auto opt = rclcpp::SubscriptionOptions();
		opt.callback_group = cg1;
		location_service = this->create_service<service_interface::srv::Location>("/service/odom", std::bind(&px4_node::location_callback, this, std::placeholders::_1, std::placeholders::_2), rmw_qos_profile_services_default, cg2);
		i_location_service = this->create_service<service_interface::srv::Location>("/init/odom", std::bind(&px4_node::i_location_callback, this, std::placeholders::_1, std::placeholders::_2), rmw_qos_profile_services_default, cg2);
		control_sub = this->create_subscription<msg_interface::msg::Control>("/control", 10, std::bind(&px4_node::control_callback, this, std::placeholders::_1), opt);
		shutdown_sub = this->create_subscription<std_msgs::msg::Empty>("/shutdown", 10, std::bind(&px4_node::shutdown_callback, this, std::placeholders::_1), opt);
		
	}
	~px4_node()
	{

		autopilot_interface->stop();
		port->stop();

		delete port;
	}
	Generic_Port *port;
	Autopilot_Interface *autopilot_interface;
	double x, y, yaw;
	double i_x, i_y, i_yaw;
	std::ofstream f;
	rclcpp::Service<service_interface::srv::Location>::SharedPtr location_service;
	rclcpp::Service<service_interface::srv::Location>::SharedPtr i_location_service;
	rclcpp::Subscription<msg_interface::msg::Control>::SharedPtr control_sub;
	rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr shutdown_sub;
	rclcpp::CallbackGroup::SharedPtr cg1;
	rclcpp::CallbackGroup::SharedPtr cg2;
	void location_callback(const std::shared_ptr<service_interface::srv::Location::Request> request,
						   std::shared_ptr<service_interface::srv::Location::Response> response)
	{
		//synchronize the reads of location.
		//this is needed as x,y,yaw all need to be updated together and one should not be able to read old x and new y and new yaw
		//because of a context switch or a parallel execution.
		read_location_msg_mtx.lock();
		response->x = x;
		response->y = y;
		response->yaw = yaw;
		read_location_msg_mtx.unlock();
	}
	void i_location_callback(const std::shared_ptr<service_interface::srv::Location::Request> request,
							 std::shared_ptr<service_interface::srv::Location::Response> response)
	{
		//initial location callback gets set only once.
		response->x = i_x;
		response->y = i_y;
		response->yaw = i_yaw;
	}
	void shutdown_callback(const std_msgs::msg::Empty::SharedPtr msg)
	{
		// we have reached goal position shutdown the controller.
		rclcpp::shutdown();
	}

	void control_callback(const msg_interface::msg::Control::SharedPtr msg)
	{
		//send velocity commands
		
		autopilot_interface->current_setpoint.data.vx = msg->vx;
		autopilot_interface->current_setpoint.data.vy = msg->vy;
		autopilot_interface->current_setpoint.data.coordinate_frame = MAV_FRAME_BODY_OFFSET_NED;
		//MAV_FRAME_BODY_OFFSET_NED : velocity is with respect to the body of the robot.
		autopilot_interface->current_setpoint.data.type_mask = MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_VELOCITY;
		
		autopilot_interface->write_setpoint();
		//log the velocity commands
		f << "vx: " << msg->vx << " vy: " << msg->vy << std::endl;
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
