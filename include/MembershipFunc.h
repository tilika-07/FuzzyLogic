#pragma once
#include <string>
class MembershipFunc{
    public:
    virtual ~MembershipFunc()=default;
    virtual double compute(double x) const=0;
    virtual std::string getName() const=0;
};
