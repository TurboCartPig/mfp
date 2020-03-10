#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

int main() {
    MatrixXd m(2, 2);
    m(0, 0) = 1.0f;
    m(0, 1) = 0.0f;
    m(1, 0) = 0.0f;
    m(1, 1) = 1.0f;

    cout << m << endl;

    return EXIT_SUCCESS;
}