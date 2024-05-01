
#include <iostream>
#include <fstream>
#include <vector>

struct Point {
    double x;
    double y;
};

class CubicSplineInterpolation {
private:
    std::vector<Point> points;
    std::vector<double> coeffs;

public:
    CubicSplineInterpolation(const std::vector<Point>& inputPoints) : points(inputPoints) {
        computeCoefficients();
    }

    double interpolate(double x) const {
        size_t i = 0;
        while (i < points.size() && x > points[i].x) {
            ++i;
        }
        if (i == 0) {
            i = 1;
        } else if (i == points.size()) {
            --i;
        }

        const double h = points[i].x - points[i - 1].x;
        const double a = (points[i].x - x) / h;
        const double b = (x - points[i - 1].x) / h;
        const double interpolatedY =
            a * points[i - 1].y + b * points[i].y +
            ((a * a * a - a) * coeffs[i - 1] + (b * b * b - b) * coeffs[i]) * (h * h) / 6.0;

        return interpolatedY;
    }

private:
    void computeCoefficients() {
        const size_t n = points.size();
        if (n <= 1) {
            return; // No interpolation needed for 0 or 1 point
        }

        std::vector<double> h(n - 1);
        std::vector<double> alpha(n - 1);
        std::vector<double> l(n);
        std::vector<double> mu(n - 1);
        std::vector<double> z(n);

        for (size_t i = 0; i < n - 1; ++i) {
            h[i] = points[i + 1].x - points[i].x;
            alpha[i] = (3.0 / h[i]) * (points[i + 1].y - points[i].y) -
                       (3.0 / (i == 0 ? h[i] : h[i - 1])) * (points[i].y - (i == 0 ? 0.0 : points[i - 1].y));
        }

        l[0] = 1.0;
        mu[0] = 0.0;
        z[0] = 0.0;

        for (size_t i = 1; i < n - 1; ++i) {
            l[i] = 2 * (points[i + 1].x - points[i - 1].x) - h[i - 1] * mu[i - 1];
            mu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
        }

        l[n - 1] = 1.0;
        z[n - 1] = 0.0;
        coeffs.resize(n);

        for (int i = n - 2; i >= 0; --i) {
            coeffs[i] = z[i] - mu[i] * coeffs[i + 1];
        }
    }
};
