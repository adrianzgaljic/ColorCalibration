//
// Created by Adrian Žgaljić on 08/11/2019.
//

#include <iostream>
#include "TransformationFinder.h"

using namespace std;






int main(){
    cout << "bok" << endl;

    double originalColor[3][3] = {{112, 115, 131},
                           {111, 51, 202},
                           {80, 228, 211}};
    double trueColor[3][3] = {{196, 165, 113},
                              {196, 38, 210},
                              {169, 242, 221}};

    TransformationFinder tf;
    double** transformation = tf.findTransformation(originalColor, trueColor, 3);
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            cout << transformation[i][j] << ",";
        }
        cout << "\n";
    }
    double color[3] = {80, 228, 211};

    double* newColor = tf.transformColor(color, transformation);
    for (int i=0; i<3; i++){
        cout << newColor[i] << endl;
    }

    return 0;
}