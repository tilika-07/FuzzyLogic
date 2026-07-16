#include "../include/Triangle.h"
Triangle::Triangle(
    const std::string& name,
    double a,
    double b,
    double c
):
name_(name),a_(a),b_(b),c_(c){
}
double Triangle::compute(double x) const{
    if (a_ == b_)
    {
        if (x <= b_) return 1.0;
        if (x >= c_) return 0.0;
        return (c_ - x) / (c_ - b_);
    }

    if (b_ == c_)
    {
        if (x <= a_) return 0.0;
        if (x >= b_) return 1.0;
        return (x - a_) / (b_ - a_);
    }
    if (x <= a_ || x >= c_)
        return 0.0;

    if (x == b_)
        return 1.0;

    if (x < b_)
        return (x - a_) / (b_ - a_);

    return (c_ - x) / (c_ - b_);
}   
std::string Triangle::getName() const{
    return name_;
}
double Triangle::getLeft() const { return a_; }
double Triangle::getPeak() const {
    return b_;
}
double Triangle::getRight() const {
    return c_;
}