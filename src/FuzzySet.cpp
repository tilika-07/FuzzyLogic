#include "../include/FuzzySet.h"
FuzzySet::FuzzySet(
    const std::string& name,
    std::shared_ptr<MembershipFunc> mf):
    name_(name),
    mf_(std::move(mf)){}
double FuzzySet::membership(double x) const{
    return mf_->compute(x);
}


const std::string& FuzzySet::getName() const{
    return name_;
}
const MembershipFunc*
FuzzySet::getMembershipFunction() const
{
    return mf_.get();
}