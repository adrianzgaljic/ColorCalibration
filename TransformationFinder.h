//
// Created by Adrian Žgaljić on 09/11/2019.
//

#ifndef COLORCALIBRATION_TRANSFORMATIONFINDER_H
#define COLORCALIBRATION_TRANSFORMATIONFINDER_H


class TransformationFinder {

    public: static double** findTransformation(double originalColor[][3], double trueColor[][3], int noOfColors);

    public: static double* transformColor(double color[3], double** transformationMatrix);

};


#endif //COLORCALIBRATION_TRANSFORMATIONFINDER_H
