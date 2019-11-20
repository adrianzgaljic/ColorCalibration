//
// Created by Adrian Žgaljić on 19/11/2019.
//

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <ctime>

using namespace cv;
using namespace std;

int scalingFactor = 4;

vector<Point2f> getMarkerCenters(Mat imgGray){
    vector<Point2f> result;
    GaussianBlur(imgGray, imgGray, Size(9, 9), 2, 2 );
    vector<Vec3f> circles;
    HoughCircles(imgGray, circles, HOUGH_GRADIENT, 1, 200/scalingFactor, 40, 30, 40/scalingFactor, 70/scalingFactor);

    Point2f topLeft, topRight, bottomLeft, bottomRight;
    vector<float> xPoints;
    vector<float> yPoints;


    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle( imgGray, center, radius, Scalar(255), 3, 8, 0 );
    }
    imshow("circle", imgGray);

    if (circles.size() == 4) {
        for (int i = 0; i < circles.size(); i++) {
            xPoints.push_back(cvRound(circles[i][0]));
            yPoints.push_back(cvRound(circles[i][1]));
        }

        sort(xPoints.begin(), xPoints.end());
        sort(yPoints.begin(), yPoints.end());

        for (int i = 0; i < circles.size(); i++) {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            if (center.x == xPoints.at(0) || center.x == xPoints.at(1)) {
                if (center.y == yPoints.at(0) || center.y == yPoints.at(1)) {
                    topLeft = center;
                } else {
                    bottomLeft = center;
                }
            } else {
                if (center.y == yPoints.at(0) || center.y == yPoints.at(1)) {
                    topRight = center;
                } else {
                    bottomRight = center;
                }
            }
        }
        result.push_back(topLeft);
        result.push_back(topRight);
        result.push_back(bottomLeft);
        result.push_back(bottomRight);

        cout << "top left:" << topLeft.x << "," << topLeft.y << endl;
        cout << "top right:" << topRight.x << "," << topRight.y << endl;
        cout << "bottom left:" << bottomLeft.x << "," << bottomLeft.y << endl;
        cout << "bottom right:" << bottomRight.x << "," << bottomRight.y << endl;

    } else {
        cout << "Found " << circles.size() << " circles!" << endl;
    }

    return result;
}

Mat getStripImage(Mat imgTransformed){
    Mat imgGrayTransformed;
    cvtColor(imgTransformed, imgGrayTransformed, COLOR_BGR2GRAY);
    //threshold(imgGrayTransformed, imgGrayTransformed, 40, 255, THRESH_BINARY );

    Mat edges;
    Canny(imgGrayTransformed, edges, 50, 50, 3);
    imshow("edges", edges);
    vector<Vec2f> lines;
    //shoud find better formula for defining threshold
    HoughLines(edges, lines, 1, CV_PI/180, 250-scalingFactor*30, 0, 0 );
    float averageRho1 = 0;
    float averageRho2  = 0;
    int sum1 = 1;
    int sum2 = 1;
    cout << "Found " << lines.size() << " lines." << endl;
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0];
/*
        cout << "rho:" << rho << endl;
        float theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 3000*(-b));
        pt1.y = cvRound(y0 + 3000*(a));
        pt2.x = cvRound(x0 - 3000*(-b));
        pt2.y = cvRound(y0 - 3000*(a));
        line(imgTransformed, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
*/
        if (averageRho1 == 0){
            averageRho1 = abs(rho);
        } else if (averageRho2 == 0){
            if (abs(averageRho1/sum1-abs(rho)) < 30/scalingFactor){
                averageRho1 += abs(rho);
                sum1++;
            } else {
                averageRho2 += abs(rho);
            }
        }
        if (abs(averageRho1/sum1-abs(rho)) < 30/scalingFactor){
            averageRho1 += abs(rho);
            sum1++;
        } else {
            averageRho2 += abs(rho);
            sum2++;
        }
    }
    float rho1 = averageRho1/sum1;
    float rho2 = averageRho2/sum2;

    cout << "rho1:" << rho1 << endl;
    cout << "rho2:" << rho2 << endl;

    line(imgTransformed, Point2f(rho1, 0), Point2f(rho1, imgTransformed.rows), Scalar(255,0,0),1);
    line(imgTransformed, Point2f(rho2, 0), Point2f(rho2, imgTransformed.rows), Scalar(255,0,0),1);
    imshow("lines", imgTransformed);
    Mat result;
    if (rho1 < rho2){
        Rect newROI(rho1, 0, rho2-rho1,imgGrayTransformed.rows);
        result = imgTransformed(newROI);
    } else {
        Rect newROI(rho2, 0, rho1-rho2,imgGrayTransformed.rows);
        result = imgTransformed(newROI);
    }
    return result;

}

Point3f getAverageValues(Mat img){
    Point3f colorValues;
    double sumBlue = 0;
    double sumGreen = 0;
    double sumRed = 0;
    double noOfPixels = img.cols * img.rows;
    for (int y = 0; y < img.rows; ++y) {
        uchar *ptr = img.ptr<uchar>(y);
        for (int x = 0; x < img.cols; ++x) {
            sumBlue += ptr[x * 3];
            sumGreen += ptr[x * 3 + 1];
            sumRed += ptr[x * 3 + 2];
        }
    }
    colorValues.x = sumBlue/noOfPixels;
    colorValues.y = sumGreen/noOfPixels;
    colorValues.z = sumRed/noOfPixels;
    return colorValues;
}

vector<Point3f> getColorSquares(Mat img){
    vector<Point3f> result;
    int noOfColors = 12;
    float partHeight = img.rows/12;
    float ratio = 50.0/63;
    float squareHeight = ratio * partHeight;
    float spaceHeight = partHeight - squareHeight;
    int squareOffset = 20/scalingFactor;
    for (int i=0; i<12; i++){

        Mat imgSquare = img(Rect(squareOffset, i*partHeight+squareOffset, img.cols-squareOffset, squareHeight-squareOffset));
        result.push_back(getAverageValues(imgSquare));
        rectangle(img, Point2f(squareOffset, i*partHeight+squareOffset), Point2f(img.cols-squareOffset, i*partHeight+squareHeight-squareOffset), cv::Scalar(0, 0, 255), 2);
    }
    imshow("detected squares", img);
    return result;
}

Mat perspectiveTransformImage(Mat img, Point2f topLeft, Point2f topRight, Point2f bottomLeft, Point2f bottomRight){
    topLeft.y = topLeft.y - 110/scalingFactor;
    topRight.y = topRight.y - 110/scalingFactor;
    bottomLeft.y = bottomLeft.y + 20/scalingFactor;
    bottomRight.y = bottomRight.y + 20/scalingFactor;
    int width = topRight.x - topLeft.x;
    int height = bottomLeft.y - topLeft.y;

    Point2f inputQuad[4];
    inputQuad[0] = topLeft;
    inputQuad[1] = topRight;
    inputQuad[2] = bottomLeft;
    inputQuad[3] = bottomRight;

    Point2f outputQuad[4];
    outputQuad[0] = Point2f(0,0);
    outputQuad[1] = Point2f(width,0);
    outputQuad[2] = Point2f(0,height);
    outputQuad[3] = Point2f(width,height);

    Mat lambda( 2, 4, CV_32FC1 );
    lambda = getPerspectiveTransform(inputQuad, outputQuad);
    Mat persepectiveTransformedImage(height, width, CV_8UC3);
    warpPerspective(img,persepectiveTransformedImage,lambda,persepectiveTransformedImage.size());
    return  persepectiveTransformedImage;
}


int main(){
    Mat imgOriginal = imread("/Users/adrianzgaljic/Desktop/moneo/test_1.jpg", 1);
    Mat img;
    resize(imgOriginal, img, cv::Size(), 1.0/scalingFactor, 1.0/scalingFactor);
    clock_t begin = clock();

    Mat imgGray;
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    vector<Point2f> markerCenters = getMarkerCenters(imgGray);
    if (markerCenters.size()==4){
        Point2f topLeft, topRight, bottomLeft, bottomRight;
        topLeft = markerCenters.at(0);
        topRight = markerCenters.at(1);
        bottomLeft = markerCenters.at(2);
        bottomRight = markerCenters.at(3);

        Mat imgTransformed = perspectiveTransformImage(img, topLeft, topRight, bottomLeft, bottomRight);
        Mat imgStrip = getStripImage(imgTransformed);
        imshow("perspective", imgTransformed);
        vector<Point3f> colorValues = getColorSquares(imgStrip);
        rectangle(imgOriginal, Point2f(0, 0), Point2f(400, 2500), cv::Scalar(0, 0, 0), -1);

        for (int i=0; i<colorValues.size(); i++){
            circle(imgOriginal, Point(200,200*(i+1)),1, Scalar(colorValues.at(i).x, colorValues.at(i).y, colorValues.at(i).z),100);

        }


        //imshow("strip", imgStrip);


    }
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "time: " << elapsed_secs << endl;
    imshow("original img", imgOriginal);

    waitKey(0);


    return 0;
}

