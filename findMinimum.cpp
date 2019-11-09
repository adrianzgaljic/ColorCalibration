//
// Created by Adrian Žgaljić on 08/11/2019.
//

#include <iostream>

using namespace std;

/*
 * params = [rows][R G B R'] || [rows][R G B G'] || [rows][R G B B']
 */
double* getCoefficients(double params[][4], int rows, int color){
    double a = 0;
    double b = 0;
    double c = 0;
    double d = 0;

    //blue
    if (color == 0) {
        for (int i = 0; i < rows; i++) {
            a += 2 * params[i][0] * params[i][0];
            b += 2 * params[i][0] * params[i][1];
            c += 2 * params[i][0] * params[i][2];
            d -= 2 * params[i][0] * params[i][3];
        }
    //green
    } else if (color == 1){
        for (int i = 0; i < rows; i++) {
            a += 2 * params[i][0] * params[i][1];
            b += 2 * params[i][1] * params[i][1];
            c += 2 * params[i][1] * params[i][2];
            d -= 2 * params[i][1] * params[i][3];
        }
    //red
    } else if (color == 2){
        for (int i = 0; i < rows; i++) {
            a += 2 * params[i][0] * params[i][2];
            b += 2 * params[i][1] * params[i][2];
            c += 2 * params[i][2] * params[i][2];
            d -= 2 * params[i][2] * params[i][3];
        }
    }
    double* result = new double[4]{a, b, c, d};

    cout << result << endl;
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
    printf("\nGiven matrix is:");
    int i, j;
    for(i = 0; i < 3; i++){
        cout<<"\n";

        for(j = 0; j < 3; j++)
            cout<<mat[i][j]<<"\t";
    }

    //finding determinant
    float determinant = 0;

    for(i = 0; i < 3; i++)
        determinant = determinant + (mat[0][i] * (mat[1][(i+1)%3] * mat[2][(i+2)%3] - mat[1][(i+2)%3] * mat[2][(i+1)%3]));

    cout<<"\n\ndeterminant: "<< determinant;

    double result[3][3];
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++)
            result[i][j] = ((mat[(j+1)%3][(i+1)%3] * mat[(j+2)%3][(i+2)%3]) - (mat[(j+1)%3][(i+2)%3] * mat[(j+2)%3][(i+1)%3]))/ determinant;
    }


    return reinterpret_cast<double *>(result);
}



int main(){
    cout << "bok" << endl;

    double paramsBlue[3][4] = {{112.792, 155.086, 131.88, 196.337},
                           {111.044, 51.0147, 202.492, 196.265},
                           {80.2594, 228.866, 211.277, 169.048}};
    double paramsGreen[3][4] = {{112.792, 155.086, 131.88, 165.561},
                               {111.044, 51.0147, 202.492, 38.9964},
                               {80.2594, 228.866, 211.277, 242.327}};
    double paramsRed[3][4] = {{112.792, 155.086, 131.88, 113.95},
                               {111.044, 51.0147, 202.492, 210.759},
                               {80.2594, 228.866, 211.277, 221.563}};

    double* coeffsBlue = getCoefficients(paramsBlue, 3, 0);
    double* coeffsGreen = getCoefficients(paramsGreen, 3, 1);
    double* coeffsRed = getCoefficients(paramsRed, 3, 2);

    double mat[3][3] = {{coeffsBlue[0], coeffsBlue[1], coeffsBlue[2]},{coeffsGreen[0], coeffsGreen[1], coeffsGreen[2]},{coeffsRed[0], coeffsRed[1], coeffsRed[2]}};
    double* result = findMatrixInverse(mat);
    double newBlue = -coeffsBlue[3]*result[0] - coeffsGreen[3]*result[3] - coeffsRed[3]*result[6];
    double newGreen = -coeffsBlue[3]*result[1] - coeffsGreen[3]*result[4] - coeffsRed[3]*result[7];
    double newRed = -coeffsBlue[3]*result[2] - coeffsGreen[3]*result[5] - coeffsRed[3]*result[8];
    cout << "result" << newBlue << "," << newGreen << "," << newRed << endl;

    return 0;
}