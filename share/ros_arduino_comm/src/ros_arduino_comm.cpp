#include <cstdio>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include "sensor_msgs/msg/imu.hpp"
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <iostream>
#include <cmath>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <chrono>
#include <thread>
#include <sys/ioctl.h>
#include <boost/thread/thread.hpp>
//#define DEBUG
static uint32_t count = 0;
enum type_id_e {
	TYPE_VELOCITY_CMD,
	TYPE_TILT_CMD,

};
enum in_type_id_e{
    IN_TYPE_IMU,
};

constexpr uint8_t type_length[16] = {2,2};
using namespace std::chrono_literals;
inline void nanosleep(){
  std::this_thread::sleep_for(std::chrono::nanoseconds(69444));
}

#define STR(a) #a
#define CONCAT(a, b) ( /**/STR(a)  STR(b) )
#define SUB_TYPE_DECL(msg,name) rclcpp::Subscription<msg>::SharedPtr sub_##name
#define TOPIC_PREFIX /
#define SUB_TYPE_INIT(msg,name,topic) sub_##name(create_subscription<msg>(CONCAT(TOPIC_PREFIX, topic), 1, std::bind(&Communicator::callback_##name, this, std::placeholders::_1)))
#define CALLBACK_FUNC(m,msg,name) void callback_##name(const m::SharedPtr msg)
#define CALLBACK_FUNC_DECL(m,msg,name) void Communicator::callback_##name(const m::SharedPtr msg)
#define HEADER(type) (type) << 4 | type_length[type] 
//struct Communicator: public rclcpp::Node;
//CALLBACK_FUNC_DECL(geometry_msgs::msg::Twist,msg,velocity);
struct Communicator : public rclcpp::Node{
    Communicator() : Node("Communicator"),
                     SUB_TYPE_INIT(geometry_msgs::msg::Twist,velocity,cmd_vel),
                    pub_(create_publisher<sensor_msgs::msg::Imu>("/imu", 1))
 
    {
        // PIPE INITIALIZATION
	serial_port = open("/dev/ttyUSB0", O_RDWR);
	//printf("%d\n", serial_port);
    // Check for errors
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }
    fcntl(serial_port, F_SETFL, 0);
    

// Read in existing settings, and handle any error
// NOTE: This is important! POSIX states that the struct passed to tcsetattr()
// must have been initialized with a call to tcgetattr() overwise behaviour
// i undefined
    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }
    tty.c_cflag &= ~PARENB; // No parity
    tty.c_cflag &= ~CSTOPB; // One stop bit
    tty.c_cflag &= ~CSIZE; // clear size bits
    tty.c_cflag |= CS8; // byte size 8 bit
    tty.c_cflag &= ~CRTSCTS; // No RTS/CTS
    tty.c_lflag &= ~ICANON; // No Canonical
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);
    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }
    // timer_ = this->create_wall_timer(
    // 100ms, std::bind(&Communicator::timer_callback, this));
    imu_msg.orientation_covariance = {0.0025, 0, 0, 0, 0.0025, 0, 0, 0, 0.0025};
    imu_msg.linear_acceleration_covariance = {0.0025, 0, 0, 0, 0.0025, 0, 0, 0, 0.0025};
    imu_msg.angular_velocity_covariance = {0.0025, 0, 0, 0, 0.0025, 0, 0, 0, 0.0025};
    imu_msg.header.frame_id = "imu_link";
    //std::thread t(Communicator::thread_call,this);
    }
    void imu_publisher(float* imu_data){
        
        tf2::Quaternion q;
        q.setRPY(imu_data[0], imu_data[1], imu_data[2]);
        imu_msg.orientation = tf2::toMsg(q);
        imu_msg.header.stamp = this->get_clock()->now();
        imu_msg.linear_acceleration.x = imu_data[3];
        imu_msg.linear_acceleration.y = imu_data[4];
        imu_msg.linear_acceleration.z = imu_data[5];
        imu_msg.angular_velocity.x = imu_data[6];
        imu_msg.angular_velocity.y = imu_data[7];
        imu_msg.angular_velocity.z = imu_data[8];

        pub_->publish(imu_msg);
    }
    CALLBACK_FUNC(geometry_msgs::msg::Twist,msg,velocity) {
		data[0] = HEADER(TYPE_VELOCITY_CMD);
        const double linear = msg->linear.x;
        double angle = msg->angular.z * (180.0/3.141592653589793238463);
        angle > 45 ? (angle = 45) : 0;
        angle < -45 ? (angle = -45) : 0;
        angle += 90;
        
        data[1] =((linear > 1) ? 135 : (linear < -1) ? 45 : std::floor(linear*45+90));
        data[2] =std::floor(angle);
        //        data[3] = '\n';
        //         std::cout << (uint32_t)data[1] << std::endl;
        //std::cout << (uint32_t)data[2] << std::endl;
        //std::cout << std::flush;
	//printf("%d\n", serial_port);
	int n = write(serial_port, data, type_length[TYPE_VELOCITY_CMD]+1);
    count++;
        if (n < 0)
  			printf("Error %i from write: %s\n", errno, strerror(errno));
        sync();
	//std::cout << data[0] << data[1] << data[2]  << std::flush;
	/*if (n < 0)
            printf("Error %i from flush: %s\n", errno, strerror(errno));
        */		
    }
    static void thread_call(std::shared_ptr<Communicator> c){
        auto next = std::chrono::steady_clock::now();
        auto prev = next - 35ms;
        while(1){
            auto now = std::chrono::steady_clock::now();
            //std::cout << round<milliseconds>(now - prev) << '\n';
        prev = now;

            int bytes_available;
        uint8_t type;
        uint8_t length;
        ioctl(c->serial_port, FIONREAD, &bytes_available);

        while(bytes_available < 2){
            nanosleep();
            ioctl(c->serial_port, FIONREAD, &bytes_available);
        }
        read(c->serial_port,&type,1);
        read(c->serial_port,&length,1);

        ioctl(c->serial_port, FIONREAD, &bytes_available);
        while(bytes_available < length){
          nanosleep();
          ioctl(c->serial_port, FIONREAD, &bytes_available);
        } // wait for the rest (probably already came)                                                                                                                               
        //std::cout << (uint32_t)header <<std::endl;
        read(c->serial_port, c->in_data, length);

        switch(type){
            case IN_TYPE_IMU:
            {
                float* imu_data = (float*)c->in_data;
                c->imu_publisher(imu_data);
            }
            break;
        }
next += 35ms;
        std::this_thread::sleep_until(next);
        }
    }
        void timer_callback(){
        int bytes_available;
        uint8_t type;
        uint8_t length;
        ioctl(serial_port, FIONREAD, &bytes_available);

        while(bytes_available < 2){
            nanosleep();
            ioctl(serial_port, FIONREAD, &bytes_available);
        }
        read(serial_port,&type,1);
        read(serial_port,&length,1);

        ioctl(serial_port, FIONREAD, &bytes_available);
        while(bytes_available < length){
          nanosleep();
          ioctl(serial_port, FIONREAD, &bytes_available);
        } // wait for the rest (probably already came)                                                                                                                               
        //std::cout << (uint32_t)header <<std::endl;
        read(serial_port, in_data, length);

        switch(type){
            case IN_TYPE_IMU:
            {
                float* imu_data = (float*)in_data;
                imu_publisher(imu_data);
            }
            break;
        }

    }

    rclcpp::TimerBase::SharedPtr timer_;
	SUB_TYPE_DECL(geometry_msgs::msg::Twist,velocity);
	SUB_TYPE_DECL(geometry_msgs::msg::Twist,tilt);
    int serial_port;
    struct termios tty;
   
    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr pub_;
    sensor_msgs::msg::Imu imu_msg;
    uint8_t data[17];
    uint8_t in_data[64];
};







int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    // Communicator::count = 0;
    /*std::shared_ptr<Communicator> a = std::make_shared<Communicator>();
    boost::thread thread_b(Communicator::thread_call, a);*/
    rclcpp::spin(a);
    rclcpp::shutdown();
    //    t1->join();
    /*std::cout << count <<std::endl;*/
     return 0;
}
