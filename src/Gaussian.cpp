#include "../include/Gaussian.h"
#include <cmath>
Gaussian::Gaussian(const std::string& name,double mean,double sigma):
    name_(name),
    mean_(mean),
    sigma_(sigma){}
double Gaussian::compute(double x) const{
    double e=-std::pow(x-mean_,2.0)/(2.0*sigma_*sigma_);
    return std::exp(e);
}
std::string Gaussian::getName() const{
    return name_;
}

double Gaussian::getMean() const
{
    return mean_;
}
double Gaussian::getSigma() const {
    return sigma_;
}
