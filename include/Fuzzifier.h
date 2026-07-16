#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "InputVar.h"
class Fuzzifier{
    public:
        using MembershipMap=std::unordered_map<std::string,double>;
        static MembershipMap fuzzify(const InputVar& var,double crispInp);  //user provides crisp numerical input
        static MembershipMap fuzzify(const InputVar& var,const std::string& lingInp);  //user gives fuzzy linguistic input
        static MembershipMap fuzzify(const InputVar& var,const MembershipMap& fuzzyInp); //user gives fuzzy set w alr defined membership values

        };
