#include "myfdlibm.h"
#include <iostream>
using namespace std;

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
	Posit64 Posit_fabs(Posit64 x)
#else
	Posit64 Posit_fabs(x)
	Posit64 x;
#endif
{
	if(x < 0) x = -x;
    return x;
}

#endif /* _DOUBLE_IS_32BITS */

// int main(){
//     Posit64 x{-34552.26524};
//     cout << fixed << setprecision(10) << fabs(x) << "\n";
// }