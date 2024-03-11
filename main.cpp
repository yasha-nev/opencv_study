#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#define w 400

int main( void ){
    char title[] = "title";

    cv::Mat image = cv::Mat::zeros( w, w, CV_8UC3 );

    cv::line(image,
             cv::Point(10, 100),
             cv::Point(100, 100),
             cv::Scalar(55, 255, 55),
             5
             );

    cv::circle(image,
               cv::Point(200, 200),
               40,
               cv::Scalar(55, 55, 255),
               5
               );

    cv::circle(image,
               cv::Point(300, 100),
               40,
               cv::Scalar(55, 255, 55),
               -1
               );

    cv::rectangle(image,
                  cv::Point(10, 300),
                  cv::Point(60, 350),
                  cv::Scalar(255, 55, 255),
                  -1);

    cv::rectangle(image,
                  cv::Point(300, 300),
                  cv::Point(360, 350),
                  cv::Scalar(255, 255, 255),
                  5);

    cv::imshow(title, image);
    cv::moveWindow(title, 0, 200 );

    cv::waitKey( 0 );
    return(0);
}
