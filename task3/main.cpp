#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main( void ){
    std::string image_path = cv::samples::findFile("/home/lotesadmin/projects/opencvtest/task3/image.jpg");

    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);
    cv::Mat gray, dst;

    if(image.empty()){
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(gray, gray, cv::Size(9, 9), 2, 2);

    cv::Canny(image, dst, 100, 200, 3);

    std::vector<cv::Vec2f> lines;
    cv::HoughLines(dst, lines, 1, CV_PI/180, 90, 0, 0);

    std::cout << lines.size() << std::endl;

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;

        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));

        cv::line(image, pt1, pt2, cv::Scalar(0,0,255), 1, cv::LINE_AA);
    }

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT,
                 2, gray.rows/4, 200, 100 );

    for( size_t i = 0; i < circles.size(); i++ ){
         cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         cv::circle( image, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
         cv::circle( image, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
    }
    cv::namedWindow( "circles", 1 );
    //cv::imshow( "circles", dst);
    cv::imshow( "circles", image);
    cv::waitKey(0);
    return 0;
}
