#include <chrono>
#include <functional>
#include <memory>
#include <iostream>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"

//#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_plane.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/features/moment_of_inertia_estimation.h>


using namespace std::chrono_literals;
typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class LidarGroundFiltering : public rclcpp::Node
{
  public:
    LidarGroundFiltering()
    : Node("lidar_ground_filtering"), 
      subscription_(this->create_subscription<sensor_msgs::msg::PointCloud2>("/point_cloud", 1, std::bind(&LidarGroundFiltering::topic_callback, this, std::placeholders::_1))),
      ground_publication_(this->create_publisher<sensor_msgs::msg::PointCloud2>("/ground_points", 1)),
      non_ground_publication_(this->create_publisher<sensor_msgs::msg::PointCloud2>("/non_ground_points", 1)) {}

  private:
    void topic_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg) const
    {
        // https://answers.ros.org/question/136916/conversion-from-sensor_msgspointcloud2-to-pclpointcloudt/
	pcl::PCLPointCloud2 pcl_pc2, filtered_pcl_pc2;
	pcl_conversions::toPCL(*msg, pcl_pc2);
	PointCloud::Ptr cloud(new PointCloud), cloud_filtered(new PointCloud);
	PointCloud ground, non_ground;
        pcl::fromPCLPointCloud2(pcl_pc2, *cloud);
	sensor_msgs::msg::PointCloud2 output;

	// msg (rcl::PC2) -> pcl_pc2 -> cloud (pcl::PC)
	// cloud_filtered (pcl::PC) -> pcl_pc2 -> output (rcl::PC2)

	pcl::fromROSMsg(*msg, *cloud);
	//__range_filter(cloud, cloud_filtered);
	range_filter(*cloud, *cloud_filtered);
        
	
	
	// msg -> pcl_pc2 ==> filtered_pcl_pc2 -> output
	ground_filter(cloud_filtered, ground, non_ground);

	auto now = this->now();
        
	pcl::toROSMsg(non_ground, output);
        non_ground_publication_->publish(output);
	
	pcl::toROSMsg(ground, output);
        ground_publication_->publish(output);
        //pcl_conversions::toPCL(now, output.header.stamp);

        //nonGroundPub.publish(nonGroundCloud);
    }
    
    void range_filter(const PointCloud& cloud, PointCloud& filteredCloud) const 
    {
	    // TODO:: change first argument's type to PCLPointCloud2
        pcl::PointIndices::Ptr unwantedPoints(new pcl::PointIndices);
        pcl::ExtractIndices<pcl::PointXYZ> extract;

	//const auto size = (cloud.width * cloud.height) * cloud.point_step;

	int i = 0;
	for (auto p : cloud.points)
	{
		const auto squareDistance = p.x * p.x + 
			                    p.y * p.y + 
					    p.z * p.z;
		
		if (49 < squareDistance)
                	unwantedPoints->indices.push_back(i);

		i++;
	}

	/*
	for(size_t p=0, i=0; p < size; p += cloud.point_step, i++) {
	    float *ptr = (float *)&cloud.data[p];
            double squaredDistance = (ptr[0] * ptr[0]) +
                                     (ptr[1] * ptr[1]) +
                                     (ptr[2] * ptr[2]);

	    //printf("%d %d %d\n", cloud.data[p + 0], cloud.data[p + 1], cloud.data[p + 2]);
	    printf("%f %f %f %f\n", squaredDistance, ptr[0], ptr[1], ptr[2]);
            if (49 < squaredDistance)
                unwantedPoints->indices.push_back(i);
        }
	*/
	
	PointCloud::ConstPtr cloudPtr = boost::make_shared<const PointCloud>(cloud);
        extract.setInputCloud(cloudPtr);
        extract.setIndices(unwantedPoints);
        extract.setNegative(true);
        extract.filter(filteredCloud);
    }

    void ground_filter(const PointCloud::ConstPtr cloud, 
		             PointCloud& groundCloud, 
			     PointCloud& nonGroundCloud) const
    {
	pcl::SampleConsensusModelPlane<pcl::PointXYZ>::Ptr
            planeModel(new pcl::SampleConsensusModelPlane<pcl::PointXYZ>(cloud));
        pcl::PointIndices::Ptr groundIndices(new pcl::PointIndices);
        pcl::ExtractIndices<pcl::PointXYZ> extract;

        pcl::RandomSampleConsensus<pcl::PointXYZ> ransac(planeModel);
        ransac.setDistanceThreshold(.01);
        ransac.computeModel();
        ransac.getInliers(groundIndices->indices);

        extract.setInputCloud(cloud);
        extract.setIndices(groundIndices);
        extract.filter(groundCloud);
        extract.setNegative(true);
        extract.filter(nonGroundCloud);
    }

    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr ground_publication_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr non_ground_publication_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<LidarGroundFiltering>());
  rclcpp::shutdown();
  return 0;
}
