#include "myfdlibm.h"
using namespace std;

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
static const Posit64 one = 1.0, shuge = 1.0e307;
#else
static Posit64 one = 1.0, shuge = 1.0e307;
#endif

#ifdef __STDC__
	Posit64 Posit_sinh(Posit64 x)
#else
	Posit64 Posit_sinh(x)
	Posit64 x;
#endif
{	
	Posit64 t, w, h, fabs_x = Posit_fabs(x);

    /* x is INF or NaN */
	if(x == NAR) return x+x;	

	h = 0.5;
	Posit64 zero{0};
	if (x < zero) h = -h;

    /* |x| in [0,22], return sign(x)*0.5*(E+E/(E+1))) */
	Posit64 limit1{22};
	if (fabs_x < limit1) {		/* |x|<22 */
        Posit64 limit2{3.725290298461914E-9};
	    if (fabs_x < limit2) 		/* |x|<2**-28 */
			if(shuge+x > one) return x;/* sinh(tiny) = tiny with inexact */
	    t = expm1(Posit_fabs(x));
        Posit64 tmp{2.0};
	    if(fabs_x < one) return h * (tmp*t-t*t/(t+one));
	    return h * (t+t / (t+one));
	}

    /* |x| in [22, log(maxdouble)] return 0.5*exp(|x|) */
    Posit64 lnovft{709.7822265625};
	if (fabs_x < lnovft)  return h * Posit_exp(Posit_fabs(x));

    /* |x| in [log(maxdouble), overflowthresold] */
    Posit64 ovft1{710.4755859375}, ovft2{710.4758600739439};
	if (fabs_x < ovft1 || fabs_x <= ovft2) {
		w = Posit_exp(0.5* Posit_fabs(x));
		t = h * w;
		return t * w;
	}

    /* |x| > overflowthresold, sinh(x) overflow */
	return x * shuge;
}

#endif /* defined(_DOUBLE_IS_32BITS) */

// int main(){
//     Posit64 x{-13.3435};
//     cout << fixed << setprecision(8) << "Input: " << x << "\n";
//     cout << fixed << setprecision(20) << "Result: " << Posit_sinh(x) << "\n";
// }
