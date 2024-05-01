#include <iostream>
#include <fstream> 
#include <algorithm>
#include <vector>
#include <cmath>
#include "bsplineinter.h" // header file containing program for Nurbs interpolation
#include "bezier.h"
#include "cspline.h"
using namespace std;

// clamped and clamped
void clamped_unclamped() {
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
    
}

//Knot refinement and insertion
void knot_insert_refine(){
    cout<< "Enter the degree"<<endl;
    int degree;
    cin>> degree;
    
    vector<vector<double>> points(degree+1, vector<double>(2));
    cout << "Enter the points (x y format): ";
    for (int i = 0; i < degree+1; ++i) {
        cin >> points[i][0] >> points[i][1];
    }
    vector<vector<double>> points_q; // Empty vector to store new control points

    //Enter Original knots
    int knot_size=degree + points.size() + 1;
    vector<double> knots(knot_size);
    cout << "Enter the knot values ";
    for(int i =0; i<knot_size; ++i){
        cin>> knots[i];
    }
    
    cout<<"Here you are going to enter knot values you want to insert."
        <<"For knot insertion you enter one value or one value multiple times."
        <<"For knot refinement, you enter any values as much as you want."
        <<"Please make sure your new knot values revolve somewhere around the middle"
        <<" of the existing values so the code does not go out of bounds"  <<endl;
    //Enter insert knots
    int insknot_size;
    cout<<"Enter the size/number of knots you want to insert"<<endl;
    cin>> insknot_size;// Asks for the size of the insert knot vector
    vector<double> insertknots(insknot_size);
    cout << "Enter the insertknot values ";
    for(int i =0; i<insknot_size; ++i){
        cin>> insertknots[i];
    }
    
    // Find the span
    for (const auto& insertknot : insertknots) {
       
        int f;
        for (f = 0; f < knots.size() - 1; f++) {
            if (insertknot >= knots[f] && insertknot < knots[f + 1]) {
                double start = max(0, f - degree + 1); // Adjust starting index to avoid out-of-bounds
                points_q.reserve(points.size() + insertknots.size()); // Reserve memory for new control points
                // Copy control points before insertion span
                for (int i = 0; i <start; i++) {
                    points_q.push_back(points[i]);
                }
                // Calculate new control points within insertion span
                for (double g = start; g <= f; g++) {
                    double alpha = (insertknot - knots[g]) / (knots[g + degree] - knots[g]);
                    vector<double> new_point(2);
                    for (int dim = 0; dim < 2; dim++) {
                        new_point[dim] = (1 - alpha) * points[g - 1][dim] + alpha * points[g][dim];
                    }
                    points_q.push_back(new_point);
                }
                // Copy control points after insertion span
                for (double i = f; i < points.size(); i++) {
                    points_q.push_back(points[i]);
                }
                break;
            }
        }
        // Update points for next iteration
        points = points_q;
        points_q.clear(); // Clear points_q for next iteration
    }

    // Print new control points
    cout << "New control points:" << endl;
    for (const auto& p : points) {
        cout << "(" << p[0] << ", " << p[1] << ") ";
    }
    cout << endl;
    
    vector<double> updated_knots = knots;         // Copy of original knots vector
    // Loop through each knot in insertknots to insert into updated_knots
    for (const double& knot : insertknots) {
        auto it = upper_bound(updated_knots.begin(), updated_knots.end(), knot); // Find the position to insert the new knot
        updated_knots.insert(it, knot); // Insert the new knot
    }

    // Output the updated knots vector
    cout << "Updated knots vector:" << endl;
    for (const auto& k : updated_knots) {
        cout << k << " ";
    }
    cout << endl;

    // Use system("pause") to pause the console before the program terminates
    system("pause");
}

//General Cylinder
void General_Cylinder(){
    double t;
    cout<< "Enter the degree"<<endl;
    int degree;
    cin>> degree;
    
    ofstream outFile("General Cylinder.txt");
    vector<vector<double>> results; // This will collect all the interpolated points
    /*vector<vector<double>> points = {
             {1,0,0}, {1,1,0}, {0,1,0}, {-1,1,0}, {-1,0,0}, {-1,-1,0}, {0,-1,0}, {1,-1,0}, {1,0,0}

    };*/
    cout<<"Enter the number of points you want to use"<<endl;
    int point_size;
    cin>> point_size;
    vector<vector<double>> points(point_size, vector<double>(3));
    cout << "Enter the points (x y z format): ";
    for (int i = 0; i < degree+1; ++i) {
        cin >> points[i][0] >> points[i][1] >> points[i][2] ;
    }
    
    //Asks if you want a clamped or unclamped spline. 
    cout << "Please, if you want an unclamped Spline, the size of your knot vector will be " << degree + points.size() + 1
    << ". If you want a clamped spline, the size of your knot vector will still be " << degree + points.size() + 1
    << ", but the first " << degree + 1 << " values in the knot vector will be the same, like 0, 0, 0..., and your last "
    << degree + 1 << " values in the knot vector will be the same, like 2, 2, 2...";

  
    //Entering the knots 
    int knot_size=degree + points.size() + 1;
    vector<double> knots(knot_size);
    cout << "Enter the knot values ";
    for(int i =0; i<knot_size; ++i){
        cin>> knots[i];
    }

    
    vector<double> weights(points.size());
    cout<<"Please Enter the weights"<<endl;
    for(int i=0; i<points.size(); ++i){
        cin>>weights[i];
    }

    cout<<"Enter the height of the extrusion"<< endl;
    double w;
    cin>> w;

    //surface is generated by a simple translation and interpolation
    for (double w=0; w<=5; w+=0.1){
        for (t = 0; t <= 1; t += 0.01) {
        vector<double> plot = interpolate(t, degree,points, knots, weights);
        plot[2] += w;
        results.push_back(plot); // Store each result

        // Write the point to the file
            if (outFile) {
                outFile << "(" << plot[0] << ", " << plot[1] << ", " << plot[2] << ")" << endl;
        }
    }
    }
    


}

//Ruled_Surface
void Ruled_Surface(){
    double t;
    int degree = 2;
    ofstream outFile1("curve1.txt");
    ofstream outFile2("curve2.txt");
    vector<vector<double>> results1; // This will collect all the interpolated points
    vector<vector<double>> results2; // This will collect all the interpolated points
    cout<<"Enter the number of points you want to use"<<endl;
    int point_size;
    cin>> point_size;
    vector<vector<double>> points1(point_size, vector<double>(3));
    cout << "Enter the points (x y z format): ";
    for (int i = 0; i < degree+1; ++i) {
        cin >> points1[i][0] >> points1[i][1] >> points1[i][2] ;
    }
    
    vector<vector<double>> points2(point_size, vector<double>(3));
    cout << "Enter the points (x y z format): ";
    for (int i = 0; i < degree+1; ++i) {
        cin >> points2[i][0] >> points2[i][1] >> points2[i][2] ;
    }

      //Entering the knots 
    int knot_size=degree + points1.size() + 1;
    vector<double> knots(knot_size);
    cout << "Enter the knot values ";
    for(int i =0; i<knot_size; ++i){
        cin>> knots[i];
    }

    vector<double> weights(points1.size());
    cout<<"Please Enter the weights"<<endl;
    for(int i=0; i<points1.size(); ++i){
        cin>>weights[i];
    }


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
        vector<double> plot2 = interpolate(t, degree, points2, knots, weights);
        results2.push_back(plot2); // Store each result
        // Write the point to the file
            if (outFile2) {
                outFile2 << "(" << plot2[0] << ", " << plot2[1] << ", " << plot2[2] << ")" << endl;

        
        }
    }

}

void Bezier(){
    int n; // degree of the Bezier curve
    vector<vector<double>> w; // control points
    const int num_points = 100;
    vector<double> points;
    vector<pair<double, double>> results;

    // Prompt user for the degree of the Bezier curve
    cout << "Enter the degree of the Bezier curve: ";
    cin >> n;

    // Prompt user for control points
    cout << "Enter the control points for the Bezier curve (each point should have two coordinates):\n";
    for (int i = 0; i <= n; ++i) {
        double x, y;
        cout << "Control Point " << i + 1 << ": ";
        cin >> x >> y;
        w.push_back({x, y});
    }

    // Generate points from 0 to 1
    for (int i = 0; i < num_points; ++i) {
        double value = static_cast<double>(i) / (num_points - 1);
        points.push_back(value);
    }

    // Evaluate Bezier function at each point and store the results
    for (int i = 0; i < num_points; ++i) {
        pair<double, double> result = Bezier(n, points[i], w);
        results.push_back(result);
    }

// Write the results to a text file
    ofstream outputFile("bazier_output.txt");
    if (outputFile.is_open()) {
        for (int i = 0; i < num_points; ++i) {
            outputFile << results[i].first << " " << results[i].second << endl;
        }
        outputFile.close();
        cout << "Results have been written to 'bazier_output.txt'." << endl;
    } else {
        cout << "Unable to open file for writing." << endl;
    }

    // Hold terminal
    cin.ignore();
    cin.get();
    
}

//Rational bezier
void RationalBezier(){
    int n; // degree of the Bezier curve
    vector<vector<double>> P; // control points
    vector<double> weights; // weights of control points
    const int num_points = 100;
    vector<double> points;
    vector<pair<double, double>> results;

    // Prompt user for the degree of the Bezier curve
    cout << "Enter the degree of the Bezier curve: ";
    cin >> n;

    // Prompt user for control points and weights
    cout << "Enter the control points and weights for the Bezier curve (each point should have two coordinates):\n";
    for (int i = 0; i <= n; ++i) {
        double x, y, w;
        cout << "Control Point " << i + 1 << ": ";
        cin >> x >> y;
        P.push_back({x, y});
        cout << "Weight for Control Point " << i + 1 << ": ";
        cin >> w;
        weights.push_back(w);
    }

    // Generate points from 0 to 1
    for (int i = 0; i < num_points; ++i) {
        double value = static_cast<double>(i) / (num_points - 1);
        points.push_back(value);
    }

    // Evaluate Rational Bezier function at each point and store the results
    for (int i = 0; i < num_points; ++i) {
        pair<double, double> result = RationalBezier(n, points[i], P, weights);
        results.push_back(result);
    }

// Write the results to a text file
    ofstream outputFile("rat_bazier_output.txt");
    if (outputFile.is_open()) {
        for (int i = 0; i < num_points; ++i) {
            outputFile << results[i].first << " " << results[i].second << endl;
        }
        outputFile.close();
        cout << "Results have been written to 'rat_bazier_output.txt'." << endl;
    } else {
        cout << "Unable to open file for writing." << endl;
    }
    
    // Hold terminal
    cin.ignore();
    cin.get();
}

//Cspline
void CSpline(){
    std::vector<Point> points;
    double x, y;

    std::cout << "Enter data points as x y pairs (type a non-numeric character to end):\n";
    while (std::cin >> x >> y) {
        points.push_back({x, y});
    }

    CubicSplineInterpolation interpolator(points);

    std::ofstream outputFile("output.txt");
    if (outputFile.is_open()) {
        //1 2
        outputFile << "Interpolated points:\n";
        const int NPOINTS = 100; // Number of points for plotting
        //outputFile << "x, y\n";
        for (int i = 0; i < NPOINTS; ++i) {
            double xi = points.front().x + (points.back().x - points.front().x) * i / (NPOINTS - 1);
            double yi = interpolator.interpolate(xi);
            outputFile << xi << ", " << yi << "\n";
        }
        outputFile.close();
        std::cout << "Interpolated points are written to output.txt\n";
    } else {
        std::cerr << "Unable to open output.txt\n";
    }

}