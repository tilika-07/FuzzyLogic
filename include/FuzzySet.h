#pragma once
#include <memory>
#include <string>
#include "MembershipFunc.h"

class FuzzySet{
    private:
        std::string name_;
        std::shared_ptr<MembershipFunc> mf_;
    public:
        FuzzySet(
            const std::string& name,
            std::shared_ptr<MembershipFunc> mf);
        double membership(double x) const;      //calls compute func of specified membership function
       
        const std::string& getName() const;
        const MembershipFunc* getMembershipFunction() const;
};