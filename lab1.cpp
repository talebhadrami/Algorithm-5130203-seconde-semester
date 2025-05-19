#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

const double PI = 3.14159265358979323846;

// =========================== Ellipse Class =============================
class Ellipse {
private:
    double h, k;   // center coordinates
    double a, b;   // semi-major and semi-minor axes
    bool majorAxisX; // true if major axis along X, else along Y

    // Helper function to swap a and b if a < b (major > minor)
    void enforceAxisLengths() {
        if (a < b) {
            swap(a, b);
            majorAxisX = !majorAxisX;
        }
    }

public:
    // Constructors
    Ellipse() : h(0), k(0), a(1), b(1), majorAxisX(true) {
        // Default ellipse at origin, circle (a=b=1)
    }

    Ellipse(double h_, double k_, double a_, double b_, bool majorAxisX_ = true) 
        : h(h_), k(k_), a(a_), b(b_), majorAxisX(majorAxisX_) {
        enforceAxisLengths();
    }

    Ellipse(const Ellipse& other) {
        h = other.h;
        k = other.k;
        a = other.a;
        b = other.b;
        majorAxisX = other.majorAxisX;
    }

    // Setters
    void setCenter(double h_, double k_) { h = h_; k = k_; }
    void setAxes(double a_, double b_, bool majorAxisX_ = true) { 
        a = a_; b = b_; majorAxisX = majorAxisX_;
        enforceAxisLengths();
    }

    // Getters
    void getCenter(double &h_, double &k_) const { h_ = h; k_ = k; }
    void getAxes(double &a_, double &b_) const { a_ = a; b_ = b; }
    bool isMajorAxisX() const { return majorAxisX; }

    // Calculate hyperparameter c, c^2 = a^2 - b^2
    double getC() const {
        double val = a*a - b*b;
        return (val >= 0) ? sqrt(val) : 0;
    }

    // Vertices: 2 points
    void getVertices(pair<double,double> &V1, pair<double,double> &V2) const {
        if (majorAxisX) {
            V1 = {h - a, k};
            V2 = {h + a, k};
        } else {
            V1 = {h, k - a};
            V2 = {h, k + a};
        }
    }

    // Foci: 2 points
    void getFoci(pair<double,double> &F1, pair<double,double> &F2) const {
        double c = getC();
        if (majorAxisX) {
            F1 = {h - c, k};
            F2 = {h + c, k};
        } else {
            F1 = {h, k - c};
            F2 = {h, k + c};
        }
    }

    // Focal chord length: LR = 2b^2 / a
    double getFocalChordLength() const {
        return 2.0 * (b*b) / a;
    }

    // Eccentricity e = c / a
    double getEccentricity() const {
        double c = getC();
        return c / a;
    }

    // Print general ellipse equation and key points
    void print() const {
        cout << fixed << setprecision(4);
        cout << "Ellipse centered at (" << h << ", " << k << ")\n";
        cout << "Semi-major axis a = " << a << ", Semi-minor axis b = " << b 
             << (majorAxisX ? " (major axis along X)" : " (major axis along Y)") << "\n";

        // Equation:
        if (majorAxisX) {
            cout << "Equation: ((x - " << h << ")^2) / (" << a*a << ") + ((y - " << k << ")^2) / (" << b*b << ") = 1\n";
        } else {
            cout << "Equation: ((x - " << h << ")^2) / (" << b*b << ") + ((y - " << k << ")^2) / (" << a*a << ") = 1\n";
        }

        pair<double,double> V1,V2,F1,F2;
        getVertices(V1, V2);
        getFoci(F1, F2);

        cout << "Vertices: V1(" << V1.first << ", " << V1.second << "), V2(" << V2.first << ", " << V2.second << ")\n";
        cout << "Foci: F1(" << F1.first << ", " << F1.second << "), F2(" << F2.first << ", " << F2.second << ")\n";

        cout << "Focal chord length LR = " << getFocalChordLength() << "\n";
        cout << "Eccentricity e = " << getEccentricity() << "\n";

        cout << "Approximate perimeter P ≈ " << approximatePerimeter() << "\n";
        cout << "Area S = " << area() << "\n";
    }

    // Check point P(x,y) relation to ellipse: inside, on, or outside
    // Returns: -1 = inside, 0 = on ellipse, +1 = outside
    int pointRelation(double x, double y) const {
        double lhs;
        if (majorAxisX) {
            lhs = ((x - h)*(x - h)) / (a*a) + ((y - k)*(y - k)) / (b*b);
        } else {
            lhs = ((x - h)*(x - h)) / (b*b) + ((y - k)*(y - k)) / (a*a);
        }

        if (fabs(lhs - 1.0) < 1e-10) return 0;     // on ellipse
        else if (lhs < 1.0) return -1;             // inside
        else return 1;                             // outside
    }

    // Approximate perimeter using formula:
    // P ≈ π * [3(a+b) - sqrt((3a + b)(a + 3b))]
    double approximatePerimeter() const {
        return PI * (3*(a + b) - sqrt((3*a + b)*(a + 3*b)));
    }

    // Area = π * a * b
    double area() const {
        return PI * a * b;
    }

    // Given one coordinate (x or y) on ellipse, compute the other
    // mode='x' means x is given, find y
    // mode='y' means y is given, find x
    // Returns two solutions (pair) because ellipse symmetric
    // If no real solution, returns NaN
    pair<double,double> computeOtherCoordinate(double known, char mode) const {
        double val1 = NAN, val2 = NAN;

        if (mode == 'x') {
            double x = known;
            if (majorAxisX) {
                double term = 1 - ((x - h)*(x - h)) / (a*a);
                if (term < 0) return {NAN, NAN};
                double y1 = k + b * sqrt(term);
                double y2 = k - b * sqrt(term);
                return {y1, y2};
            } else {
                // major axis along Y
                // equation: ((x - h)^2)/b^2 + ((y-k)^2)/a^2 =1
                double term = 1 - ((x - h)*(x - h)) / (b*b);
                if (term < 0) return {NAN, NAN};
                double y1 = k + a * sqrt(term);
                double y2 = k - a * sqrt(term);
                return {y1, y2};
            }
        } else if (mode == 'y') {
            double y = known;
            if (majorAxisX) {
                double term = 1 - ((y - k)*(y - k)) / (b*b);
                if (term < 0) return {NAN, NAN};
                double x1 = h + a * sqrt(term);
                double x2 = h - a * sqrt(term);
                return {x1, x2};
            } else {
                // major axis along Y
                double term = 1 - ((y - k)*(y - k)) / (a*a);
                if (term < 0) return {NAN, NAN};
                double x1 = h + b * sqrt(term);
                double x2 = h - b * sqrt(term);
                return {x1, x2};
            }
        }
        return {NAN, NAN}; // invalid mode
    }
};


// ======================== Quaternion Class ===========================
class Quaternion {
private:
    double a, b, c, d; // a: real part, b,c,d imaginary parts

public:
    // Constructors
    Quaternion() : a(1), b(0), c(0), d(0) {}
    Quaternion(double a_, double b_, double c_, double d_) : a(a_), b(b_), c(c_), d(d_) {}
    Quaternion(const Quaternion& other) : a(other.a), b(other.b), c(other.c), d(other.d) {}

    // Setters
    void set(double a_, double b_, double c_, double d_) {
        a = a_; b = b_; c = c_; d = d_;
    }

    // Getters
    void get(double &a_, double &b_, double &c_, double &d_) const {
        a_ = a; b_ = b; c_ = c; d_ = d;
    }

    // Print quaternion in typical form
    void print() const {
        cout << fixed << setprecision(4);
        cout << a << " + " << b << "i + " << c << "j + " << d << "k";
    }

    // Norm ||q|| = sqrt(a^2 + b^2 + c^2 + d^2)
    double norm() const {
        return sqrt(a*a + b*b + c*c + d*d);
    }

    // Conjugate q* = a - bi - cj - dk
    Quaternion conjugate() const {
        return Quaternion(a, -b, -c, -d);
    }

    // Normalized quaternion q / ||q||
    Quaternion normalized() const {
        double n = norm();
        if (n == 0) return Quaternion(0,0,0,0);
        return Quaternion(a/n, b/n, c/n, d/n);
    }

    // Inverse q^-1 = q* / ||q||^2
    Quaternion inverse() const {
        double n2 = a*a + b*b + c*c + d*d;
        if (n2 == 0) return Quaternion(0,0,0,0);
        Quaternion conj = conjugate();
        return Quaternion(conj.a / n2, conj.b / n2, conj.c / n2, conj.d / n2);
    }

    // Operator overloads for quaternion arithmetic
    // Addition
    Quaternion operator+(const Quaternion& q) const {
        return Quaternion(a + q.a, b + q.b, c + q.c, d + q.d);
    }

    // Subtraction
    Quaternion operator-(const Quaternion& q) const {
        return Quaternion(a - q.a, b - q.b, c - q.c, d - q.d);
    }

    // Multiplication (Hamilton product)
    Quaternion operator*(const Quaternion& q) const {
        double A = a*q.a - b*q.b - c*q.c - d*q.d;
        double B = a*q.b + b*q.a + c*q.d - d*q.c;
        double C = a*q.c - b*q.d + c*q.a + d*q.b;
        double D = a*q.d + b*q.c - c*q.b + d*q.a;
        return Quaternion(A,B,C,D);
    }

    // Division: q1 / q2 = q1 * q2^-1
    Quaternion operator/(const Quaternion& q) const {
        return (*this) * q.inverse();
    }

    // Scalar (dot) product q1 · q2 = a1a2 + b1b2 + c1c2 + d1d2
    double scalarProduct(const Quaternion& q) const {
        return a*q.a + b*q.b + c*q.c + d*q.d;
    }

    // Euclidean distance between two quaternions
    double euclideanDistance(const Quaternion& q) const {
        double da = a - q.a;
        double db = b - q.b;
        double dc = c - q.c;
        double dd = d - q.d;
        return sqrt(da*da + db*db + dc*dc + dd*dd);
    }

    // Chebyshev norm (max absolute difference)
    double chebyshevNorm(const Quaternion& q) const {
        double da = fabs(a - q.a);
        double db = fabs(b - q.b);
        double dc = fabs(c - q.c);
        double dd = fabs(d - q.d);
        return max({da, db, dc, dd});
    }
};


// =========================== Main function for testing ======================
int main() {
    cout << "=== Ellipse Class Demo ===\n";

    Ellipse e1; // default ellipse
    e1.print();

    cout << "\nCreate ellipse with center (2,3), a=5, b=3, major axis X\n";
    Ellipse e2(2,3,5,3,true);
    e2.print();

    // Test point relation
    double px = 3, py = 4;
    int rel = e2.pointRelation(px, py);
    cout << "Point (" << px << "," << py << ") is ";
    if (rel == 0) cout << "on the ellipse\n";
    else if (rel < 0) cout << "inside the ellipse\n";
    else cout << "outside the ellipse\n";

    // Compute other coordinate
    cout << "Given x=6, compute y:\n";
    auto ys = e2.computeOtherCoordinate(6, 'x');
    cout << "y = " << ys.first << " or " << ys.second << "\n";

    cout << "\n=== Quaternion Class Demo ===\n";

    Quaternion q1; // default quaternion (1+0i+0j+0k)
    cout << "Default quaternion q1: ";
    q1.print();
    cout << "\nNorm(q1) = " << q1.norm() << "\n";

    Quaternion q2(0,1,1,1);
    cout << "Quaternion q2: ";
    q2.print();
    cout << "\nNorm(q2) = " << q2.norm() << "\n";

    Quaternion q3 = q1 + q2;
    cout << "q1 + q2 = ";
    q3.print();
    cout << "\n";

    Quaternion q4 = q1 * q2;
    cout << "q1 * q2 = ";
    q4.print();
    cout << "\n";

    Quaternion q5 = q2.inverse();
    cout << "Inverse of q2: ";
    q5.print();
    cout << "\n";

    cout << "Euclidean distance between q1 and q2 = " << q1.euclideanDistance(q2) << "\n";
    cout << "Chebyshev norm between q1 and q2 = " << q1.chebyshevNorm(q2) << "\n";

    return 0;
}
