#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main( void ){
    cv::Mat image = cv::imread("./image.jpeg", cv::IMREAD_COLOR);

    if(image.empty())
    {
        std::cout << "Could not read the image \n";
        return 1;
    }
    cv::Size size = image.size();

    cv::line(image,
             cv::Point(size.width / 10, size.height / 10),
             cv::Point(size.width / 4, size.height / 10),
             cv::Scalar(55, 255, 55),
             5
             );

    cv::circle(image,
               cv::Point(size.width / 2, size.height / 2),
               40,
               cv::Scalar(55, 55, 255),
               5
               );

    cv::circle(image,
               cv::Point(size.width - size.width / 10, size.height / 10),
               40,
               cv::Scalar(55, 255, 55),
               -1
               );

    cv::rectangle(image,
                  cv::Point(size.width / 10, size.height - size.height /10),
                  cv::Point(size.width / 3, size.height - size.height / 4),
                  cv::Scalar(255, 55, 255),
                  -1);

    cv::rectangle(image,
                  cv::Point(size.width - size.width / 10, size.height - size.height / 10),
                  cv::Point(size.width - size.width / 3, size.height - size.height / 4),
                  
                  5);
    
    cv::putText(image, 
                std::to_string(size.width),
                cv::Point(size.width / 2, size.height / 2 - 10),
                cv::FONT_HERSHEY_DUPLEX,
                1,
                cv::Scalar(255, 255, 255),
                2
                );

    cv::putText(image, 
                std::to_string(size.height),
                cv::Point(size.width / 2, size.height / 2 + 10),
                cv::FONT_HERSHEY_DUPLEX,
                1,
                cv::Scalar(255, 255, 255),
                2
                );

    cv::imshow("title", image);
    cv::moveWindow("title", 0, 200 );
    cv::waitKey( 0 );
    return(0);
}
