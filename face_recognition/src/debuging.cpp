// debuging.cpp
#include "debuging.h"

float std_conversion::int2float(int x){
	char buf[50];
    sprintf(buf, "%d", x);
    return atof(buf);
}
