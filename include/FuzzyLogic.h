#pragma once
//membership funcs
#include "MembershipFunc.h"		//parent class
#include "Triangle.h"
#include "Trapezoidal.h"
#include "Gaussian.h"
#include "Bell.h"
#include "Sigmoid.h"
#include "Singleton.h"
//variables
#include "FuzzySet.h"
#include "FuzzyVariable.h"
#include "InputVar.h"
#include "OutputVar.h"
#include "InputValue.h"
//rules
#include "Condition.h"
#include "Antecedent.h"
#include "Consequent.h"
#include "Rule.h"
#include "SugenoRule.h"
//sugeno consequents
#include "SugenoConsequent.h"
#include "ConstSugenoConsequent.h"
#include "LinearSugenoConsequent.h"
//ops
#include "TNorm.h"
#include "MinTNorm.h"
#include "ProdTNorm.h"
#include "SNorm.h"
#include "MaxSNorm.h"
#include "ProbSumSNorm.h"
//defuzzifiers
#include "Defuzzifier.h"
#include "CentroidDefuzz.h"
#include "BisectorDefuzz.h"
#include "LOMDefuzz.h"
#include "MOMDefuzz.h"
#include "SOMDefuzz.h"
//inference
#include "RuleEvaluator.h"
#include "Aggregator.h"
#include "InferenceEngine.h"
#include "SugenoInference.h"

#include "FuzzySystem.h"

#include "MamdaniSystem.h"

#include "SugenoSystem.h"