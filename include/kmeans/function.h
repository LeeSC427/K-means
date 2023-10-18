#include "kmeans/headers.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis_x(0,89);

class K_means
{
public:
    struct centroid
    {
        int k;
        int x;
        int r, g, b;

        centroid() : k(0), x(0), r(0), g(0), b(0)
        {
        }
        ~centroid()
        {
        }
    };

    struct img_pixels
    {
        int k;
        int r, g, b;

        img_pixels() : k(0), r(0), g(0), b(0)
        {
        }
        ~img_pixels()
        {
        }
    };

    std::vector<centroid> cen_vec;
    std::vector<img_pixels> img_vec;
    int ksize;
    int img_num;
    std::string file_name;

    void Kmeans()
    {
        get_img_pix();

        first_cent();

        cluster_loop();

        check_cluster();
    }

    void check_cluster()
    {
        for(int i = 0; i < img_vec.size(); i++)
        {
            std::string fruit;
            int k = i + 1;

            if(img_vec[i].k == 0)
                fruit = "apple";
            else if(img_vec[i].k == 1)
                fruit = "banana";
            else if(img_vec[i].k == 2)
                fruit = "grape";

            std::cout << "img" << k << "_cluster : " << fruit << std::endl;
        }
    }

    void get_img_pix()
    {
        std::vector<img_pixels> temp_vec;

        for(int i = 1; i <= img_num; i++)
        {
            std::string img_name = std::to_string(i);
            std::string format = ".jpg";
            std::string file_path = file_name + img_name + format;
            cv::Mat img_mat;
            char imgName[255];
            img_pixels pix_temp;
            cv::Mat resize_img;

            // sprintf(imgName, "/home/cona/fruit/img%02d.jpg",i);
            //img_mat = cv::imread("/home/cona/fruit/img%d.jpg",i);
            img_mat = cv::imread(file_path, cv::IMREAD_COLOR);
                        
            std::cout << file_path << std::endl;

            cv::resize(img_mat, resize_img,cv::Size(100, 100),0, 0, CV_INTER_LINEAR);  

            if(img_mat.empty())
            {
                continue;
            }

           
            pix_temp = get_avg_rgb(resize_img);
            // pix_temp = get_avg_rgb(img_mat);


            std::cout << pix_temp.r << " , " << pix_temp.g << " , " << pix_temp.b << std::endl;

            temp_vec.push_back(pix_temp);
            std::cout << "done_get_img_pix" << std::endl;
        }


        temp_vec.swap(img_vec);
    }

    img_pixels get_avg_rgb(cv::Mat &image)
    {
        img_pixels pix_temp;
        int avg_r = 0, avg_g = 0, avg_b = 0;
        int num = 0;
        // std::cout << "get_avg1" << std::endl;
        // std::cout << "image_size : " << image.size() << std::endl;
        for(int i = 0; i < image.rows; i++)
        {
            //  std::cout << "get_avg2" << std::endl;
            for(int j = 0; j < image.cols; j++)
            {
            //  std::cout << "get_avg3" << std::endl;
               
                 if((int)image.at<cv::Vec3b>(j,i)[0] >= 250 && (int)image.at<cv::Vec3b>(j,i)[1] >= 250 && (int)image.at<cv::Vec3b>(j,i)[2] >= 250)
                { 
                    // std::cout << "get_avg4" << std::endl;
               
                    // std::cout << "get_avg5" << std::endl;
                              
                }
                else
                {
                        avg_b += (int)image.at<cv::Vec3b>(j,i)[0];
                        avg_g += (int)image.at<cv::Vec3b>(j,i)[1];
                        avg_r += (int)image.at<cv::Vec3b>(j,i)[2];
                    num++;
                }
            }
        }
        if(num != 0)
        {
            pix_temp.b = (int)avg_b / num;
            pix_temp.g = (int)avg_g / num;
            pix_temp.r = (int)avg_r / num;
        }

        return pix_temp;
    }

    void first_cent()
    {
        std::vector<centroid> temp_vec;
        std::cout << "first_pos" << std::endl;

        for(int i = 0; i < ksize; i++)
        {
            
            centroid cent;
            cent.k = i;

            if(cent.k == 0)
            {
                cent.r = 255;
                cent.g = 0;
                cent.b = 0;
            }

            else if(cent.k == 1)
            {
                cent.r = 255;
                cent.g = 255;
                cent.b = 0;
            }

            else if(cent.k == 2)
            {
                cent.r = 0;
                cent.g = 0;
                cent.b = 0;
            }

            temp_vec.push_back(cent);
        }
        std::cout << "done_init_pos" << std::endl;

        temp_vec.swap(cen_vec);

    }

    void init_pos()
    {
        std::vector<centroid> temp_vec;
        std::cout << "start_init_pos" << std::endl;
        for(int i = 0; i < ksize; i++)
        {
            centroid cent;
            cent.k = i;
            cent.x = dis_x(gen);
            bool same_chk = false;

            for(int j = 0; j < img_vec.size(); j++)
            {
                if(j = cent.x)
                {
                    cent.r = img_vec[j].r;
                    cent.g = img_vec[j].g;
                    cent.b = img_vec[j].b;

                    break;
                }
            }

            while(!same_chk)
            {
                int seq = 0;
                std::cout << "same_chk seq: " << seq << std::endl;
                std::cout << cent.x << std::endl;                
                if(temp_vec.size() != 0)
                {
                    for(int k = 0; k < temp_vec.size(); k++)
                    {
                        if(temp_vec[k].r == cent.r && temp_vec[k].g == cent.g && temp_vec[k].b == cent.b)
                        {
                            // std::cout << temp_vec[k].r << " , " << cent.r << std::endl;
                            same_chk = false;

                            break;
                        }
                        seq++;
                    }
                    
                }

                if(seq == temp_vec.size())
                    same_chk = true;
                
                if(!same_chk)
                {
                    cent.x = dis_x(gen);
                    for(int m = 0; m < img_vec.size(); m++)
                    {
                        if(m = cent.x)
                        {
                            cent.r = img_vec[m].r;
                            cent.g = img_vec[m].g;
                            cent.b = img_vec[m].b;

                            break;
                        }
                    }
                }
            }

            temp_vec.push_back(cent);
        }
        std::cout << "done_init_pos" << std::endl;

        temp_vec.swap(cen_vec);
    }

    void clusterize()
    {
        for(int i = 0; i < img_vec.size(); i++)
        {
            double dist_ref = 9999.0;

            for(int j = 0; j < cen_vec.size(); j++)
            {
                double dist = rgb_distance(img_vec[i], cen_vec[j]);

                if(dist < dist_ref)
                {
                    dist_ref = dist;
                    img_vec[i].k = j;
                }
            }
        }
    }

    void re_centralize()
    {
        for(int i = 0; i < cen_vec.size(); i++)
        {
            int sum_r = 0, sum_g = 0, sum_b = 0;
            int num_pixel = 0;

            for(int j = 0; j < img_vec.size(); j++)
            {
                if(img_vec[j].k == cen_vec[i].k)
                {
                    sum_r += img_vec[j].r;
                    sum_g += img_vec[j].g;
                    sum_b += img_vec[j].b;

                    num_pixel++;
                }
            }
            if(num_pixel != 0)
            {
                cen_vec[i].r = (int)(sum_r / num_pixel);
                cen_vec[i].g = (int)(sum_g / num_pixel);
                cen_vec[i].b = (int)(sum_b / num_pixel);
                // std::cout << cen_vec[i].r << " , " << cen_vec[i].g << " , " << cen_vec[i].b << std::endl;
            }

        }
    }

    void cluster_loop()
    {
        int seq = 0;
        
        while(seq != 1000)
        {
            // std::vector<centroid> prev_vec(cen_vec.size());
            // std::copy(cen_vec.begin(), cen_vec.end(), prev_vec.begin());
            
            clusterize();

            re_centralize();

            seq++;

            // std::cout << seq << std::endl;
        }
    }

    double rgb_distance(img_pixels pix, centroid cen)
    {
        return sqrt(pow((pix.r - cen.r),2) + pow((pix.g - cen.g),2) + pow((pix.b - cen.b),2));
    }

    bool service_cluster(kmeans::kmeans::Request &req, kmeans::kmeans::Response &res)
    {
        std::string path_name = req.file_name;

        img_pixels pix_temp;

        cv::Mat img_mat;
        cv::Mat resize;
        double dist_ref = 9999.0;
        img_mat = cv::imread(path_name, 1);

        if(img_mat.empty())
        {
            std::cout << "wrong_path" << std::endl;

            return false;
        }
        
        cv::resize(img_mat, resize, cv::Size(100,100), 0, 0, CV_INTER_LINEAR);
        
        pix_temp = get_avg_rgb(resize);

        for(int i = 0; i < ksize; i++)
        {
            double dist = rgb_distance(pix_temp, cen_vec[i]);
            if(dist_ref > dist)
            {
                dist_ref = dist;
                pix_temp.k = i;
            }
        }

        if(pix_temp.k == 0)
            res.result = "apple";
        else if(pix_temp.k == 1)
            res.result = "banana";
        else
            res.result = "grape";
        
        ROS_INFO("request: %s", path_name.c_str());
        ROS_INFO("response: %s", res.result.c_str());

        return true;
    }

    K_means()
    {
        ksize = 3;
        img_num = 90;
        file_name = "/home/cona/fruit/img";
    }
    ~K_means()
    {
    }
};