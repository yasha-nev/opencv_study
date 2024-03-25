#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

void drawContour(cv::InputArray &array, cv::InputArray &image, std::string name);

int main( void ){
    std::string image_path = cv::samples::findFile("/Users/yasha_nev/projects/opencvPractice/image.png");
    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);
    cv::Mat imageColor, imageGray, cannyColor, cannyGray;

    if(image.empty()){
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(image, imageColor, cv::Size(9, 9), 0);
    cv::GaussianBlur(imageGray, imageGray, cv::Size(3, 3), 0);

    cv::Canny(imageColor, cannyColor, 50, 200, 3);
    cv::Canny(imageGray, cannyGray, 50, 50, 3);

    drawContour(cannyColor, image, "color");
    drawContour(cannyGray, image, "gray");

    cv::waitKey(0);
    return 0;
}

void drawContour(cv::InputArray &array, cv::InputArray &image, std::string name){
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::RNG rng(12345);

    cv::findContours( array, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );

    cv::Mat output(cv::Mat::zeros( image.size(), CV_8UC3 ));
    for( size_t i = 0; i< contours.size(); i++ ){
        cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        cv::drawContours(output, contours, (int) i, color, 2, cv::LINE_8, hierarchy, 0 );
    }

    for(auto &contour : contours){
        auto moment = cv::moments(contour);
        auto point = cv::Point( static_cast<float>(moment.m10 / (moment.m00 + 1e-5)),
                                static_cast<float>(moment.m01 / (moment.m00 + 1e-5)));

        std::vector<cv::Point> contoursOUT;

        cv::approxPolyDP(cv::Mat(contour), contoursOUT, 5, true );

        if (contoursOUT.size() == 4){
            cv::putText(output, "rect", point, 1, 1, cv::Scalar(255, 255, 255));
        }
        else if (contoursOUT.size() == 3){
            cv::putText(output, "triangle", point, 1, 1, cv::Scalar(255, 255, 255));
        }
        else{
            cv::putText(output, "circle", point, 1, 1, cv::Scalar(255, 255, 255));
        }
    }

    cv::imshow(name, output);
}
