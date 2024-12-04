#include <iostream>
#include "myfdlibm.h"
using namespace std;

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
static const Posit64
#else
static Posit64
#endif
ln2_hi  =  6.93147180369123816490e-01,	/* 3fe62e42 fee00000 */
ln2_lo  =  1.90821492927058770002e-10,	/* 3dea39ef 35793c76 */
two54   =  1.80143985094819840000e+16,  /* 43500000 00000000 */
Lg1 = 6.666666666666735130e-01,  /* 3FE55555 55555593 */
Lg2 = 3.999999999940941908e-01,  /* 3FD99999 9997FA04 */
Lg3 = 2.857142874366239149e-01,  /* 3FD24924 94229359 */
Lg4 = 2.222219843214978396e-01,  /* 3FCC71C5 1D8E78AF */
Lg5 = 1.818357216161805012e-01,  /* 3FC74664 96CB03DE */
Lg6 = 1.531383769920937332e-01,  /* 3FC39A09 D078C69F */
Lg7 = 1.479819860511658591e-01;  /* 3FC2F112 DF3E5244 */

#ifdef __STDC__
Posit64 zero   =  0.0;
#else
Posit64 zero   =  0.0;
#endif

#ifdef __STDC__
	Posit64 Posit_log(Posit64 x)
#else
	Posit64 Posit_log(Posit64)
	Posit64 x;
#endif
{
	Posit64 hfsq, f, s, z, R, w, t1, t2, dk;
	__int32_t k, hx, i, j;
	__uint32_t lx;

	EXTRACT_WORDS(hx, lx, x);

	k = 0;
	if (hx < 0x00100000) {			/* x < 2**-1022  */
	    if (x==0) return -two54/zero;		/* log(+-0)=-inf */
	    if (hx<0) return (x-x)/zero;	/* log(-#) = NaN */
	    k -= 54; x *= two54; /* subnormal number, scale up x */
	    GET_HIGH_WORD(hx,x);
	} 
	if (hx >= 0x7ff00000) return x+x;

	int data[5] = {0};
	getRegime(hx, data);
	getExponent(hx, data);

	int ex = 1;
	for(int i = 0; i < N; i++){
		ex *= 2;
	}

	k += (ex * data[0] + data[2]);
	hx &= (0x7fffffff >> (N + data[1])); 
	int offset = 12 - (N + 1 + data[1]);
	
	i = (hx + (0x95f64 << offset)) & (0x100000 << offset); 
	k += (i >> (20 + offset));
	
	Posit64 two{2};
    while(x >= Posit_sqrt(two)){
        x /= 2;    
    }

	while(x <= (Posit_sqrt(two)/two)){
        x *= 2;    
    }
	
	f = x - 1.0;

	if((0x000fffff&(2+hx))<3) {	/* |f| < 2**-20 */
        if(f==zero) { 
			if(k==0) return zero;  
			else {dk=(Posit64)k; return dk*ln2_hi+dk*ln2_lo;}
		}
	    R = f*f*(0.5-0.33333333333333333*f);
	    if(k==0) return f-R; 
		else {dk=(Posit64)k; return dk*ln2_hi-((R-dk*ln2_lo)-f);}
	}

 	s = f/(2.0+f); 
	Posit64 Posit_k{k};
	dk = Posit_k;
    
	z = s*s;
	i = hx-(0x6147a << offset);
	w = z*z;
	j = (0x6b851 << offset) - hx;
	t1= w*(Lg2+w*(Lg4+w*Lg6)); 
	t2= z*(Lg1+w*(Lg3+w*(Lg5+w*Lg7))); 
	i |= j;
	R = t2+t1;

	if(i>0) {
	    hfsq=0.5*f*f;
	    if(k==0) return f-(hfsq-s*(hfsq+R)); 
		else return dk*ln2_hi-((hfsq-(s*(hfsq+R)+dk*ln2_lo))-f);
	} else {
	    if(k==0) return f-s*(f-R);
		else return dk*ln2_hi-((s*(f-R)-dk*ln2_lo)-f);
	}
}
#endif /* defined(_DOUBLE_IS_32BITS) */

// int main(){
//     Posit64 x{0.1100919444};
// 	cout << fixed << setprecision(9) << "Input:\t" << x << "\n";
//     cout << fixed << setprecision(20) << "Result:\t" << Posit_log(x) << "\n"; 
// 	//cout << fixed << setprecision(20) << "Result:\t" << log(x) << "\n"; 
// }
