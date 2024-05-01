#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>

struct Point {
    double x;
    double y;
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

    // Cox-de Boor recursion formula for NURBS
    Point calculateNURBSPoint(double t) {
        Point result = {0, 0, 0};
        size_t n = controlPoints.size() - 1;
        for (size_t i = 0; i <= n; ++i) {
            double basis = Nip(i, degree, knotVector, t);
            result.x += controlPoints[i].x * controlPoints[i].weight * basis;
            result.y += controlPoints[i].y * controlPoints[i].weight * basis;
            result.weight += controlPoints[i].weight * basis;
        }
        result.x /= result.weight;
        result.y /= result.weight;
        return result;
    }

    // Cox-de Boor recursion formula for NURBS basis functions
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
            outFile << p.x << " " << p.y << std::endl;
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

// Function to generate random control points
std::vector<Point> generateRandomControlPoints(int numPoints) {
    std::vector<Point> controlPoints;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-5.0, 5.0); // Random values between -5 and 5

    for (int i = 0; i < numPoints; ++i) {
        double x, y, weight;
        std::cout << "Enter x and y coordinates for control point " << i + 1 << ": ";
        std::cin >> x >> y;
        std::cout << "Enter weight for control point " << i + 1 << ": ";
        std::cin >> weight;
        Point p = {x, y, weight};
        controlPoints.push_back(p);
    }

    return controlPoints;
}

int main() {
    int degree;
    std::cout << "Enter the degree of the NURBS curve: ";
    std::cin >> degree;

    int numControlPoints;
    std::cout << "Enter the number of control points: ";
    std::cin >> numControlPoints;

    std::vector<Point> controlPoints = generateRandomControlPoints(numControlPoints);

    std::vector<double> knotVector(numControlPoints + degree + 1);
    std::cout << "Enter knot vector values:" << std::endl;
    for (int i = 0; i < numControlPoints + degree + 1; ++i) {
        std::cout << "Knot value " << i + 1 << ": ";
        std::cin >> knotVector[i];
    }

    double stepSize;
    std::cout << "Enter step size for plotting the curve: ";
    std::cin >> stepSize;

    // Create a NURBS object for the curve
    NURBS nurbsCurve(degree, knotVector, controlPoints, stepSize);

    // Plot the NURBS curve and write points to a file
    nurbsCurve.plotNURBSCurve("nurbs_curve.txt");

    // Generate grid points along the NURBS curve
    int numGridPoints;
    std::cout << "Enter the number of grid points: ";
    std::cin >> numGridPoints;

    std::vector<Point> gridPoints = nurbsCurve.generateGridPoints(numGridPoints);

    // Revolve the grid points around the z-axis to create a 3D surface
    std::ofstream surfaceFile("nurbs_surface.txt");
    double angleStep = 0.1; // Angle step for revolving
    double angle = 0.0;

    while (angle <= 2 * M_PI) {
        for (const auto& p : gridPoints) {
            double x = p.x * cos(angle);
            double y = p.x * sin(angle);
            double z = p.y;
            surfaceFile << x << " " << y << " " << z << std::endl;
        }
        angle += angleStep;
    }
    surfaceFile.close();

    return 0;
}
