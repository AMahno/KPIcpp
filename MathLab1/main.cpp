#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::EigenSolver;
using namespace std;

int main(){
    cout << "Enter the matrix size, one number: ";
    int aSize = 1; //in case cin fails
    cin >> aSize;
    cout << "Enter the matrix:" << endl;
    MatrixXd A = MatrixXd(aSize,aSize);
    A.setZero();
    for(int i = 0; i < aSize; i++){
        for(int j = 0; j < aSize; j++){
            cout << "Enter the member (" << i << ", " << j << "): ";
            cin >>  A(i, j);
        }
    }
    cout << "\nHere is a matrix A:" << endl << A << endl << endl;
    EigenSolver<MatrixXd> es(A);
    cout << "The eigenvalues of A are:" << endl << es.eigenvalues() << endl;
}
