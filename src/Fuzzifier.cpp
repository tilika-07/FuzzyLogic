#include "../include/Fuzzifier.h"
Fuzzifier::MembershipMap Fuzzifier::fuzzify(const InputVar& var,double crispInp){
    MembershipMap res;
    for(const auto& set:var.getSets()){
        res[set.getName()]=set.membership(crispInp);
    }
    return res;
}
Fuzzifier::MembershipMap Fuzzifier::fuzzify(const InputVar&var,const std::string& lingInp){
    MembershipMap res;
    bool found=false;
    for(const auto& set:var.getSets()){
        if(lingInp ==set.getName()){
            res[set.getName()]=1.0;
            found=true;
        }
        else{
            res[set.getName()]=0.0;
        }
    }
    if(!found)
    {
        throw std::invalid_argument(
            "Unknown linguistic term: " + lingInp);
    }
    return res;
}
Fuzzifier::MembershipMap
Fuzzifier::fuzzify(
    const InputVar& var,
    const MembershipMap& fuzzyInp)
{
    MembershipMap res;

    for(const auto& set : var.getSets())
    {
        auto it = fuzzyInp.find(
            set.getName());

        if(it != fuzzyInp.end())
        {
            double value = it->second;

            if(value < 0.0)
            {
                value = 0.0;
            }

            if(value > 1.0)
            {
                value = 1.0;
            }

            res[set.getName()] = value;
        }
        else
        {
            res[set.getName()] = 0.0;
        }
    }

    return res;
}