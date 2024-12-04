#include "myfdlibm.h"
#include <iostream>
#include <iomanip>
using namespace std;

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
Posit64 huge{1.0e300};
#else
static Posit64 huge{1.0e300};
#endif

#ifdef __STDC__
	Posit64 Posit_floor(Posit64 x)
#else
	Posit64 Posit_floor(x)
	Posit64 x;
#endif
{
	x = (int)x;

	return x;
}

#endif /* _DOUBLE_IS_32BITS */

// int main(){
//     Posit64 x{124151.325};
//     cout << fixed << setprecision(10) << Posit_floor(x) << "\n";
// }

