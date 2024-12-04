#include "myfdlibm.h"
#include <iostream>
using namespace std;

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
	Posit64 Posit_cos(Posit64 x)
#else
	Positt64 Posit_cos(x)
	Posit64 x;
#endif
{
	Posit64 y[2], z{0.0}, fabs_x = Posit_fabs(x);
	__int32_t n,ix;

    /* |x| ~< pi/4 */
	Posit64 limit{0.785398006439209};
	if(fabs_x <= limit) return __kernel_cos(x, 0);

    /* cos(Inf or NaN) is NaN */
	else if (x == NAR) return x-x;

    /* argument reduction needed */
	else {

	    n = Posit_rempio2(x, y);
	    switch(n&3) {
		case 0: return  __kernel_cos(y[0],y[1]);
		case 1: return -__kernel_sin(y[0],y[1],1);
		case 2: return -__kernel_cos(y[0],y[1]);
		default:
		        return  __kernel_sin(y[0],y[1],1);
	    }
	}
}

#endif /* _DOUBLE_IS_32BITS */

// int main(){
// 	Posit64 value{436.6543};
// 	cout << "Input: " << value << "\n";
//     cout << fixed << setprecision(16) << "Result: " << cos(value) << "\n";
// }
