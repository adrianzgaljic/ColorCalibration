//
// Created by Adrian Žgaljić on 08/11/2019.
//

#include <iostream>

using namespace std;

/*
 * params = [rows][B G R B'] || [rows][B G R G'] || [rows][B G R R']
 */
double* getCoefficients(double originalColor[][3], double trueColor[][3], int rows, int color, int coeffRow){
    double a = 0;
    double b = 0;
    double c = 0;
    double d = 0;

    //blue
    if (color == 0) {
        for (int i = 0; i < rows; i++) {
            a += 2 * originalColor[i][0] * originalColor[i][0];
            b += 2 * originalColor[i][0] * originalColor[i][1];
            c += 2 * originalColor[i][0] * originalColor[i][2];
            d += 2 * originalColor[i][0] * trueColor[i][coeffRow];
        }
    //green
    } else if (color == 1){
        for (int i = 0; i < rows; i++) {
            a += 2 * originalColor[i][0] * originalColor[i][1];
            b += 2 * originalColor[i][1] * originalColor[i][1];
            c += 2 * originalColor[i][1] * originalColor[i][2];
            d += 2 * originalColor[i][1] * trueColor[i][coeffRow];
        }
    //red
    } else if (color == 2){
        for (int i = 0; i < rows; i++) {
            a += 2 * originalColor[i][0] * originalColor[i][2];
            b += 2 * originalColor[i][1] * originalColor[i][2];
            c += 2 * originalColor[i][2] * originalColor[i][2];
            d += 2 * originalColor[i][2] * trueColor[i][coeffRow];
        }
    }
    double* result = new double[4]{a, b, c, d};

    return result;
}

void findMinimum(double params[][4], int rows){

    for (int i=0; i<rows; i++){
        for (int j=0; j<4; j++){
            cout << params[i][j] << endl;
        }
    }

}

double* findMatrixInverse(double mat[3][3]){
    int i, j;

    //finding determinant
    float determinant = 0;

    for(i = 0; i < 3; i++)
        determinant = determinant + (mat[0][i] * (mat[1][(i+1)%3] * mat[2][(i+2)%3] - mat[1][(i+2)%3] * mat[2][(i+1)%3]));


    double result[3][3];
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++)
            result[i][j] = ((mat[(j+1)%3][(i+1)%3] * mat[(j+2)%3][(i+2)%3]) - (mat[(j+1)%3][(i+2)%3] * mat[(j+2)%3][(i+1)%3]))/ determinant;
    }


    return reinterpret_cast<double *>(result);
}

double* transformColor(double color[3], double transformationMatrix[3][3]){
    double newColor[3] = {0, 0, 0};
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            newColor[i] += color[j]*transformationMatrix[i][j];
        }
    }
    return newColor;
}

double* getTransformationMatrix(){
    double transformationMatrix[3][3];
    return reinterpret_cast<double *>(transformationMatrix);
}


int main(){
    cout << "bok" << endl;

    double originalColor[3][3] = {{112, 115, 131},
                           {111, 51, 202},
                           {80, 228, 211}};
    double trueColor[3][3] = {{196, 165, 113},
                              {196, 38, 210},
                              {169, 242, 221}};




    double transformationMatrix[3][3];
    for (int coeffRow=0; coeffRow<3; coeffRow++){
        double* coeffsBlue = getCoefficients(originalColor, trueColor, 3, 0, coeffRow);
        double* coeffsGreen = getCoefficients(originalColor, trueColor,3, 1, coeffRow);
        double* coeffsRed = getCoefficients(originalColor, trueColor, 3, 2, coeffRow);

        double mat[3][3] = {{coeffsBlue[0], coeffsBlue[1], coeffsBlue[2]},{coeffsGreen[0], coeffsGreen[1], coeffsGreen[2]},{coeffsRed[0], coeffsRed[1], coeffsRed[2]}};
        double* result = findMatrixInverse(mat);
        double newBlue = coeffsBlue[3]*result[0] + coeffsGreen[3]*result[3] + coeffsRed[3]*result[6];
        double newGreen = coeffsBlue[3]*result[1] + coeffsGreen[3]*result[4] + coeffsRed[3]*result[7];
        double newRed = coeffsBlue[3]*result[2] + coeffsGreen[3]*result[5] + coeffsRed[3]*result[8];
        transformationMatrix[coeffRow][0] = newBlue;
        transformationMatrix[coeffRow][1] = newGreen;
        transformationMatrix[coeffRow][2] = newRed;
    }

    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            cout << transformationMatrix[i][j] << ",";
        }
        cout << "\n";
    }
    double color[3] = {112, 115, 131};
    double* newColor = transformColor(color, transformationMatrix);



    return 0;
}