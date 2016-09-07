#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::EigenSolver;
using namespace std;

int main(){
    MatrixXd A = MatrixXd::Random(6,6);
    cout << "Here is a random 6x6 matrix, A:" << endl << A << endl << endl;
    EigenSolver<MatrixXd> es(A);
    cout << "The eigenvalues of A are:" << endl << es.eigenvalues() << endl;
}
