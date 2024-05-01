#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>

struct Point {
    double x;
    double y;
    double z;
    double weight;
};

class NURBS {
private:
    std::vector<Point> controlPoints;
    std::vector<double> knotVector;
    int degree;
    double stepSize;

public:
    NURBS(int deg, const std::vector<double>& knots, const std::vector<Point>& points, double step)
        : degree(deg), knotVector(knots), controlPoints(points), stepSize(step) {}

    Point calculateNURBSPoint(double t) {
        Point result = {0, 0, 0, 0};
        size_t n = controlPoints.size() - 1;
        for (size_t i = 0; i <= n; ++i) {
            double basis = Nip(i, degree, knotVector, t);
            result.x += controlPoints[i].x * controlPoints[i].weight * basis;
            result.y += controlPoints[i].y * controlPoints[i].weight * basis;
            result.z += controlPoints[i].z * controlPoints[i].weight * basis;
            result.weight += controlPoints[i].weight * basis;
        }
        result.x /= result.weight;
        result.y /= result.weight;
        result.z /= result.weight;
        return result;
    }

    double Nip(size_t i, int p, const std::vector<double>& U, double u) {
        if (p == 0) {
            return (U[i] <= u && u < U[i + 1]) ? 1.0 : 0.0;
        } else {
            double left = (U[i + p] - U[i] != 0) ? (u - U[i]) / (U[i + p] - U[i]) : 0.0;
            double right = (U[i + p + 1] - U[i + 1] != 0) ? (U[i + p + 1] - u) / (U[i + p + 1] - U[i + 1]) : 0.0;
            return left * Nip(i, p - 1, U, u) + right * Nip(i + 1, p - 1, U, u);
        }
    }

    void plotNURBSCurve(const std::string& filename) {
        std::ofstream outFile(filename);
        for (double t = knotVector[degree]; t <= knotVector[controlPoints.size()]; t += stepSize) {
            Point p = calculateNURBSPoint(t);
            outFile << p.x << " " << p.y << " " << p.z << std::endl;
        }
        outFile.close();
    }

    std::vector<Point> generateGridPoints(int numPoints) {
        std::vector<Point> gridPoints;
        double spacing = (knotVector[controlPoints.size()] - knotVector[degree]) / (numPoints - 1);
        for (double t = knotVector[degree]; t <= knotVector[controlPoints.size()]; t += spacing) {
            Point p = calculateNURBSPoint(t);
            gridPoints.push_back(p);
        }
        return gridPoints;
    }
};

int main() {
    // Input for initial NURBS curve
    int degree_initial;
    std::cout << "Enter the degree of the initial NURBS curve: ";
    std::cin >> degree_initial;

    int numControlPoints_initial;
    std::cout << "Enter the number of control points for the initial curve: ";
    std::cin >> numControlPoints_initial;

    std::vector<Point> controlPoints_initial;
    for (int i = 0; i < numControlPoints_initial; ++i) {
        double x, y, z;
        std::cout << "Enter x, y, and z coordinates for control point " << i + 1 << " of the initial curve: ";
        std::cin >> x >> y >> z;
        double weight;
        std::cout << "Enter weight for control point " << i + 1 << " of the initial curve: ";
        std::cin >> weight;
        Point p = {x, y, z, weight};
        controlPoints_initial.push_back(p);
    }

    std::vector<double> knotVector_initial(numControlPoints_initial + degree_initial + 1);
    std::cout << "Enter knot vector values for the initial curve:" << std::endl;
    for (int i = 0; i < numControlPoints_initial + degree_initial + 1; ++i) {
        std::cout << "Knot value " << i + 1 << " for the initial curve: ";
        std::cin >> knotVector_initial[i];
    }

    double stepSize_initial;
    std::cout << "Enter step size for plotting the initial curve: ";
    std::cin >> stepSize_initial;

    // Create a NURBS object for the initial curve
    NURBS nurbsCurveInitial(degree_initial, knotVector_initial, controlPoints_initial, stepSize_initial);

    // Plot the initial NURBS curve and write points to a file
    nurbsCurveInitial.plotNURBSCurve("nurbs_curve_initial.txt");

    // Input for rotation angle
    double rotationAngle;
    std::cout << "Enter the rotation angle in radians: ";
    std::cin >> rotationAngle;

    // Rotate the curve
    std::ofstream rotatedCurveFile("nurbs_curve_rotated.txt");
    for (double t = knotVector_initial[degree_initial]; t <= knotVector_initial[numControlPoints_initial]; t += stepSize_initial) {
        Point p = nurbsCurveInitial.calculateNURBSPoint(t);
        // Rotate the point around the Z-axis
        double x_rotated = p.x * cos(rotationAngle) - p.y * sin(rotationAngle);
        double y_rotated = p.x * sin(rotationAngle) + p.y * cos(rotationAngle);
        double z_rotated = p.z;
        rotatedCurveFile << x_rotated << " " << y_rotated << " " << z_rotated << std::endl;
    }
    rotatedCurveFile.close();

    std::cout << "Rotated curve data saved to nurbs_curve_rotated.txt" << std::endl;

    return 0;
}
