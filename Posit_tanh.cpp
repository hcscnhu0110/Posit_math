#include "myfdlibm.h"
#include <iostream>
#include <iomanip>
using namespace std;

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
static const  Posit64 one = 1.0, two = 2.0, tiny = 1.0e-300;
#else
static double Posit64 one = 1.0, two = 2.0, tiny = 1.0e-300;
#endif

#ifdef __STDC__
	Posit64 Posit_tanh(Posit64 x)
#else
	Posit64 tanh(x)
	Posit64 x;
#endif
{
	Posit64 t, z, fabs_x = Posit_fabs(x);
	__int32_t jx,ix;

    /* High word of |x|. */
	GET_HIGH_WORD(jx,x);
     /* x is INF or NaN */
	// if(ix>=0x7ff00000) { 
	//     if (jx>=0) return one / x+one;    /* tanh(+-inf)=+-1 */
	//     else       return one / x-one;    /* tanh(NaN) = NaN */
	// }

    /* |x| < 22 */
    Posit64 limit1{22}, limit2{2.7755576e-17}, limit3{1};
	if (fabs_x < limit1) {		/* |x|<22 */
	    if (fabs_x < limit2) 		/* |x|<2**-55 */
		return x*(one+x);    	/* tanh(small) = small */
	    if (fabs_x >=limit3) {	/* |x|>=1  */
            t = expm1(two*fabs(x));
            z = one - two/(t+two);
	    } else {
	        t = expm1(-two*fabs(x));
	        z= -t/(t+two);
	    }
    /* |x| > 22, return +-1 */
	} else {
	    z = one - tiny;		/* raised inexact flag */
	}
	return (jx>=0)? z: -z;
}

#endif /* _DOUBLE_IS_32BITS */

// int main(){
//     Posit64 x{-0.3435};
//     cout << fixed << setprecision(8) << "Input: " << x << "\n";
//     cout << fixed << setprecision(20) << "Result: " << Posit_tanh(x) << "\n";
// }

