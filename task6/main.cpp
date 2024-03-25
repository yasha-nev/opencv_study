#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main( void ){
    std::string image_path = cv::samples::findFile("/Users/yasha_nev/projects/opencvPractice/image.png");
    
    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);
    cv::Mat imageColor, cannyColor;

    if(image.empty()){
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    cv::GaussianBlur(image, imageColor, cv::Size(9, 9), 0);

    cv::Canny(imageColor, cannyColor, 50, 200, 3);
   
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours( cannyColor, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );

    cv::Mat output(cv::Mat::zeros( image.size(), CV_8UC3));
    for(size_t i = 0; i< contours.size(); i++){
        cv::Scalar color = cv::Scalar(255, 255, 255);
        cv::drawContours(output, contours, (int) i, color, 1, cv::LINE_8, hierarchy, 0 );
    }

    cv::imshow("text", output);

    cv::waitKey(0);
    return 0;
}
