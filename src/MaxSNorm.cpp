#include "../include/MaxSNorm.h"
#include <algorithm>
double MaxSNorm::apply(double a,double b)const{
    return std::max(a,b);
}