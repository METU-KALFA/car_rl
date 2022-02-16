#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_plane.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/features/moment_of_inertia_estimation.h>
#include <visualization_msgs/MarkerArray.h>

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

class SubscribeAndPublish {
public:
    SubscribeAndPublish() {
        rangeFilterPub = nh.advertise<PointCloud>("/prius/center_laser/range_filtered_scan", 100);
        groundPub = nh.advertise<PointCloud>("/prius/center_laser/ground_scan", 100);
        nonGroundPub = nh.advertise<PointCloud>("/prius/center_laser/non_ground_scan", 100);
        clusteringPub = nh.advertise<PointCloud>("/prius/center_laser/clustered_scan", 100);
        boundingBoxPub = nh.advertise<visualization_msgs::MarkerArray>("/prius/center_laser/bounding_boxes", 100);
        rangeFilterSub = nh.subscribe("/prius/center_laser/scan", 100, &SubscribeAndPublish::rangeFilter, this);
        groundFilterSub = nh.subscribe("/prius/center_laser/range_filtered_scan", 100, &SubscribeAndPublish::groundFilter, this);
        clusteringSub = nh.subscribe("/prius/center_laser/non_ground_scan", 100, &SubscribeAndPublish::clustering, this);
    }

    void rangeFilter(const PointCloud::ConstPtr& input) {
        PointCloud::ConstPtr cloud(input);
        PointCloud filteredCloud;
        pcl::PointIndices::Ptr unwantedPoints(new pcl::PointIndices);
        pcl::ExtractIndices<pcl::PointXYZ> extract;

        for(int p=0; p<cloud->size(); p++) {
            float squaredDistance = (cloud->points[p].x * cloud->points[p].x) +
                                    (cloud->points[p].y * cloud->points[p].y) +
                                    (cloud->points[p].z * cloud->points[p].z);
            if (squaredDistance < 9)    {
            unwantedPoints->indices.push_back(p);
            }
        }

        extract.setInputCloud(cloud);
        extract.setIndices(unwantedPoints);
        extract.setNegative(true);
        extract.filter(filteredCloud);

        pcl_conversions::toPCL(ros::Time::now(), filteredCloud.header.stamp);
        rangeFilterPub.publish(filteredCloud);
    }

    void groundFilter(const PointCloud::ConstPtr& input) {
        PointCloud::ConstPtr cloud(input);
        PointCloud groundCloud;
        PointCloud nonGroundCloud;
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

        pcl_conversions::toPCL(ros::Time::now(), groundCloud.header.stamp);
        pcl_conversions::toPCL(ros::Time::now(), nonGroundCloud.header.stamp);

        groundPub.publish(groundCloud);
        nonGroundPub.publish(nonGroundCloud);
    }

    void clustering(const PointCloud::ConstPtr& input) {
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr clusteredCloud(new pcl::PointCloud<pcl::PointXYZRGB>);
        pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGB>);
        pcl::EuclideanClusterExtraction<pcl::PointXYZRGB> euclideanCluster;
        std::vector<pcl::PointIndices> clusterIndices;

        pcl::copyPointCloud(*input, *cloud);
        tree->setInputCloud(cloud);
        euclideanCluster.setClusterTolerance(.25);
        euclideanCluster.setMinClusterSize(30);
        euclideanCluster.setMaxClusterSize(10000);
        euclideanCluster.setSearchMethod(tree);
        euclideanCluster.setInputCloud(cloud);
        euclideanCluster.extract(clusterIndices);

        visualization_msgs::MarkerArray marker_array;
        marker_array.markers.resize(clusterIndices.size() + 1);
        marker_array.markers[0].action = visualization_msgs::Marker::DELETEALL; //Delete markers in past frame

        for(int i=0; i < clusterIndices.size(); i++) {
            uint8_t R = rand() % 255;
            uint8_t G = rand() % 255;
            uint8_t B = rand() % 255;
            uint32_t RGB = (R << 16) | (G << 8) | B;
            pcl::PointCloud<pcl::PointXYZRGB>::Ptr cluster(new pcl::PointCloud<pcl::PointXYZRGB>);
            for(int j=0; j<clusterIndices[i].indices.size(); j++) {
                (*cloud)[clusterIndices[i].indices[j]].rgb = RGB;
                cluster->push_back((*cloud)[clusterIndices[i].indices[j]]);
                clusteredCloud->push_back((*cloud)[clusterIndices[i].indices[j]]);
            }
            pcl::MomentOfInertiaEstimation <pcl::PointXYZRGB> feature_extractor;
            pcl::PointXYZRGB boundingBoxMin;
            pcl::PointXYZRGB boundingBoxMax;
            pcl::PointXYZRGB boundingBoxPos;
            Eigen::Matrix3f boundingBoxRot;
            

            feature_extractor.setInputCloud(cluster);
            feature_extractor.compute();
            feature_extractor.getOBB(boundingBoxMin, boundingBoxMax, boundingBoxPos, boundingBoxRot);

            Eigen::Quaternionf quat(boundingBoxRot);

            marker_array.markers[i+1].header.frame_id = input->header.frame_id;
            marker_array.markers[i+1].header.stamp = ros::Time();
            marker_array.markers[i+1].id =  + i;
            marker_array.markers[i+1].type = visualization_msgs::Marker::CUBE;
            marker_array.markers[i+1].action = visualization_msgs::Marker::ADD;
            marker_array.markers[i+1].pose.position.x = boundingBoxPos.x;
            marker_array.markers[i+1].pose.position.y = boundingBoxPos.y;
            marker_array.markers[i+1].pose.position.z = boundingBoxPos.z;
            marker_array.markers[i+1].pose.orientation.x = quat.x();
            marker_array.markers[i+1].pose.orientation.y = quat.y();
            marker_array.markers[i+1].pose.orientation.z = quat.z();
            marker_array.markers[i+1].pose.orientation.w = quat.w();
            marker_array.markers[i+1].scale.x = boundingBoxMax.x - boundingBoxMin.x;
            marker_array.markers[i+1].scale.y = boundingBoxMax.y - boundingBoxMin.y;
            marker_array.markers[i+1].scale.z = boundingBoxMax.z - boundingBoxMin.z;
            marker_array.markers[i+1].color.a = 1.0;
            marker_array.markers[i+1].color.r = 1.0;
            marker_array.markers[i+1].color.g = 1.0;
            marker_array.markers[i+1].color.b = 1.0;
        }

        clusteredCloud->header = cloud->header;
        clusteredCloud->width = clusteredCloud->size();
        clusteredCloud->height = 1;
        clusteredCloud->is_dense = true;

        pcl_conversions::toPCL(ros::Time(), clusteredCloud->header.stamp);

        clusteringPub.publish(clusteredCloud);
        boundingBoxPub.publish(marker_array);
    }

private:
  ros::NodeHandle nh;
  ros::Publisher rangeFilterPub;
  ros::Publisher groundPub;
  ros::Publisher nonGroundPub;
  ros::Publisher clusteringPub;
  ros::Publisher boundingBoxPub;
  ros::Subscriber rangeFilterSub;
  ros::Subscriber groundFilterSub;
  ros::Subscriber clusteringSub;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "lidar_perception");
    SubscribeAndPublish SAP;
    ros::spin();
}
