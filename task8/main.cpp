#include <string>
#include <iostream>
#include <filesystem>
#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <algorithm>
#include <random>

std::vector<cv::Mat> descriptors;
std::vector<std::vector<cv::KeyPoint>> keypoints;
std::vector<std::string> cards_name;
std::vector<cv::Mat> cards;

void getCardsImages(std::string path, std::vector<cv::Mat> &cards, std::vector<std::string> &cards_name);

void find_cards(std::vector<std::vector<cv::Point>> &contours, cv::Mat &image);

void find_contours(cv::Mat &image, std::vector<std::vector<cv::Point>> &contours, std::vector<cv::Vec4i> &hierarchy);

void rot(cv::RotatedRect &rect, cv::Mat &image, cv::Mat &rotated);

std::string get_key_features(cv::Mat &image);


int main() {
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    
    getCardsImages("./cards/", cards, cards_name);
    
    cv::Mat image = cv::imread("./image.jpeg");
    
    find_contours(image, contours, hierarchy);
    
    find_cards(contours, image);
    
    cv::imshow("My Window", image);
    
    cv::waitKey(0);
    
    /*cv::VideoCapture cap = cv::VideoCapture("../video.mp4");
    
    while (true){
        cv::Mat image;
        contours.clear();
        hierarchy.clear();
        
        cap.read(image);
        
        if (image.empty()){
            continue;
        }
        
        find_contours(image, contours, hierarchy);
        
        find_cards(contours, image);
        
        cv::imshow("My Window", image);
    
        if (cv::waitKey(10) >= 0){
            break;
        }
    }*/
    
}

void getCardsImages(std::string path, std::vector<cv::Mat> &cards, std::vector<std::string> &cards_name){
    cv::Ptr<cv::ORB> detector = cv::ORB::create();
    for (const auto& entry : std::filesystem::directory_iterator(path)){
        std::string imPath = cv::samples::findFile(entry.path());
        
        cv::Mat card = cv::imread(imPath, cv::IMREAD_GRAYSCALE);
        
        if(card.empty()){
            continue;
        }
        std::string name = imPath;
        
        name.erase(0, 8);
        
        cv::resize(card, card, cv::Size(145, 220));
        
        card.convertTo(card, -1, 1, 50);
        
        equalizeHist( card, card );
        
        cards_name.push_back(name);
        cards.push_back(card);
        
        cv::Mat dis;
        std::vector<cv::KeyPoint> keyp;
        
        detector->detectAndCompute(card, cv::noArray(), keyp, dis);
        keypoints.push_back(keyp);
        descriptors.push_back(dis);
    }
    
    std::cout << "pre render finish" << std::endl;
}

void find_contours(cv::Mat &image, std::vector<std::vector<cv::Point>> &contours, std::vector<cv::Vec4i> &hierarchy){
    cv::Mat blur, thresh, canny;
    cv::GaussianBlur(image, blur, cv::Size(9, 9), 3);
    cv::Canny(blur, canny, 120, 120);
    cv::findContours( canny, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
}

void find_cards(std::vector<std::vector<cv::Point>> &contours, cv::Mat &image){
    std::vector<std::vector<cv::Point>> contours_poly( contours.size() );
    std::vector<cv::Rect> boundRect( contours.size() );

    for( size_t i = 0; i < contours.size(); i++ ){
        cv::Point2f vtx[4];
        auto moment = cv::moments(contours[i]);
        auto point = cv::Point( static_cast<float>(moment.m10 / (moment.m00 + 1e-5)),
                                static_cast<float>(moment.m01 / (moment.m00 + 1e-5)));
        approxPolyDP(contours[i], contours_poly[i], 1, true );
        cv::RotatedRect box = cv::minAreaRect(contours_poly[i]);
        box.points(vtx);
            
        if (box.size.width < 100 || box.size.height < 100){
            continue;
        }
        
        for(int j = 0; j < 4; j++ ){
            cv::line(image, vtx[j], vtx[(j+1) % 4], cv::Scalar(0, 255, 0), 4, cv::LINE_AA);
        }
        
        cv::Mat img;
        rot(box, image, img);
        
        if (img.empty()){
            continue;
        }
        
        cv::cvtColor(img, img, cv::COLOR_RGB2GRAY);
        equalizeHist(img, img);
        
        std::string name = get_key_features(img);
        putText(image, name, point, 1, 2, cv::Scalar(255, 255, 255), 5);
    }
}

void rot(cv::RotatedRect &rect, cv::Mat &image, cv::Mat &cropped){
    
    cv::Mat M, rotated;
    
    float angle = rect.angle;
    cv::Size rect_size = rect.size;
    if (rect.angle < -45.) {
        angle += 90.0;
        std::swap(rect_size.width, rect_size.height);
    }
    M = getRotationMatrix2D(rect.center, angle, 1.0);
    cv::warpAffine(image, rotated, M, image.size(), cv::INTER_CUBIC);
    getRectSubPix(rotated, rect_size, rect.center, cropped);
    
    if (cropped.size[0] < cropped.size[1]){
        cv::rotate(cropped, cropped, cv::ROTATE_90_CLOCKWISE);
    }
}

std::string get_key_features(cv::Mat &image){
    cv::Ptr<cv::ORB> detector = cv::ORB::create();
    
    cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create();
    std::vector<std::pair<size_t, size_t>> matches;
    std::vector<cv::KeyPoint> keypoints2;
    cv::Mat descriptors2;
    
    detector->detectAndCompute(image, cv::noArray(), keypoints2, descriptors2);
    
    if (descriptors2.empty()){
        return "";
    }
    
    for (int i = 0; i < cards.size(); i++){
        
        if (descriptors[i].empty()){
            continue;
        }
        
        std::vector< std::vector<cv::DMatch>> knn_matches;
        
        matcher->knnMatch(descriptors[i], descriptors2, knn_matches, 3);

        std::vector<cv::DMatch> good_matches;
        for (size_t i = 0; i < knn_matches.size(); i++){
            if (knn_matches[i][0].distance < 0.75 *  knn_matches[i][1].distance){
                good_matches.push_back(knn_matches[i][0]);
            }
        }
        
        matches.push_back({i, good_matches.size()});
    }
    
    std::sort(matches.begin(), matches.end(), [](std::pair<size_t, size_t> r, std::pair<size_t, size_t> l){
        return r.second > l.second;
    });
    
    return cards_name[matches[0].first];
}
