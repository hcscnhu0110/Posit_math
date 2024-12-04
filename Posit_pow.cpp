#include <iostream>
#include <iomanip>
#include "myfdlibm.h"
using namespace std;

Posit64 myfmod(Posit64 x, Posit64 y) {
    if (y == 0.0) return NAN;
    
    Posit64 result = x - (floor(x / y) * y);

    return result;
}

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
	Posit64 Posit_pow(Posit64 x, Posit64 y)
#else
	Posit64 Posit_pow(x, y)
	Posit64 x, y;
#endif
{
	if (y == 0.0) return 1.0;  // anything ** 0 is 1
    if (x == 0.0) {
        if (y > 0) return 0.0;  // +0 ** (+anything except 0, NAN) is +0
        else return INFINITY;   // +0 ** (-anything except 0, NAN) is +INF
    }
    if (x == 1.0 || y == 1.0) return x;  // (anything) ** 1 is itself
    if (isnan(x) || isnan(y)) {
        if (x == 1.0) return 1.0;  // +1 ** NAN is 1
        else return NAN;           // anything ** NAN is NAN
    }
    if (isinf(y)) {
        if (fabs(x) > 1.0) return (y > 0) ? INFINITY : 0.0;  // |x| > 1
        else if (fabs(x) < 1.0) return (y > 0) ? 0.0 : INFINITY;  // |x| < 1
        else return 1.0;  // +-1 ** +-INF is 1
    }
    
    if (isinf(x)) {
        if (y > 0) return (x > 0) ? INFINITY : (myfmod(y, 2.0) == 0.0 ? INFINITY : -INFINITY);  // +INF or -INF
        else return 0.0;  // +INF ** (-anything except 0, NAN) is +0
    }

    // // Handling negative bases
    if (x < 0) {
        if (floor(y) != y) return NAN;  // (-anything except 0 and inf) ** (non-integer) is NAN
        Posit64 two{2.0};
        return (myfmod(y, two) == 0.0) ? pow(-x, y) : -pow(-x, y);  // (-anything) ** (integer)
    }

    //cout << y * Posit_log(x) << "\n";
    return Posit_exp(y * Posit_log(x));  // x^y = exp(y * log(x))
}

#endif /* defined(_DOUBLE_IS_32BITS) */


// int main(){
//     Posit64 x{0.742}, y{-2.4};
//     cout << "x: "<< x << "  y: " <<  y << "\n";
//     cout << "result: "<< fixed << setprecision(20) << Posit_pow(x, y) << "\n";
// }
