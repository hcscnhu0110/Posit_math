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
one	= 1.0,
ln2	= 6.93147180559945286227e-01;  /* 0x3FE62E42, 0xFEFA39EF */

#ifdef __STDC__
	Posit64 Posit_acosh(Posit64 x)
#else
	Posit64 Posit_acosh(x)
	Posit64 x;
#endif
{	
	Posit64 t;
	__int32_t hx;
	__uint32_t lx;

    Posit64 limit1{1}, limit2{268435456}, limit3{2};
	if(x < limit1) {		/* x < 1 */
	    return (x-x)/(x-x);
	} else if(x >= limit2) {	/* x > 2**28 */
	    if(hx >=0x7ff00000) {	/* x is inf of NaN */
	        return x+x;
	    } 
        else return Posit_log(x) + ln2;	/* acosh(huge)=log(2x) */
	} else if(x == limit1) {
	    return 0.0;			/* acosh(1) = 0 */
	} else if (x > limit3) {	/* 2**28 > x > 2 */
	    t=x*x;
	    return Posit_log(2.0*x - one/(x + Posit_sqrt(t-one)));
	} else {			/* 1<x<2 */
	    t = x-one;
	    return Posit_log(1 + t + Posit_sqrt(2.0*t+t*t));
	}
}

#endif /* defined(_DOUBLE_IS_32BITS) */

// int main(){
//     Posit64 x{1.3462};
//     cout << "Input: " << x << "\n";
//     cout << fixed << setprecision(25) << "Result: " << Posit_acosh(x) << "\n";
// }
