#include <vector>
#include <cmath>

using namespace std;

std::vector<std::vector<int>> A = {
    {1},                            // n=0
    {1, 1},                         // n=1
    {1, 2, 1},                      // n=2
    {1, 3, 3, 1},                   // n=3
    {1, 4, 6, 4, 1},                // n=4
    {1, 5, 10, 10, 5, 1},           // n=5
    {1, 6, 15, 20, 15, 6, 1}        // n=6
};
int binomial(int n, int k) {
    while (n >= A.size()) {
        int s = A.size();
        std::vector<int> nextRow(s + 1, 0);
        nextRow[0] = 1;
        for (int i = 1, prev = s - 1; i < s; ++i) {
            nextRow[i] = A[prev][i - 1] + A[prev][i];
        }
        nextRow[s] = 1;
        A.push_back(nextRow);
    }
    return A[n][k];
}

// Function to compute Bezier curve value with weights containing arrays of vectors
pair<double, double> Bezier(int n, double t, const vector<vector<double>>& w) {
    double sumx = 0, sumy = 0;
    for (int k = 0; k <= n; ++k) {
        sumx += w[k][0] * binomial(n, k) * pow(1 - t, n - k) * pow(t, k);
        sumy += w[k][1] * binomial(n, k) * pow(1 - t, n - k) * pow(t, k);
    }
    return {sumx, sumy};
}


//rational bezier
// Function to compute Rational Bezier curve value
pair<double, double> RationalBezier(int n, double t, const vector<vector<double>>& P, const vector<double>& weights) {
    double sumx = 0, sumy = 0, sumw = 0;
    for (int k = 0; k <= n; ++k) {
        double blend = binomial(n, k) * pow(1 - t, n - k) * pow(t, k);
        sumx += P[k][0] * weights[k] * blend;
        sumy += P[k][1] * weights[k] * blend;
        sumw += weights[k] * blend;
    }
    // Normalize by sum of weights
    return {sumx / sumw, sumy / sumw};
}