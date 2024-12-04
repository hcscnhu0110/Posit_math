
/* @(#)s_asinh.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/*
FUNCTION
	<<asinh>>, <<asinhf>>---inverse hyperbolic sine 

INDEX
	asinh
INDEX
	asinhf

SYNOPSIS
	#include <math.h>
	double asinh(double <[x]>);
	float asinhf(float <[x]>);

DESCRIPTION
<<asinh>> calculates the inverse hyperbolic sine of <[x]>.
<<asinh>> is defined as 
@ifnottex
. sgn(<[x]>) * log(abs(<[x]>) + sqrt(1+<[x]>*<[x]>))
@end ifnottex
@tex
$$sign(x) \times ln\Bigl(|x| + \sqrt{1+x^2}\Bigr)$$
@end tex

<<asinhf>> is identical, other than taking and returning floats.

RETURNS
<<asinh>> and <<asinhf>> return the calculated value.

PORTABILITY
Neither <<asinh>> nor <<asinhf>> are ANSI C.

*/

/* asinh(x)
 * Method :
 *	Based on 
 *		asinh(x) = sign(x) * log [ |x| + sqrt(x*x+1) ]
 *	we have
 *	asinh(x) := x  if  1+x*x=1,
 *		 := sign(x)*(log(x)+ln2)) for large |x|, else
 *		 := sign(x)*log(2|x|+1/(|x|+sqrt(x*x+1))) if|x|>2, else
 *		 := sign(x)*log1p(|x| + x^2/(1 + sqrt(1+x^2)))  
 */

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
one =  1.00000000000000000000e+00, /* 0x3FF00000, 0x00000000 */
ln2 =  6.93147180559945286227e-01, /* 0x3FE62E42, 0xFEFA39EF */
huge=  1.00000000000000000000e+300; 

#ifdef __STDC__
	Posit64 Posit_asinh(Posit64 x)
#else
	Posit64 Posit_asinh(x)
	Posit64 x;
#endif
{	
	Posit64 t, w, fabs_x = Posit_fabs(x);
	__int32_t hx;
	GET_HIGH_WORD(hx,x);

	//if(ix>=0x7ff00000) return x+x;	/* x is inf or NaN */
	Posit64 limit1{3.7252903e-9}, limit2{268435456}, limit3{2};
	if(fabs_x < limit1) {	/* |x|<2**-28 */
	    if(huge+x>one) return x;	/* return x inexact except 0 */
	} 
	if(fabs_x > limit2) {	/* |x| > 2**28 */
	    w = Posit_log(Posit_fabs(x))+ln2;
	} else if (fabs_x > limit3) {	/* 2**28 > |x| > 2.0 */
	    t = fabs(x);
	    w = Posit_log(2.0*t + one/(Posit_sqrt(x*x+one)+t));
	} else {		/* 2.0 > |x| > 2**-28 */
	    t = x*x;
	    w = Posit_log(1 + Posit_fabs(x)+t/(one + Posit_sqrt(one+t)));
	}
	if(hx>0) return w; else return -w;
}

#endif /* _DOUBLE_IS_32BITS */

// int main(){
//     Posit64 x{2.2523};
//     cout << "Input: " << x << "\n";
//     cout << fixed << setprecision(25) << "Result: " << Posit_asinh(x) << "\n";
// }