#include "myfdlibm.h"
#include <iostream>
#include <iomanip>
using namespace std;

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
static const Posit64
#else
static Posit64
#endif
two54      =  1.80143985094819840000e+16, /* 0x43500000, 0x00000000 */
ivln10     =  4.34294481903251816668e-01, /* 0x3FDBCB7B, 0x1526E50E */
log10_2hi  =  3.01029995663611771306e-01, /* 0x3FD34413, 0x509F6000 */
log10_2lo  =  3.69423907715893078616e-13; /* 0x3D59FEF3, 0x11F12B36 */

#ifdef __STDC__
static const Posit64 zero   =  0.0;
#else
static Posit64 zero   =  0.0;
#endif

#ifdef __STDC__
	Posit64 Posit_log10(Posit64 x)
#else
	Posit64 __ieee754_log10(x)
	Posit64 x;
#endif
{
	Posit64 y, z;
    int n;
    __uint32_t hx;
    GET_HIGH_WORD(hx,x);
    int data[5] = {0};
	getRegime(hx, data);
	getExponent(hx, data);

    // Normalize x and extract exponent
    n = data[2];        // Extract the exponent n such that x = m * 2^n
    if (n < 0) n += 1;   // Adjust n if it's negative
    Posit64 pn{n};
    x = x * Posit_pow(2, -pn);   // Scale x by 2^(-n), effectively normalizing it

    // Compute log10(x) using the formula
    z = ivln10 * Posit_log(x);   // ivln10 * natural log of x
    y = n * log10_2hi + (z + n * log10_2lo);

    return y;
}

#endif /* defined(_DOUBLE_IS_32BITS) */

// int main(){
//     Posit64 x{45.2523};
//     cout << fixed << setprecision(20) << Posit_log10(x) << "\n";
// }

