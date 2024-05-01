#include <vector>
#include <stdexcept>
#include <iostream>
#include "bsplineinter.h" // header file containing program for Nurbs interpolation
using namespace std;


int main() {
    double t;
    cout<< "Enter the degree"<<endl;
    int degree;
    cin>> degree;
    
    
    vector<vector<double>> results; // This will collect all the interpolated points
    vector<vector<double>> points(degree+1, vector<double>(2));
    cout << "Enter the points (x y format): ";
    for (int i = 0; i < degree+1; ++i) {
        cin >> points[i][0] >> points[i][1];
    }

    //Asks if you want a clamped or unclamped spline. 
    cout << "Please, if you want an unclamped Spline, the size of your knot vector will be " << degree + points.size() + 1
    << ". If you want a clamped spline, the size of your knot vector will still be " << degree + points.size() + 1
    << ", but the first " << degree + 1 << " values in the knot vector will be the same, like 0, 0, 0..., and your last "
    << degree + 1 << " values in the knot vector will be the same, like 2, 2, 2..."<< endl;

    //Entering the knots 
    int knot_size=degree + points.size() + 1;
    vector<double> knots(knot_size);
    cout << "Enter the knot values ";
    for(int i =0; i<knot_size; ++i){
        cin>> knots[i];
    }
    
    //Entering the weights 
    vector<double> weights(points.size());
    cout<<"Please Enter the weights"<<endl;
    for(int i=0; i<points.size(); ++i){
        cin>>weights[i];
    }
   
    for (t = 0; t <= 1; t += 0.01) {
        vector<double> plot = interpolate(t, degree, points, knots, weights);
        results.push_back(plot); // Store each result
    }
    

    
    // Example of how to print the results
    for(const auto& plot : results) {
        for(double val : plot) {
            cout << val << " ";
        }
        cout << endl;
    }

    // Use system("pause") to pause the console before the program terminates
    system("pause");
    return 0;
}