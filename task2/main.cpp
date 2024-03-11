#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#define w 400

int main( void ){
    cv::Mat image = cv::imread("./image.jpeg", cv::IMREAD_COLOR);

    if(image.empty())
    {
        std::cout << "Could not read the image \n";
        return 1;
    }
    cv::Size size = image.size();

    cv::Mat croppImage1 = image(cv::Rect(0, 0, size.width / 2, size.height / 2));
    cv::Mat croppImage2 = image(cv::Rect(size.width / 2, 0, size.width / 2, size.height / 2));
    cv::Mat croppImage3 = image(cv::Rect(0, size.height / 2, size.width / 2, size.height / 2));
    cv::Mat croppImage4 = image(cv::Rect(size.width / 2, size.height / 2, size.width / 2, size.height / 2));


    cv::Mat mask = cv::Mat::zeros(image.size(), image.type());
    cv::Mat dstImage = cv::Mat::zeros(image.size(), image.type());    
    cv::circle(mask, cv::Point(mask.cols/2, mask.rows/2), 200, cv::Scalar(255, 255, 255), -1, 8, 0);
    image.copyTo(dstImage, mask);

    cv::imshow("title0", dstImage);
    cv::imshow("title1", croppImage1);
    cv::imshow("title2", croppImage2);
    cv::imshow("title3", croppImage3);
    cv::imshow("title4", croppImage4);
    cv::moveWindow("title", 0, 200 );
    cv::waitKey( 0 );
    return(0);
}
