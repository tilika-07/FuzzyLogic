#include <gtest/gtest.h>
#include <algorithm>
#include "../include/Aggregator.h"
#include "../include/OutputVar.h"
#include "../include/Rule.h"
#include "../include/Antecedent.h"
#include "../include/Consequent.h"
#include "../include/Condition.h"
#include "../include/Triangle.h"
#include "../include/FuzzySet.h"
#include "../include/MinTNorm.h"
#include "../include/MaxSNorm.h"
#include "../include/ProdTNorm.h"
#include "../include/ProbSumSNorm.h"

class AggregatorTest : public ::testing::Test
{
protected:

    OutputVar fan{ "Fan",0,100 };

    MinTNorm minNorm;
    ProdTNorm productNorm;

    MaxSNorm maxNorm;
    ProbSumSNorm probSum;

    Aggregator aggregator;

    Rule fastRule;

    void SetUp() override
    {
        fan.addSet(
            FuzzySet(
                "Fast",
                std::make_shared<Triangle>(
                    "Fast", 50, 75, 100)));

        Antecedent ant;

        Consequent cons({
            Condition("Fan","Fast")
            });

        fastRule = Rule(ant, cons);
    }
};
TEST_F(AggregatorTest, SingleRuleProduces500Samples)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 1.0 },
        fan,
        minNorm,
        maxNorm);

    EXPECT_EQ(result.first.size(), 500);
    EXPECT_EQ(result.second.size(), 500);
}
TEST_F(AggregatorTest, XValuesCoverEntireUniverse)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 1.0 },
        fan,
        minNorm,
        maxNorm);

    EXPECT_DOUBLE_EQ(result.first.front(), 0.0);
    EXPECT_DOUBLE_EQ(result.first.back(), 100.0);
}
TEST_F(AggregatorTest, ZeroFiringStrengthProducesZeroMembershipEverywhere)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 0.0 },
        fan,
        minNorm,
        maxNorm);

    for (double mu : result.second)
        EXPECT_DOUBLE_EQ(mu, 0.0);
}
TEST_F(AggregatorTest, MembershipNeverExceedsOne)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 1.0 },
        fan,
        minNorm,
        maxNorm);

    for (double mu : result.second)
    {
        EXPECT_GE(mu, 0.0);
        EXPECT_LE(mu, 1.0);
    }
}
TEST_F(AggregatorTest, MembershipContainsPositiveValues)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 1.0 },
        fan,
        minNorm,
        maxNorm);

    bool found = false;

    for (double mu : result.second)
    {
        if (mu > 0.0)
        {
            found = true;
            break;
        }
    }

    EXPECT_TRUE(found);
}
TEST_F(AggregatorTest, SizeMismatchThrows)
{
    EXPECT_THROW(
        aggregator.aggregate(
            { fastRule },
            { 0.5,0.6 },
            fan,
            minNorm,
            maxNorm),
        std::runtime_error);
}
TEST_F(AggregatorTest, EmptyOutputVariableThrows)
{
    OutputVar empty("Fan", 0, 100);

    EXPECT_THROW(
        aggregator.aggregate(
            { fastRule },
            { 1.0 },
            empty,
            minNorm,
            maxNorm),
        std::runtime_error);
}
TEST_F(AggregatorTest, InvalidRangeThrows)
{
    OutputVar invalid("Fan", 10, 10);

    invalid.addSet(
        FuzzySet(
            "Fast",
            std::make_shared<Triangle>(
                "Fast", 0, 5, 10)));

    EXPECT_THROW(
        aggregator.aggregate(
            { fastRule },
            { 1.0 },
            invalid,
            minNorm,
            maxNorm),
        std::runtime_error);
}
TEST_F(AggregatorTest, ProductImplicationProducesValidMemberships)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 0.6 },
        fan,
        productNorm,
        maxNorm);

    for (double mu : result.second)
    {
        EXPECT_GE(mu, 0.0);
        EXPECT_LE(mu, 1.0);
    }
}
TEST_F(AggregatorTest, ProbabilisticAggregationProducesValidMemberships)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 0.7 },
        fan,
        minNorm,
        probSum);

    for (double mu : result.second)
    {
        EXPECT_GE(mu, 0.0);
        EXPECT_LE(mu, 1.0);
    }
}
TEST_F(AggregatorTest, TwoRulesAggregateCorrectly)
{
    auto result = aggregator.aggregate(
        { fastRule,fastRule },
        { 0.4,0.8 },
        fan,
        minNorm,
        maxNorm);

    EXPECT_EQ(result.first.size(), 500);
    EXPECT_EQ(result.second.size(), 500);
}
TEST_F(AggregatorTest, NoMembershipValueIsNegative)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 0.8 },
        fan,
        minNorm,
        maxNorm);

    for (double mu : result.second)
    {
        EXPECT_GE(mu, 0.0);
    }
}
TEST_F(AggregatorTest, MinimumImplicationClipsPeakToFiringStrength)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 0.6 },
        fan,
        minNorm,
        maxNorm);

    double peak =
        *std::max_element(result.second.begin(), result.second.end());

    EXPECT_NEAR(peak, 0.6, 1e-2);
}
TEST_F(AggregatorTest, ProductImplicationScalesPeakCorrectly)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 0.4 },
        fan,
        productNorm,
        maxNorm);

    double peak =
        *std::max_element(result.second.begin(), result.second.end());

    EXPECT_NEAR(peak, 0.4, 1e-2);
}
TEST_F(AggregatorTest, MaximumAggregationChoosesLargestContribution)
{
    auto result = aggregator.aggregate(
        { fastRule, fastRule },
        { 0.25, 0.8 },
        fan,
        minNorm,
        maxNorm);

    double peak =
        *std::max_element(result.second.begin(), result.second.end());

    EXPECT_NEAR(peak, 0.8, 1e-2);
}
TEST_F(AggregatorTest, ProbabilisticSumAggregatesCorrectly)
{
    auto result = aggregator.aggregate(
        { fastRule, fastRule },
        { 0.3, 0.8 },
        fan,
        minNorm,
        probSum);

    double peak =
        *std::max_element(result.second.begin(), result.second.end());

    EXPECT_NEAR(peak, 0.86, 1e-2);
}
TEST_F(AggregatorTest, FullFiringStrengthPreservesMembershipFunction)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 1.0 },
        fan,
        minNorm,
        maxNorm);

    double peak =
        *std::max_element(result.second.begin(), result.second.end());

    EXPECT_NEAR(peak, 1.0, 1e-2);
}
TEST_F(AggregatorTest, EmptyRuleBaseReturnsZeroMemberships)
{
    auto result = aggregator.aggregate(
        {},
        {},
        fan,
        minNorm,
        maxNorm);

    EXPECT_EQ(result.first.size(), 500);

    for (double mu : result.second)
    {
        EXPECT_DOUBLE_EQ(mu, 0.0);
    }
}
TEST_F(AggregatorTest, XValuesAndMembershipsHaveEqualLength)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 1.0 },
        fan,
        minNorm,
        maxNorm);

    EXPECT_EQ(result.first.size(), result.second.size());
}
TEST_F(AggregatorTest, SamplePointsAreIncreasing)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 1.0 },
        fan,
        minNorm,
        maxNorm);

    for (size_t i = 1; i < result.first.size(); ++i)
    {
        EXPECT_GT(result.first[i], result.first[i - 1]);
    }
}
TEST_F(AggregatorTest, AggregatedCurveContainsPositiveRegion)
{
    auto result = aggregator.aggregate(
        { fastRule },
        { 0.5 },
        fan,
        minNorm,
        maxNorm);

    EXPECT_TRUE(std::any_of(
        result.second.begin(),
        result.second.end(),
        [](double x)
        {
            return x > 0.0;
        }));
}
TEST_F(AggregatorTest, MembershipNeverExceedsFiringStrengthWithMinimumImplication)
{
    constexpr double firing = 0.45;

    auto result = aggregator.aggregate(
        { fastRule },
        { firing },
        fan,
        minNorm,
        maxNorm);

    for (double mu : result.second)
    {
        EXPECT_LE(mu, firing + 1e-6);
    }
}
TEST_F(AggregatorTest, UnknownConsequentSetProducesZeroAggregation)
{
    Consequent cons({
        Condition("Fan", "Turbo")
        });

    Rule badRule(Antecedent(), cons);

    auto result = aggregator.aggregate(
        { badRule },
        { 1.0 },
        fan,
        minNorm,
        maxNorm);

    for (double mu : result.second)
    {
        EXPECT_DOUBLE_EQ(mu, 0.0);
    }
}
TEST_F(AggregatorTest, MultipleConsequentsAreAggregated)
{
    fan.addSet(
        FuzzySet(
            "VeryFast",
            std::make_shared<Triangle>(
                "VeryFast", 75, 90, 100)));

    Consequent cons({
        Condition("Fan", "Fast"),
        Condition("Fan", "VeryFast")
        });

    Rule multiRule(Antecedent(), cons);

    auto result = aggregator.aggregate(
        { multiRule },
        { 1.0 },
        fan,
        minNorm,
        maxNorm);

    EXPECT_TRUE(std::any_of(
        result.second.begin(),
        result.second.end(),
        [](double mu)
        {
            return mu > 0.0;
        }));
}
