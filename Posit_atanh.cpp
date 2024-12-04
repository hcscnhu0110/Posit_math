#include "myfdlibm.h"
#include <iostream>
#include <iomanip>
using namespace std;

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
static const Posit64 one = 1.0, huge = 1e300;
#else
static Posit64 one = 1.0, huge = 1e300;
#endif

#ifdef __STDC__
static const Posit64 zero = 0.0;
#else
static Posit64 zero = 0.0;
#endif

#ifdef __STDC__
	Posit64 Posit_atanh(Posit64 x)
#else
	Posit64 Posit_atanh(x)
	Posit64 x;
#endif
{
	Posit64 t, fabs_x = Posit_fabs(x);
	__int32_t hx,ix;
	__uint32_t lx;

	EXTRACT_WORDS(hx,lx,x);

    Posit64 limit1{1}, limit2{3.7252903e-9}, limit3{0.5};
	if (fabs_x > limit1) /* |x|>1 */
	    return (x-x)/(x-x);
	if(fabs_x == 1) 
	    return x/zero;
	if(fabs_x < limit2) return x;	/* x<2**-28 */
	
    x = fabs_x;
    Posit64 half_one{0.5};
    
	if(fabs_x < limit3){		/* x < 0.5 */
	    t = x+x;
	    t = half_one * Posit_log(1+t+t*x/(one-x));
	} else 
	    t = half_one * Posit_log(1+(x+x)/(one-x));
	if(hx>=0) return t; else return -t;
}

#endif /* defined(_DOUBLE_IS_32BITS) */

// int main(){
//     Posit64 x{0.7347};
//     cout << fixed << setprecision(8) << "Input: " << x << "\n";
//     cout << fixed << setprecision(20) << "Result: " << Posit_atanh(x) << "\n";
// }
