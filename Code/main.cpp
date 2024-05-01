#include "Source.h"
int main() {
    int choice;
    cout << "Select operation:\n";
    cout << "1. clamped_unclamped\n";
    cout << "2.  knot_insert_refine\n";
    cout << "3. General Cylinder\n";
    cout << "4. Ruled_Surface\n";
    cout << "5. Bezier\n";
    cout << "6. Rational Bezier\n";
    cout << "7. Cspline" << endl;
    
    cin >> choice;

    switch (choice) {
    case 1:
        clamped_unclamped();
        break;
    case 2:
        knot_insert_refine();
        break;
    case 3:
        General_Cylinder();
        break;
    case 4:
        Ruled_Surface();
        break;
    case 5:
        Bezier();
        break;
    case 6:
        RationalBezier();
        break;
    case 7:
        CSpline();
        break;
    
    }
    //Pause the console window before closing
    system("pause");
    return 0;
}