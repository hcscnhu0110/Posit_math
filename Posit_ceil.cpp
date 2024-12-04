#include "myfdlibm.h"
#include <iostream>
using namespace std;

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
Posit64 huge2{1.0e300};
#else
static Posit64 huge2{1.0e300};
#endif

#ifdef __STDC__
	Posit64 Posit_ceil(Posit64 x)
#else
	Posit64 Posit_ceil(x)
	Posit64 x;
#endif
{
    x = (int)x;
    if(x >= 0) x += 1;
    return x;
}

#endif 
