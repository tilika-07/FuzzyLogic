#pragma once
class SNorm{
    public:
        virtual ~SNorm()=default;
        virtual double apply(double a,double b) const=0;
};