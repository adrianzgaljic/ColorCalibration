//
// Created by Adrian Žgaljić on 19/11/2019.
//

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main(){
    Mat img = imread("/Users/adrianzgaljic/Desktop/moneo/test1.jpg", 1);
    Mat imgGray;
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgGray, Size(9, 9), 2, 2 );

    vector<Vec3f> circles;
    HoughCircles(imgGray, circles, HOUGH_GRADIENT, 1, 400, 30, 30, 40, 70);

    Point2f topLeft, topRight, bottomLeft, bottomRight;
    vector<float> xPoints;
    vector<float> yPoints;
    if (circles.size() == 4){
        for (int i=0; i<circles.size(); i++){
           xPoints.push_back(cvRound(circles[i][0]));
           yPoints.push_back(cvRound(circles[i][1]));
        }

        sort(xPoints.begin(), xPoints.end());
        sort(yPoints.begin(), yPoints.end());

        for(int i = 0; i < circles.size(); i++ ) {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            if(center.x == xPoints.at(0) || center.x == xPoints.at(1)){
                if (center.y == yPoints.at(0) || center.y == yPoints.at(1)) {
                    topLeft = center;
                } else {
                    bottomLeft = center;
                }
            } else {
                if (center.y == yPoints.at(0) || center.y == yPoints.at(1)){
                    topRight = center;
                } else {
                    bottomRight = center;
                }
            }
        }

        cout << "rez:" << endl;
        cout << "top left:" << topLeft.x << "," << topLeft.y << endl;
        cout << "top right:" << topRight.x << "," << topRight.y << endl;
        cout << "bottom left:" << bottomLeft.x << "," << bottomLeft.y << endl;
        cout << "bottom right:" << bottomRight.x << "," << bottomRight.y << endl;

        Rect stripROI(topLeft.x, topLeft.y, topRight.x-topLeft.x, bottomLeft.y - topLeft.y);
        Mat imgStrip = img(stripROI);
        imshow("strip", imgStrip);


    }




    for(int i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cout << radius << endl;
        circle(img, center, 3, Scalar(0,255,0), -1, 8, 0 );
        circle(img, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    imshow("img", img);
    waitKey(0);
    return 0;
}

