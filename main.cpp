#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "TransformationFinder.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <time.h>



using namespace cv;
using namespace std;

TransformationFinder tf;

void getAverageValues(Mat img, double values[3]){
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
    values[0] = sumBlue/noOfPixels;
    values[1] = sumGreen/noOfPixels;
    values[2] = sumRed/noOfPixels;
}

void getCalibrationPatternColors(double colors[][3], Mat image){

    Mat cropped;
    for (int i=0; i<6; i++){
        for (int j=0; j<4; j++){
            Rect roi(200*i+50, 200*j+50, 100, 100);
            cropped = image(roi);
            getAverageValues(cropped, colors[j*6+i]);
        }
    }
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
    Mat img1 = imread("/Users/adrianzgaljic/Desktop/moneo/template_2.jpg",1);

    for (int i=1; i<6; i++){
        clock_t start = clock();

        string name = "/Users/adrianzgaljic/Desktop/moneo/colors_example_" + to_string(i) + "_crop.jpg";

        Mat img2 = imread(name,1);

        //imshow("template 1", img1);
        //imshow("tamplate 2", img2);
        double colorsOriginal[24][3];
        double colorsMeasured[24][3];
        double colorsCalibrated[24][3];

        getCalibrationPatternColors(colorsOriginal, img1);
       getCalibrationPatternColors(colorsMeasured, img2);

        double** transformation = tf.findTransformation(colorsMeasured, colorsOriginal, 24);


        //tf.printTransformationMatrix(transformation);
        Mat calibrated = colorCalibrateImage(img2, transformation);

        getCalibrationPatternColors(colorsCalibrated, calibrated);
        float error = tf.getError(colorsOriginal, colorsCalibrated, 24);

/*
        cout << "Image " << i << " error: " << error << endl;
        imwrite("original.jpg", img1);
        string badname = "before_calibration_"+to_string(i) + ".jpg";
        string calibratedname = "calibrated_"+to_string(i) + ".jpg";

        //imwrite(badname, img2);
        imwrite(calibratedname, calibrated);
        imshow("calibrated", calibrated);
        //waitKey(0);
        */
        clock_t stop = clock();
        double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
        printf("\nTime elapsed: %.5f", elapsed);
        cout << "end" << endl;

    }


    /*
    cout << "colors:" << endl;
    cv::Mat mat(800, 1200, CV_8UC3, cv::Scalar(255,255,255));

    for (int i=0; i<6; i++){
        for (int j=0; j<4; j++){
            circle(mat, Point(200*i+50,200*j+50),1, Scalar(colors[j*6+i][0],colors[j*6+i][1],colors[j*6+i][2]),100);
        }
    }

    imshow("painting", mat);
    waitKey(0);



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


    cout << "--------" << endl;
    for (int i=0; i<4; i++){
        for (int j=0; j<3; j++){
            cout << newValues[i][j] << ",";
        }
        cout << "\n";
    }
    cout << "--------" << endl;
    cout << "error: " << tf.getError(trueColors, newValues, noOfRows) << endl;


    imshow("new b1", newB1);
    imshow("dark b1", b1Dark);
    imshow("original b1", b1Orig);
*/






    return 0;
}