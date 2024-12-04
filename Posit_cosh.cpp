#include "myfdlibm.h"

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
static const Posit64 one = 1.0, half=0.5, shuge = 1.0e307;
#else
static Posit64 one = 1.0, half=0.5, shuge = 1.0e307;
#endif

#ifdef __STDC__
	Posit64 Posit_cosh(Posit64 x)
#else
	Posit64 Posit_cosh(x)
	Posit64 x;
#endif
{	
	Posit64 t, w, fabs_x = Posit_fabs(x);

    /* x is INF or NaN */
	if(x == NAR) return x*x;	

    /* |x| in [0,0.5*ln2], return 1+expm1(|x|)^2/(2*exp(|x|)) */ 
    Posit64 limit1{0.346573590279973}, limit2{2.7755575615628914E-17};
	if(fabs_x < limit1){
	    t = expm1(Posit_fabs(x));
	    w = one+t;
	    if (fabs_x < limit2) return w;	/* cosh(tiny) = 1 */
	    return one + (t*t) / (w+w);
	}

    /* |x| in [0.5*ln2,22], return (exp(|x|)+1/exp(|x|)/2; */
    Posit64 limit3{22};
	if (fabs_x < limit3) {
		t = Posit_exp(Posit_fabs(x));
		return half*t + half/t;
	}

    /* |x| in [22, log(maxdouble)] return half*exp(|x|) */
    Posit64 lnovft{709.7822265625};
	if (fabs_x < lnovft)  return half * Posit_exp(Posit_fabs(x));

    /* |x| in [log(maxdouble), overflowthresold] */
	Posit64 ovft1{710.4755859375}, ovft2{710.4758600739439};
	if (fabs_x < ovft1 || fabs_x <= ovft2) {
	    w = Posit_exp(half * Posit_fabs(x));
	    t = half * w;
	    return t * w;
	}

    /* |x| > overflowthresold, cosh(x) overflow */
	return x * shuge;
}

#endif /* defined(_DOUBLE_IS_32BITS) */

// int main(){
//     Posit64 x{-21.9438};
//     cout << fixed << setprecision(8) << "Input: " << x << "\n";
//     cout << fixed << setprecision(21) << "Result: " << Posit_cosh(x) << "\n";
// }
