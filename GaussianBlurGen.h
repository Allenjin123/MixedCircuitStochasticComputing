// C++ program to generate Gaussian filter
#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

// Function to create Gaussian filter
void FilterCreation(float GKernel[][3], double sigma = 1.0)
{
    // initialising standard deviation to 1.0
    double r, s = 2.0 * sigma * sigma;

    // sum is for normalization
    double sum = 0.0;

    // generating 5x5 kernel
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            r = sqrt(x * x + y * y);
            GKernel[x + 1][y + 1] = (exp(-(r * r) / s)) / (M_PI * s);
            sum += GKernel[x + 1][y + 1];
        }
    }

    // normalising the Kernel
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            GKernel[i][j] /= sum;
}

// Driver program to test above function
