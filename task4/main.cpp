#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main( void ){
    cv::Mat image = cv::imread("/Users/yasha_nev/projects/opencvPractice/image.jpeg", cv::IMREAD_COLOR);
    cv::Mat imageHSV;
    cv::Mat imageLAB;
    cv::Mat imageYUV;
    cv::Mat imageRGB;

    if(image.empty())
    {
        std::cout << "Could not read the image \n";
        return 1;
    }
    cv::cvtColor(image, imageRGB, cv::COLOR_BGR2RGB);
    cv::cvtColor(image, imageHSV, cv::COLOR_BGR2HSV);
    cv::cvtColor(image, imageLAB, cv::COLOR_BGR2Lab);
    cv::cvtColor(image, imageYUV, cv::COLOR_BGR2YUV);
    
    cv::imshow("RGB", imageRGB);
    cv::imshow("HSV", imageHSV);
    cv::imshow("LAB", imageLAB);
    cv::imshow("YUV", imageYUV);
    cv::moveWindow("title", 0, 200 );
    cv::waitKey( 0 );
    return(0);
}
