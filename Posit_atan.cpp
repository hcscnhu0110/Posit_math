#include "myfdlibm.h"
#include <iostream>
#include <iomanip>
using namespace std;

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
static const double atanhi[] = {
#else
static double atanhi[] = {
#endif
  4.63647609000806093515e-01, /* atan(0.5)hi 0x3FDDAC67, 0x0561BB4F */
  7.85398163397448278999e-01, /* atan(1.0)hi 0x3FE921FB, 0x54442D18 */
  9.82793723247329054082e-01, /* atan(1.5)hi 0x3FEF730B, 0xD281F69B */
  1.57079632679489655800e+00, /* atan(inf)hi 0x3FF921FB, 0x54442D18 */
};

#ifdef __STDC__
static const double atanlo[] = {
#else
static double atanlo[] = {
#endif
  2.26987774529616870924e-17, /* atan(0.5)lo 0x3C7A2B7F, 0x222F65E2 */
  3.06161699786838301793e-17, /* atan(1.0)lo 0x3C81A626, 0x33145C07 */
  1.39033110312309984516e-17, /* atan(1.5)lo 0x3C700788, 0x7AF0CBBD */
  6.12323399573676603587e-17, /* atan(inf)lo 0x3C91A626, 0x33145C07 */
};

#ifdef __STDC__
static const double aT[] = {
#else
static double aT[] = {
#endif
  3.33333333333329318027e-01, /* 0x3FD55555, 0x5555550D */
 -1.99999999998764832476e-01, /* 0xBFC99999, 0x9998EBC4 */
  1.42857142725034663711e-01, /* 0x3FC24924, 0x920083FF */
 -1.11111104054623557880e-01, /* 0xBFBC71C6, 0xFE231671 */
  9.09088713343650656196e-02, /* 0x3FB745CD, 0xC54C206E */
 -7.69187620504482999495e-02, /* 0xBFB3B0F2, 0xAF749A6D */
  6.66107313738753120669e-02, /* 0x3FB10D66, 0xA0D03D51 */
 -5.83357013379057348645e-02, /* 0xBFADDE2D, 0x52DEFD9A */
  4.97687799461593236017e-02, /* 0x3FA97B4B, 0x24760DEB */
 -3.65315727442169155270e-02, /* 0xBFA2B444, 0x2C6A6C2F */
  1.62858201153657823623e-02, /* 0x3F90AD3A, 0xE322DA11 */
};

#ifdef __STDC__
	static const Posit64 
#else
	static Posit64 
#endif
one   = 1.0,
huge   = 1.0e300;

#ifdef __STDC__
	Posit64 Posit_atan(Posit64 x)
#else
	Posit64 atan(x)
	Posit64 x;
#endif
{
	Posit64 w, s1, s2, z, fabs_x = Posit_fabs(x);
	__int32_t hx,id;

	GET_HIGH_WORD(hx,x);

    Posit64 limit1{7.3786976e+19}, limit2{0.4375}, limit3{1.86264515e-9};
	if(fabs_x >= limit1) {	/* if |x| >= 2^66 */
	    __uint32_t low;
	    GET_LOW_WORD(low,x);
	    if(x == NAR) return x+x;		/* NaN */
	    if(hx>0) return  atanhi[3]+atanlo[3];
	    else     return -atanhi[3]-atanlo[3];
	} 
    if (fabs_x < limit2) {	/* |x| < 0.4375 */
	    if (fabs_x < limit3) {	/* |x| < 2^-29 */
		    if(huge+x>one) return x;	/* raise inexact */
	    }
	    id = -1;
	} 
    else {
        Posit64 limit4{1.1875}, limit5{0.6875}, limit6{2.4375};
        x = Posit_fabs(x);
        if (x < limit4) {		/* |x| < 1.1875 */
            if (x < limit5) {	/* 7/16 <=|x|<11/16 */
                id = 0; x = (2.0*x-one)/(2.0+x); 
            } else {			/* 11/16<=|x|< 19/16 */
                id = 1; x  = (x-one)/(x+one); 
            }
        } else {
            if (x < limit6) {	/* |x| < 2.4375 */
                id = 2; x  = (x-1.5)/(one+1.5*x);
            } else {			/* 2.4375 <= |x| < 2^66 */
                id = 3; x  = -1.0/x;
            }
        }
    }
    /* end of argument reduction */
	z = x*x;
	w = z*z;
    /* break sum from i=0 to 10 aT[i]z**(i+1) into odd and even poly */
	s1 = z*(aT[0]+w*(aT[2]+w*(aT[4]+w*(aT[6]+w*(aT[8]+w*aT[10])))));
	s2 = w*(aT[1]+w*(aT[3]+w*(aT[5]+w*(aT[7]+w*aT[9]))));
	if (id<0) return x - x*(s1+s2);
	else {
	    z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
	    return (hx<0)? -z:z;
	}
}

#endif /* _DOUBLE_IS_32BITS */

// int main(){
//     Posit64 x{2.136};
//     cout << fixed << setprecision(8) << "Input: " << x << "\n";
//     cout << fixed << setprecision(20) << "Result: " << Posit_atan(x) << "\n";
// }
