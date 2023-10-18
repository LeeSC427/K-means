#include "kmeans/function.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "kmeans");

    ros::NodeHandle nh;
 
    std::cout << CV_VERSION << std::endl;
 
    K_means kmean;
 
    kmean.Kmeans();

    ros::ServiceServer ss = nh.advertiseService("img_cluster", &K_means::service_cluster, (K_means*) &kmean);

    ros::spin();

    return 0;
}