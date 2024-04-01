#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>


void getContures(cv::Mat &image){
    cv::Mat blur, canny;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    
    cv::GaussianBlur(image, blur, cv::Size(9, 9), 0);
    
    cv::Canny(blur, canny, 50, 200, 3);
    
    cv::findContours(canny, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
    
    cv::Mat output(cv::Mat::zeros(image.size(), CV_8UC3 ));
    for( size_t i = 0; i< contours.size(); i++ ){
        cv::Scalar color = cv::Scalar(255, 255, 255);
        cv::drawContours(image, contours, (int) i, color, 2, cv::LINE_8, hierarchy, 0 );
    }
    
    for(auto &contour : contours){
        auto moment = cv::moments(contour);
        auto point = cv::Point( static_cast<float>(moment.m10 / (moment.m00 + 1e-5)),
                                static_cast<float>(moment.m01 / (moment.m00 + 1e-5)));

        std::vector<cv::Point> contoursOUT;

        cv::approxPolyDP(cv::Mat(contour), contoursOUT, 10, true );
        
        if (contoursOUT.size() == 4){
            cv::putText(image, "rectangle", point, 1, 1, cv::Scalar(255, 255, 255));
        }
    }
}


int main(int, char**){
    cv::Mat frame;
    cv::VideoCapture cap("/Users/yasha_nev/projects/opencvPractice/image.mp4");
    
    if (!cap.isOpened()){
        return 0;
    }
    
    for (;;) {
        cap.read(frame);
        if (frame.empty()) {
            break;
        }
        
        getContures(frame);
        
        cv::imshow("Live", frame);
       
        if (cv::waitKey(10) >= 0)
            break;
    }
    
    return 0;
}
