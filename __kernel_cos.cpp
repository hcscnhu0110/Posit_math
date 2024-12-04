#include "myfdlibm.h"
#include <iostream>
using namespace std;

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
Posit64 
#else
Posit64
#endif
one =  1.00000000000000000000e+00, /* 0x3FF00000, 0x00000000 */
C1  =  4.16666666666666019037e-02, /* 0x3FA55555, 0x5555554C */
C2  = -1.38888888888741095749e-03, /* 0xBF56C16C, 0x16C15177 */
C3  =  2.48015872894767294178e-05, /* 0x3EFA01A0, 0x19CB1590 */
C4  = -2.75573143513906633035e-07, /* 0xBE927E4F, 0x809C52AD */
C5  =  2.08757232129817482790e-09, /* 0x3E21EE9E, 0xBDB4B1C4 */
C6  = -1.13596475577881948265e-11; /* 0xBDA8FAE9, 0xBE8838D4 */

#ifdef __STDC__
	Posit64 __kernel_cos(Posit64 x, Posit64 y)
#else
	Posit64 __kernel_cos(x, y)
	Posit64 x, y;
#endif
{
	Posit64 a, hz, z, r, qx, tmp_x = x;
	Posit64 b{0.5};

	//if(x < 0) tmp_x = -x;
	__int32_t ix;
	GET_HIGH_WORD(ix, tmp_x);
	ix &= 0x7fffffff;			/* ix = |x|'s high word*/

	Posit64 limit1{134217728};  //limit1 = 2**27 
	if(ix < limit1) {			/* if x < 2**27 */
	    if(((int)x)==0) return one;		/* generate inexact */
	}

	z  = x * x;
	r  = z * (C1+z*(C2+z*(C3+z*(C4+z*(C5+z*C6)))));

	Posit64 limit2{0.3};
	if(ix < limit2) 			/* if |x| < 0.3 */ 
	    return one - (b*z - (z*r - x*y));
	else {
		Posit64 limit3{0.78125};
	    if(ix > limit3) {		/* x > 0.78125 */
			Posit64 tmp{0.28125};
		    qx = tmp;
	    } 
		else {
	        x /= 4;
	    }
	    hz = b * z - qx;
	    a  = one-qx;
	    return a - (hz - (z*r-x*y));
	}
}

#endif /* defined(_DOUBLE_IS_32BITS) */

// int main(){
//     Posit64 x{-0.63478}, y{0};
//     cout << "input: " << x << "\n";
//     cout << "result: " << __kernel_cos(x, y) << "\n";
// }
