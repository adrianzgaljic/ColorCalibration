#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>


using namespace cv;
using namespace std;


void balance_white(cv::Mat mat) {
    Mat hsv;
    Mat hue = Mat::zeros(Size(mat.cols, mat.rows), CV_8UC1);
    Mat saturation = Mat::zeros(Size(mat.cols, mat.rows), CV_8UC1);
    Mat value = Mat::zeros(Size(mat.cols, mat.rows), CV_8UC1);
    Mat newmat = Mat::zeros(Size(mat.cols, mat.rows), CV_8UC3);

    cvtColor(mat , hsv, COLOR_RGB2HSV);
    imshow("Img", mat);
    //waitKey(0);

    for (int y = 0; y < mat.rows; ++y) {
        uchar* ptr = mat.ptr<uchar>(y);
        uchar* h_ptr = hue.ptr<uchar>(y);
        uchar* s_ptr = saturation.ptr<uchar>(y);
        uchar* v_ptr = value.ptr<uchar>(y);
        uchar* newmatel = newmat.ptr<uchar>(y);

        for (int x = 0; x < mat.cols; ++x) {
            int h = ptr[x * 3 ];
            int s = ptr[x * 3 + 1];
            int v = ptr[x * 3 + 2];
            h_ptr[x] = static_cast<uchar>(h);
            s_ptr[x] = static_cast<uchar>(s);
            v_ptr[x] = static_cast<uchar>(v);
            newmatel[x * 3] = static_cast<uchar>(h);
            newmatel[x * 3 + 1] = static_cast<uchar>(s);
            newmatel[x * 3 + 2] = static_cast<uchar>(v);

        }
    }

    imshow("hue", hue);
    imshow("saturation", saturation);
    imshow("value", value);
    imshow("hsv", newmat);

    waitKey(0);

    /*
    double discard_ratio = 0.05;
    int hists[3][256];
    memset(hists, 0, 3*256*sizeof(int));
    int max = 0;
    for (int y = 0; y < mat.rows; ++y) {
        uchar* ptr = mat.ptr<uchar>(y);
        for (int x = 0; x < mat.cols; ++x) {
            for (int j = 0; j < 3; ++j) {
                hists[j][ptr[x * 3 + j]] += 1;
                if (hists[j][ptr[x * 3 + j]]>max){
                    max = hists[j][ptr[x * 3 + j]];
                }
            }
        }
    }

    int factor = max/500 ;
    Mat histogramImageB = Mat::zeros(Size(500,255), CV_8UC3);
    for (int i=0; i<255; i++){
        uchar* ptr = histogramImageB.ptr<uchar>(i);
         for (int j=0; j<hists[0][i]/factor; j++){
             ptr[j * 3] = static_cast<uchar>(255);
             ptr[j * 3 + 1] = static_cast<uchar>(0);
             ptr[j * 3 + 2] = static_cast<uchar>(0);
        }
    }
    Mat histogramImageG = Mat::zeros(Size(500,255), CV_8UC3);
    for (int i=0; i<255; i++){
        uchar* ptr = histogramImageG.ptr<uchar>(i);
        for (int j=0; j<hists[1][i]/factor; j++){
            ptr[j * 3] = static_cast<uchar>(0);
            ptr[j * 3 + 1] = static_cast<uchar>(255);
            ptr[j * 3 + 2] = static_cast<uchar>(0);
        }
    }
    Mat histogramImageR = Mat::zeros(Size(500,255), CV_8UC3);
    for (int i=0; i<255; i++){
        uchar* ptr = histogramImageR.ptr<uchar>(i);
        for (int j=0; j<hists[2][i]/factor; j++){
            ptr[j * 3] = static_cast<uchar>(0);
            ptr[j * 3 + 1] = static_cast<uchar>(0);
            ptr[j * 3 + 2] = static_cast<uchar>(255);
        }
    }
    Mat histogram;
    vconcat(histogramImageR, histogramImageG, histogram);
    vconcat(histogram, histogramImageB, histogram);
    cv::rotate(histogram, histogram, ROTATE_90_COUNTERCLOCKWISE);

    imshow("histogram", histogram);


    // cumulative hist
    int total = mat.cols*mat.rows;
    int vmin[3], vmax[3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 255; ++j) {
            hists[i][j + 1] += hists[i][j];
        }
        vmin[i] = 0;
        vmax[i] = 255;
        while (hists[i][vmin[i]] < discard_ratio * total)
            vmin[i] += 1;
        while (hists[i][vmax[i]] > (1 - discard_ratio) * total)
            vmax[i] -= 1;
        if (vmax[i] < 255 - 1)
            vmax[i] += 1;
    }


    for (int y = 0; y < mat.rows; ++y) {
        uchar* ptr = mat.ptr<uchar>(y);
        for (int x = 0; x < mat.cols; ++x) {
            for (int j = 0; j < 3; ++j) {
                int val = ptr[x * 3 + j];
                if (val < vmin[j])
                    val = vmin[j];
                if (val > vmax[j])
                    val = vmax[j];
                ptr[x * 3 + j] = static_cast<uchar>((val - vmin[j]) * 255.0 / (vmax[j] - vmin[j]));

            }

        }
    }
     */
}

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

double determinantOfMatrix(double mat[3][3]){
    double ans;
    ans = mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2])
          - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
          + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    return ans;
}


void findSolution(double coeff[3][4], double *result) {
    double d[3][3] = {
            { coeff[0][0], coeff[0][1], coeff[0][2] },
            { coeff[1][0], coeff[1][1], coeff[1][2] },
            { coeff[2][0], coeff[2][1], coeff[2][2] },
    };
    double d1[3][3] = {
            { coeff[0][3], coeff[0][1], coeff[0][2] },
            { coeff[1][3], coeff[1][1], coeff[1][2] },
            { coeff[2][3], coeff[2][1], coeff[2][2] },
    };
    double d2[3][3] = {
            { coeff[0][0], coeff[0][3], coeff[0][2] },
            { coeff[1][0], coeff[1][3], coeff[1][2] },
            { coeff[2][0], coeff[2][3], coeff[2][2] },
    };
    double d3[3][3] = {
            { coeff[0][0], coeff[0][1], coeff[0][3] },
            { coeff[1][0], coeff[1][1], coeff[1][3] },
            { coeff[2][0], coeff[2][1], coeff[2][3] },
    };

    double D = determinantOfMatrix(d);
    double D1 = determinantOfMatrix(d1);
    double D2 = determinantOfMatrix(d2);
    double D3 = determinantOfMatrix(d3);

    if (D != 0) {
        double x = D1 / D;
        double y = D2 / D;
        double z = D3 / D;

        result[0] = x;
        result[1] = y;
        result[2] = z;
    }
    else {
        if (D1 == 0 && D2 == 0 && D3 == 0)
             cout << "Infinite solutions" << endl;
        else if (D1 != 0 || D2 != 0 || D3 != 0)
           cout << "No solutions" << endl;
    }
}
//m-measured pixel values (e.g. red=235,10,20), t-true pixel values (e.g. reg=255,0,0)
//m[0]-measurement 1 m[0][0] - red component, m[0][1] - green component, m[0][2] - blue component
//m[1]-measurement 2
//m[3]-measurement 3
void getCalibrationMatrix(double m[3][3], double t[3][3], double result[3][3]){

    double redInput[3][4] = {
            {t[0][0], t[0][1], t[0][2], m[0][0]},
            {t[1][0], t[1][1], t[1][2], m[1][0]},
            {t[2][0], t[2][1], t[2][2], m[2][0]},
    };
    double red[3];
    findSolution(redInput, red);

    double greenInput[3][4] = {
            {t[0][0], t[0][1], t[0][2], m[0][1]},
            {t[1][0], t[1][1], t[1][2], m[1][1]},
            {t[2][0], t[2][1], t[2][2], m[2][1]},
    };
    double green[3];
    findSolution(greenInput, green);

    double blueInput[3][4] = {
            {t[0][0], t[0][1], t[0][2], m[0][2]},
            {t[1][0], t[1][1], t[1][2], m[1][2]},
            {t[2][0], t[2][1], t[2][2], m[2][2]},
    };
    double blue[3];
    findSolution(blueInput, blue);

    result[0][0] = red[0];
    result[0][1] = red[1];
    result[0][2] = red[2];

    result[1][0] = green[0];
    result[1][1] = green[1];
    result[1][2] = green[2];

    result[2][0] = blue[0];
    result[2][1] = blue[1];
    result[2][2] = blue[2];


}

void multiplyVectorWithMatrix(int vector[3], double matrix[3][3], int result[3]){
    result[0] = vector[0]*matrix[0][0] + vector[1]*matrix[0][1] + vector[2]*matrix[0][2];
    result[1] = vector[0]*matrix[1][0] + vector[1]*matrix[1][1] + vector[2]*matrix[1][2];
    result[2] = vector[0]*matrix[2][0] + vector[1]*matrix[2][1] + vector[2]*matrix[2][2];
    for (int i=0; i<3; i++){
        if (result[i] < 0){
            result[i] = 0;
        } else if (result[i] > 255){
            result[i] = 255;
        }
    }


}

void colorCalibrateImage(Mat src, double calibrationMatrix[3][3]){


    for (int y = 0; y < src.rows; ++y) {
        uchar *ptr = src.ptr<uchar>(y);
        for (int x = 0; x < src.cols; ++x) {

            int newValues[3];
            int r = ptr[x*3];
            int g = ptr[x*3 +1];
            int b = ptr[x*3 +2];

            int color[3] = {r, g, b};
            multiplyVectorWithMatrix(color, calibrationMatrix, newValues);

            for (int i=0; i<3; i++){
                if (newValues[i] < 0){
                    newValues[i] = 0;
                } else if (newValues[i] > 255){
                    newValues[i] = 255;
                }
            }

            ptr[x * 3] = static_cast<uchar>(newValues[0]); //red
            ptr[x * 3 + 1] = static_cast<uchar>(newValues[1]); //green
            ptr[x * 3 + 2] = static_cast<uchar>(newValues[2]); //blue
        }
    }

}

int main() {


    Mat src = imread("processed_image.jpg", 1);
    //cvtColor(src, src, COLOR_BGR2RGB);

    Mat b1_orig = imread("b1_original.jpg", 1);
    //cvtColor(b1_orig, b1_orig, COLOR_BGR2RGB);

    Mat b1_dark = imread("b1_p.jpg", 1);
    //cvtColor(b1_dark, b1_dark, COLOR_BGR2RGB);

    Mat b2_orig = imread("b2_original.jpg", 1);
    //cvtColor(b2_orig, b2_orig, COLOR_BGR2RGB);

    Mat b2_dark = imread("b2_p.jpg", 1);
    //cvtColor(b2_dark, b2_dark, COLOR_BGR2RGB);

    Mat b3_orig = imread("b3_original.jpg", 1);
    //cvtColor(b3_orig, b3_orig, COLOR_BGR2RGB);

    Mat b3_dark = imread("b3_p.jpg", 1);
    //cvtColor(b3_dark, b3_dark, COLOR_BGR2RGB);

    double b1_origValue[3];
    getAverageValues(b1_orig, b1_origValue);
    double b2_origValue[3];
    getAverageValues(b2_orig, b2_origValue);
    double b3_origValue[3];
    getAverageValues(b3_orig, b3_origValue);
    double b1_darkValue[3];
    getAverageValues(b1_dark, b1_darkValue);
    double b2_darkValue[3];
    getAverageValues(b2_dark, b2_darkValue);
    double b3_darkValue[3];
    getAverageValues(b3_dark, b3_darkValue);

    double result[3][3];
    double measuredColors[3][3] = {
            {b1_darkValue[0], b1_darkValue[1], b1_darkValue[2]},
            {b2_darkValue[0], b2_darkValue[1], b2_darkValue[2]},
            {b3_darkValue[0], b3_darkValue[1], b3_darkValue[2]}
    };
    double trueColors[3][3] = {
            {b1_origValue[0], b1_origValue[1], b1_origValue[2]},
            {b2_origValue[0], b2_origValue[1], b2_origValue[2]},
            {b3_origValue[0], b3_origValue[1], b3_origValue[2]}
    };
    getCalibrationMatrix(trueColors, measuredColors, result);
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            cout << result[j][i] << ",";
        }
        cout << "\n";
    }

/*
    Mat src = imread("processed_image.jpg", 1);

    double result[3][3] = {
            {0.674767,0.0149959,0.201285},
            {-0.0562391,0.87316,0.029343},
            {-0.0910815,0.0665288,0.767902}
    };
    */

    //cvtColor(src, src, COLOR_BGR2RGB);

    imshow("before calibration", src);

    colorCalibrateImage(src, result);
    //cvtColor(src, src, COLOR_BGR2RGB);

    imshow("calibrated image", src);
    Mat light = imread("original.jpg", 1);
    //cvtColor(light, light, COLOR_BGR2RGB);
    imshow("original image", light);
    imwrite("calibrated.jpg", src);
    waitKey(0);


    //testing
    /*
    int newValues[3];
    int color[3] = {20,0,0};
    multiplyVectorWithMatrix(color, result, newValues);
    for (int i=0; i<3; i++){
        cout << "nv:" << newValues[i] << endl;
    }
*/



    return 0;
}