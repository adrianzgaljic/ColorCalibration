#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "TransformationFinder.h"


using namespace cv;
using namespace std;

TransformationFinder tf;

void getAverageValues(Mat img, double values[3]){
    double sumRed = 0;
    double sumGreen = 0;
    double sumBlue = 0;
    double noOfPixels = img.cols * img.rows;
    for (int y = 0; y < img.rows; ++y) {
        uchar* ptr = img.ptr<uchar>(y);
        for (int x = 0; x < img.cols; ++x) {
            sumRed += ptr[x * 3];
            sumGreen += ptr[x * 3 + 1];
            sumBlue += ptr[x * 3 + 2];
        }
    }
    values[0] = sumRed/noOfPixels;
    values[1] = sumGreen/noOfPixels;
    values[2] = sumBlue/noOfPixels;
}



Mat colorCalibrateImage(Mat original, double** calibrationMatrix){

    Mat src = original.clone();
    for (int y = 0; y < src.rows; ++y) {
        uchar *ptr = src.ptr<uchar>(y);
        for (int x = 0; x < src.cols; ++x) {

            double* newValues;
            double b = ptr[x*3];
            double g = ptr[x*3 +1];
            double r = ptr[x*3 +2];

            double color[3] = {b, g, r};
            newValues = tf.transformColor(color, calibrationMatrix);
            for (int i=0; i<3; i++){
                if (newValues[i] < 0){
                    newValues[i] = 0;
                } else if (newValues[i] > 255){
                    newValues[i] = 255;
                }
            }


            ptr[x * 3] = static_cast<uchar>(newValues[0]); //blue
            ptr[x * 3 + 1] = static_cast<uchar>(newValues[1]); //green
            ptr[x * 3 + 2] = static_cast<uchar>(newValues[2]); //red
        }
    }

    return src;


}

int main() {


    Mat src = imread("processed_image.jpg", 1);
    Mat original = imread("original.jpg", 1);


    Mat b1Orig = imread("b1_original.jpg", 1);
    Mat b1Dark = imread("b1_p.jpg", 1);

    Mat b2Orig = imread("b2_original.jpg", 1);
    Mat b2Dark = imread("b2_p.jpg", 1);

    Mat b3Orig = imread("b3_original.jpg", 1);
    Mat b3Dark = imread("b3_p.jpg", 1);

    Mat b4Orig = imread("b4_original.jpg", 1);
    Mat b4Dark = imread("b4_p.jpg", 1);

    double b1OrigValue[3];
    getAverageValues(b1Orig, b1OrigValue);
    double b2OrigValue[3];
    getAverageValues(b2Orig, b2OrigValue);
    double b3OrigValue[3];
    getAverageValues(b3Orig, b3OrigValue);
    double b4OrigValue[3];
    getAverageValues(b4Orig, b4OrigValue);
    double b1DarkValue[3];
    getAverageValues(b1Dark, b1DarkValue);
    double b2DarkValue[3];
    getAverageValues(b2Dark, b2DarkValue);
    double b3DarkValue[3];
    getAverageValues(b3Dark, b3DarkValue);
    double b4DarkValue[3];
    getAverageValues(b4Dark, b4DarkValue);

    double measuredColors[][3] = {
            {b1DarkValue[0], b1DarkValue[1], b1DarkValue[2]},
            {b2DarkValue[0], b2DarkValue[1], b2DarkValue[2]},
            {b3DarkValue[0], b3DarkValue[1], b3DarkValue[2]},
            {b4DarkValue[0], b4DarkValue[1], b4DarkValue[2]}
    };
    double trueColors[][3] = {
            {b1OrigValue[0], b1OrigValue[1], b1OrigValue[2]},
            {b2OrigValue[0], b2OrigValue[1], b2OrigValue[2]},
            {b3OrigValue[0], b3OrigValue[1], b3OrigValue[2]},
            {b4OrigValue[0], b4OrigValue[1], b4OrigValue[2]}

    };

    int noOfRows = sizeof(trueColors)/sizeof(double)/3;
    double** transformation = tf.findTransformation(measuredColors, trueColors, noOfRows);
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            cout << transformation[i][j] << ",";
        }
        cout << "\n";
    }



    Mat newImg = colorCalibrateImage(src, transformation);
    imshow("blue image", src);
    imshow("calibrated image", newImg);
    imshow("original", original);


    Mat newB1 = colorCalibrateImage(b1Dark, transformation);
    Mat newB2 = colorCalibrateImage(b2Dark, transformation);
    Mat newB3 = colorCalibrateImage(b3Dark, transformation);
    Mat newB4 = colorCalibrateImage(b4Dark, transformation);

    double* b1NewValue = new double[3];
    getAverageValues(newB1, b1NewValue);
    double* b2NewValue = new double[3];
    getAverageValues(newB2, b2NewValue);
    double* b3NewValue = new double[3];
    getAverageValues(newB3, b3NewValue);
    double* b4NewValue = new double[3];
    getAverageValues(newB4, b4NewValue);
    double newValues[4][3] = {{b1NewValue[0], b1NewValue[1], b1NewValue[2]},
                              {b2NewValue[0], b2NewValue[1], b2NewValue[2]},
                              {b3NewValue[0], b3NewValue[1], b3NewValue[2]},
                              {b4NewValue[0], b4NewValue[1], b4NewValue[2]}};
    cout << "error: " << tf.getError(trueColors, newValues, noOfRows) << endl;


    imshow("new b1", newB1);
    imshow("dark b1", b1Dark);
    imshow("original b1", b1Orig);

    waitKey(0);






    return 0;
}