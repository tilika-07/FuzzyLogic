#pragma once
class TNorm{
    public:
    virtual ~TNorm()=default;
    virtual double apply(double a,double b) const=0;
};