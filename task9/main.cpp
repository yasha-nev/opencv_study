#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDisplay( Mat &frame );

CascadeClassifier face_cascade;

CascadeClassifier eyes_cascade;

CascadeClassifier smile_cascade;



int main( int argc, const char** argv ){

    if( !face_cascade.load("data/haarcascades/haarcascade_frontalcatface.xml"  ) ){
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };

    if( !eyes_cascade.load("data/haarcascades/haarcascade_eye.xml") ){
        cout << "--(!)Error loading eyes cascade\n";
        return -1;
    };
    
    if (!smile_cascade.load(samples::findFile("data/haarcascades/haarcascade_smile.xml"))){
        cout << "--(!)Error loading smile cascade\n";
        return -1;
    }
    
    VideoCapture capture("ZUA.mp4");
    if ( !capture.isOpened() ){
        cout << "--(!)Error opening video capture\n";
        return -1;
    }
    
    Mat frame;
    
    VideoWriter out("/Users/yasha_nev/projects/opencvPractice/output.mp4", capture.get(CAP_PROP_FOURCC), capture.get(CAP_PROP_FPS), Size(capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT)));

    while ( capture.read(frame) ){
        if( frame.empty() ) {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }

        detectAndDisplay( frame );
        
        
        if( waitKey(10) == 27 ){
            break;
        }
        
        imshow( "Capture - Face detection", frame );
        
        out.write(frame);
    }
    return 0;
}

void detectAndDisplay( Mat &frame ){
    Mat frame_gray, blur;
    GaussianBlur(frame, blur, Size(9, 9), 3);
    cvtColor( blur, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    
    

    std::vector<Rect> faces;
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 1);
    for ( size_t i = 0; i < faces.size(); i++ ){
        Rect face = Rect(faces[i].x - 50, faces[i].y - 50, faces[i].width + 100, faces[i].height + 100);
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        Mat faceROI = frame_gray(face);

        std::vector<Rect> eyes;
        std::vector<Rect> smiles;
        
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 25);
        smile_cascade.detectMultiScale( faceROI, smiles, 1.1, 30, 0, Size(30, 30), Size(120, 120));
        
        if (eyes.size() == 2){
            for ( size_t j = 0; j < eyes.size(); j++ ){
                Point eye_center( face.x + eyes[j].x + eyes[j].width/2, face.y + eyes[j].y + eyes[j].height/2 );
                int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
                circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4 );
            }
        

            for ( size_t j = 0; j < smiles.size(); j++ ){
                Point smile_center( face.x + smiles[j].x + smiles[j].width/2, face.y + smiles[j].y + smiles[j].height/2 );
                int radius = cvRound( (smiles[j].width + smiles[j].height)*0.25 );
                circle( frame, smile_center, radius, Scalar( 0, 255, 0 ), 4 );
            }
        
            ellipse( frame, center, Size( face.width/2, face.height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
        }
    }
}
