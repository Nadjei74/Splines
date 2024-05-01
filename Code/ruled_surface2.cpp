#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream> 
using namespace std;

vector<double> interpolate(double t, int degree, const vector<vector<double>>& points,vector<double> knots, const vector<double>& weights) {
    int i, j, s, l;
    int n = points.size(); // points count
    int d = points[0].size(); // point dimensionality

    //Here we check if the points and the degree tally
    if(degree < 1) throw invalid_argument("degree must be at least 1 (linear)");
    if(degree > (n - 1)) throw invalid_argument("degree must be less than or equal to point count - 1");

    /*This code checks if any values were given for weight(Nurbs), if not the weight vector
    is initialized to be 1's*/
    vector<double> w = weights;
    if(w.empty()) {
        w.resize(n, 1); // build weight vector of length [n]
    }
    /*
    This code checks if a knot vector is given, if not, it builds a knot vector
    from 0 to size of knot vector -1 
    */
    if(knots.empty()) {
        knots.resize(n + degree + 1);
        for(i = 0; i < n + degree + 1; i++) {
            knots[i] = i;
        } 
    } else {
        /*
        this checks if the knot vector size is correct
        */
        if(knots.size() != n + degree + 1) throw invalid_argument("bad knot vector length");
    }

    /*
    Here we declare the array domain to 2 values, degree and size of the knot-1-degree.
    this describes the domain over which the curves can be evaluated with the degree as minimum size of 
    control point as maximum
    */
    int domain[2] = {
        degree,
        static_cast<int>(knots.size()) - 1 - degree
    };

    // here we remap t to the domain where the spline is defined
    double low = knots[domain[0]];
    double high = knots[domain[1]];
    t = t * (high - low) + low;

    //this part check if t is out of bounds
    if(t < low || t > high) throw out_of_range("out of bounds");

    // find s(s is the index) (the spline segment) for the [t] value provided
    for(s = domain[0]; s < domain[1]; s++) {
        if(t >= knots[s] && t <= knots[s + 1]) {
            break;
        }
    }

    /*convert points to homogeneous coordinates
    we create an n(size of point) by d(dimension) matrix to store
    the points multiplied by weight.
     */
    vector<vector<double>> v(n,vector<double>(d + 1));
    for(i = 0; i < n; i++) {
        for(j = 0; j < d; j++) {
            v[i][j] = points[i][j] * w[i];
        }
        v[i][d] = w[i];
    }

    /*l (level) goes from 1 to the curve degree + 1
    This part of the code computes the Cox de boor's algo where we have an alpha used to
    store the interpolation factor based on the current knot in the knot vector.
    l is the variable that defines the step through the levels of the algo starting from 1 
    to degree+1

    */
    double alpha;
    for(l = 1; l <= degree + 1; l++) {
        for(i = s; i > s - degree - 1 + l; i--) {
            alpha = (t - knots[i]) / (knots[i + degree + 1 - l] - knots[i]);
            for(j = 0; j < d + 1; j++) {
                v[i][j] = (1 - alpha) * v[i - 1][j] + alpha * v[i][j];
            }
        }
    }

    // convert back to cartesian and return
    vector<double> result(d);
    for(i = 0; i < d; i++) {
        result[i] = v[s][i] / v[s][d];
    }

    return result;
}

int main() {
    double t;
    int degree = 2;
    ofstream outFile1("curve1.txt");
    ofstream outFile2("curve2.txt");
    vector<vector<double>> results1; // This will collect all the interpolated points
    vector<vector<double>> results2; // This will collect all the interpolated points
    vector<vector<double>> points1 = {
             {1,1,0}, {2,2,0},{3,1,0},{5,7,0},{6,2,0}

    };
    
    vector<vector<double>> points2 = {
        {2,1,5}, {1,2,2}, {4,1,3},{5,7,5}, {1,2,5}
    };

    //Entering the 
    vector<double> knots = {
        0, 0, 0,0.25,0.5,1,1,1
    };

    vector<double> knots2 = {
        0, 0, 0,0.35,0.5,1,1,1
    };
    vector<double> weights = {
        1, 0.701,1,0.707,1,0.707,1,0.707,1
    };

    //First curve
    for (t = 0; t <= 1; t += 0.01) {
        vector<double> plot1 = interpolate(t, degree, points1, knots, weights);
        results1.push_back(plot1); // Store each result
        // Write the point to the file
            if (outFile1) {
               outFile1 << "(" << plot1[0] << ", " << plot1[1] << ", " << plot1[2] << ")" << endl;

    }
    }

    //second curve
    for (t = 0; t <= 1; t += 0.01) {
        vector<double> plot2 = interpolate(t, degree, points2, knots2, weights);
        results2.push_back(plot2); // Store each result
        // Write the point to the file
            if (outFile2) {
                outFile2 << "(" << plot2[0] << ", " << plot2[1] << ", " << plot2[2] << ")" << endl;

        
        }
    };

    
    

    /* Example of how to print the results
    for(const auto& plot : results) {
        for(double val : plot) {
            cout << val << " ";
        }
        cout << endl;
    }*/



    return 0;
}
