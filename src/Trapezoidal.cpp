#include "../include/Trapezoidal.h"
#include <algorithm>
Trapezoidal::Trapezoidal(const std::string&name,double a,double b,double c,double d):
    name_(name),
    a_(a),
    b_(b),c_(c),d_(d){}
double Trapezoidal::compute(double x) const{
    if (x <= a_ || x >= d_)
    {
        return 0.0;
    }

    if (x >= b_ && x <= c_)
    {
        return 1.0;
    }

    if (x > a_ && x < b_)
    {
        return (x - a_) / (b_ - a_);
    }

    return (d_ - x) / (d_ - c_);
}
std::string Trapezoidal::getName() const{
    return name_;
}
double Trapezoidal::getA() const {
    return a_;
}

double Trapezoidal::getB() const {
    return b_;
}

double Trapezoidal::getC() const {
    return c_;
}

double Trapezoidal::getD() const {
    return d_;
}