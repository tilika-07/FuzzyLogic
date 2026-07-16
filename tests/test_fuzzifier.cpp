#include <gtest/gtest.h>

#include "../include/Fuzzifier.h"
#include "../include/InputVar.h"
#include "../include/Triangle.h"

class FuzzifierTest : public ::testing::Test
{
protected:
    InputVar temperature{ "Temperature", 0.0, 100.0 };

    void SetUp() override
    {
        temperature.addSet(
            FuzzySet(
                "Cold",
                std::make_shared<Triangle>("Cold",0.0, 0.0, 50.0)));

        temperature.addSet(
            FuzzySet(
                "Warm",
                std::make_shared<Triangle>("Warm",25.0, 50.0, 75.0)));

        temperature.addSet(
            FuzzySet(
                "Hot",
                std::make_shared<Triangle>("Hot",50.0, 100.0, 100.0)));
    }
};
//testing crisp input

TEST_F(FuzzifierTest, CrispInputReturnsMembershipForEverySet)
{
    auto result = Fuzzifier::fuzzify(temperature, 50.0);

    EXPECT_EQ(result.size(), 3);    //generates key for hot,cold and warm
}

TEST_F(FuzzifierTest, CrispInputComputesCorrectMemberships)
{
    auto result = Fuzzifier::fuzzify(temperature, 50.0);

    EXPECT_DOUBLE_EQ(result["Cold"], 0.0);
    EXPECT_DOUBLE_EQ(result["Warm"], 1.0);
    EXPECT_DOUBLE_EQ(result["Hot"], 0.0);
}

TEST_F(FuzzifierTest, CrispInputAtLeftBoundary)
{
    auto result = Fuzzifier::fuzzify(temperature, 0.0);

    EXPECT_DOUBLE_EQ(result["Cold"], 1.0);
    EXPECT_DOUBLE_EQ(result["Warm"], 0.0);
    EXPECT_DOUBLE_EQ(result["Hot"], 0.0);
}

TEST_F(FuzzifierTest, CrispInputAtRightBoundary)
{
    auto result = Fuzzifier::fuzzify(temperature, 100.0);

    EXPECT_DOUBLE_EQ(result["Cold"], 0.0);
    EXPECT_DOUBLE_EQ(result["Warm"], 0.0);
    EXPECT_DOUBLE_EQ(result["Hot"], 1.0);
}

TEST_F(FuzzifierTest, CrispInputOutsideUniverseBelowMinimum)
{
    auto result = Fuzzifier::fuzzify(temperature, -20.0);

    EXPECT_DOUBLE_EQ(result["Cold"], 1.0);
    EXPECT_DOUBLE_EQ(result["Warm"], 0.0);
    EXPECT_DOUBLE_EQ(result["Hot"], 0.0);
}

TEST_F(FuzzifierTest, CrispInputOutsideUniverseAboveMaximum)
{
    auto result = Fuzzifier::fuzzify(temperature, 120.0);

    EXPECT_DOUBLE_EQ(result["Cold"], 0.0);
    EXPECT_DOUBLE_EQ(result["Warm"], 0.0);
    EXPECT_DOUBLE_EQ(result["Hot"], 1.0);
}
TEST_F(FuzzifierTest, CrispInputProducesColdWarmOverlap)
{
    auto result = Fuzzifier::fuzzify(temperature, 37.5);

    EXPECT_NEAR(result["Cold"], 0.25, 1e-6);
    EXPECT_NEAR(result["Warm"], 0.50, 1e-6);
    EXPECT_NEAR(result["Hot"], 0.0, 1e-6);
}
TEST_F(FuzzifierTest, CrispInputProducesWarmHotOverlap)
{
    auto result = Fuzzifier::fuzzify(temperature, 62.5);

    EXPECT_NEAR(result["Cold"], 0.0, 1e-6);
    EXPECT_NEAR(result["Warm"], 0.5, 1e-6);
    EXPECT_NEAR(result["Hot"], 0.25, 1e-6);
}
//testing for linguistic input

TEST_F(FuzzifierTest, LinguisticInputAssignsMembershipCorrectly)
{
    auto result = Fuzzifier::fuzzify(temperature, "Warm");
    EXPECT_EQ(result.size(), 3);
    EXPECT_DOUBLE_EQ(result["Warm"], 1.0);    
    EXPECT_DOUBLE_EQ(result["Cold"], 0.0);
    EXPECT_DOUBLE_EQ(result["Hot"], 0.0);
}


TEST_F(FuzzifierTest, UnknownLinguisticTermThrows)
{
    EXPECT_THROW(
        Fuzzifier::fuzzify(temperature, "VeryHot"),
        std::invalid_argument);
}

//
// Fuzzy input tests
//

TEST_F(FuzzifierTest, FuzzyInputCopiesCorrectly)
{
    Fuzzifier::MembershipMap input{
        {"Cold", 0.3},
        {"Warm", 0.7}
    };

    auto result = Fuzzifier::fuzzify(temperature, input);

    EXPECT_DOUBLE_EQ(result["Cold"], 0.3);
    EXPECT_DOUBLE_EQ(result["Warm"], 0.7);
    EXPECT_DOUBLE_EQ(result["Hot"], 0.0);   //missing set gets 0 membership
}


TEST_F(FuzzifierTest, MembershipGreaterThanOneIsClamped)
{
    Fuzzifier::MembershipMap input{
        {"Hot", 5.0}
    };

    auto result = Fuzzifier::fuzzify(temperature, input);

    EXPECT_DOUBLE_EQ(result["Hot"], 1.0);
}

TEST_F(FuzzifierTest, MembershipLessThanZeroIsClamped)
{
    Fuzzifier::MembershipMap input{
        {"Cold", -2.5}
    };

    auto result = Fuzzifier::fuzzify(temperature, input);

    EXPECT_DOUBLE_EQ(result["Cold"], 0.0);
}

TEST_F(FuzzifierTest, EmptyFuzzyInputReturnsAllZeros)
{
    Fuzzifier::MembershipMap input;

    auto result = Fuzzifier::fuzzify(temperature, input);

    EXPECT_DOUBLE_EQ(result["Cold"], 0.0);
    EXPECT_DOUBLE_EQ(result["Warm"], 0.0);
    EXPECT_DOUBLE_EQ(result["Hot"], 0.0);
}

TEST_F(FuzzifierTest, UnknownFuzzySetIsIgnored)
{
    Fuzzifier::MembershipMap input{
        {"Unknown", 1.0},
        {"Warm", 0.5}
    };

    auto result = Fuzzifier::fuzzify(temperature, input);

    EXPECT_EQ(result.size(), 3);

    EXPECT_DOUBLE_EQ(result["Cold"], 0.0);
    EXPECT_DOUBLE_EQ(result["Warm"], 0.5);
    EXPECT_DOUBLE_EQ(result["Hot"], 0.0);
}