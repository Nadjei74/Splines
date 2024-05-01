#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
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

    return 0;
}