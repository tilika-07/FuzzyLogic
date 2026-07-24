#include <ML/Data/Dataset.h>
#include <gtest/gtest.h>
#include "ML/Generation/FuzzySetGenerator.h"
#include "../include/Gaussian.h"
#include "Triangle.h"
#include "Trapezoidal.h"
Dataset createGaussianDataset()
{
    return Dataset(
        {
            {10},
            {11},
            {12},
            {50},
            {51},
            {52},
            {90},
            {91},
            {92}
        },
        {
            "Temperature"
        }
        );
}
TEST(FuzzySetGeneratorTest, GeneratesRequestedNumberOfGaussianSets)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    EXPECT_EQ(sets.size(), 3u);
}
TEST(FuzzySetGeneratorTest, GaussianNamesAreCorrect)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    EXPECT_EQ(sets[0]->getName(), "Temperature_1");
    EXPECT_EQ(sets[1]->getName(), "Temperature_2");
    EXPECT_EQ(sets[2]->getName(), "Temperature_3");
}
TEST(FuzzySetGeneratorTest, GeneratedMembershipFunctionsAreGaussian)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        EXPECT_NE(
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction()
                ),
            nullptr
        );
    }
}
TEST(FuzzySetGeneratorTest, GaussianMeansAreSorted)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    auto g1 =
        dynamic_cast<const Gaussian*>(
            sets[0]->getMembershipFunction());

    auto g2 =
        dynamic_cast<const Gaussian*>(
            sets[1]->getMembershipFunction());

    auto g3 =
        dynamic_cast<const Gaussian*>(
            sets[2]->getMembershipFunction());

    ASSERT_NE(g1, nullptr);
    ASSERT_NE(g2, nullptr);
    ASSERT_NE(g3, nullptr);

    EXPECT_LT(g1->getMean(), g2->getMean());
    EXPECT_LT(g2->getMean(), g3->getMean());
}
TEST(FuzzySetGeneratorTest, GaussianMeansLieInsideDatasetRange)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto g =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction());

        ASSERT_NE(g, nullptr);

        EXPECT_GE(g->getMean(), 10.0);
        EXPECT_LE(g->getMean(), 92.0);
    }
}
TEST(FuzzySetGeneratorTest, GaussianSigmasArePositive)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto g =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction());

        ASSERT_NE(g, nullptr);

        EXPECT_GT(g->getSigma(), 0.0);
    }
}
TEST(FuzzySetGeneratorTest, GaussianSigmasAreFinite)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto g =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction());

        ASSERT_NE(g, nullptr);

        EXPECT_TRUE(
            std::isfinite(
                g->getSigma()));
    }
}
TEST(FuzzySetGeneratorTest, GaussianMeansAreFinite)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto g =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction());

        ASSERT_NE(g, nullptr);

        EXPECT_TRUE(
            std::isfinite(
                g->getMean()));
    }
}
TEST(FuzzySetGeneratorTest, GaussianPeaksAtMean)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto g =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction());

        ASSERT_NE(g, nullptr);

        EXPECT_NEAR(
            set->membership(
                g->getMean()),
            1.0,
            1e-12);
    }
}
TEST(FuzzySetGeneratorTest, GaussianMembershipIsSymmetric)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    constexpr double d = 0.5;

    for (const auto& set : sets)
    {
        auto g =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction());

        ASSERT_NE(g, nullptr);

        EXPECT_NEAR(
            set->membership(g->getMean() - d),
            set->membership(g->getMean() + d),
            1e-12);
    }
}
TEST(FuzzySetGeneratorTest, GaussianMembershipMatchesAnalyticalEquation)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto gaussian =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction()
                );

        ASSERT_NE(gaussian, nullptr);

        double mean = gaussian->getMean();
        double sigma = gaussian->getSigma();

        double x = mean + 0.75 * sigma;

        double expected =
            std::exp(
                -(x - mean) * (x - mean) /
                (2.0 * sigma * sigma)
            );

        EXPECT_NEAR(
            set->membership(x),
            expected,
            1e-12
        );
    }
}
TEST(FuzzySetGeneratorTest, GaussianMembershipDecreasesWithDistance)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto gaussian =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction()
                );

        ASSERT_NE(gaussian, nullptr);

        double c = gaussian->getMean();
        double s = gaussian->getSigma();

        EXPECT_GT(
            set->membership(c),
            set->membership(c + s)
        );

        EXPECT_GT(
            set->membership(c + s),
            set->membership(c + 2.0 * s)
        );

        EXPECT_GT(
            set->membership(c + 2.0 * s),
            set->membership(c + 3.0 * s)
        );
    }
}
TEST(FuzzySetGeneratorTest, GaussianHasHighestMembershipAtItsOwnCenter)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    for (size_t i = 0; i < sets.size(); ++i)
    {
        auto gaussian =
            dynamic_cast<const Gaussian*>(
                sets[i]->getMembershipFunction()
                );

        ASSERT_NE(gaussian, nullptr);

        double center = gaussian->getMean();
        double ownMembership =
            sets[i]->membership(center);

        for (size_t j = 0; j < sets.size(); ++j)
        {
            if (i == j)
            {
                continue;
            }

            EXPECT_GE(
                ownMembership,
                sets[j]->membership(center)
            );
        }
    }
}
TEST(FuzzySetGeneratorTest, GeneratedMeansAreUnique)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    std::set<double> means;

    for (const auto& set : sets)
    {
        auto gaussian =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction()
                );

        ASSERT_NE(gaussian, nullptr);

        means.insert(
            gaussian->getMean()
        );
    }

    EXPECT_EQ(
        means.size(),
        sets.size()
    );
}
TEST(FuzzySetGeneratorTest, UniformDatasetProducesFiniteGaussian)
{
    Dataset dataset(
        {
            {5},
            {5},
            {5},
            {5},
            {5}
        },
        {
            "Feature"
        }
        );

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            1
        );

    auto gaussian =
        dynamic_cast<const Gaussian*>(
            sets.front()->getMembershipFunction()
            );

    ASSERT_NE(gaussian, nullptr);

    EXPECT_TRUE(
        std::isfinite(
            gaussian->getMean()
        )
    );

    EXPECT_TRUE(
        std::isfinite(
            gaussian->getSigma()
        )
    );

    EXPECT_GT(
        gaussian->getSigma(),
        0.0
    );
}
TEST(FuzzySetGeneratorTest, NegativeDatasetGeneratesValidGaussians)
{
    Dataset dataset(
        {
            {-30},
            {-20},
            {-10},
            {-5},
            {-2}
        },
        {
            "Feature"
        }
        );

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            2
        );

    for (const auto& set : sets)
    {
        auto gaussian =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction()
                );

        ASSERT_NE(gaussian, nullptr);

        EXPECT_LT(
            gaussian->getMean(),
            0.0
        );

        EXPECT_GT(
            gaussian->getSigma(),
            0.0
        );
    }
}
TEST(FuzzySetGeneratorTest, DecimalDatasetProducesFiniteParameters)
{
    Dataset dataset(
        {
            {1.25},
            {1.80},
            {2.15},
            {5.40},
            {5.95},
            {6.30}
        },
        {
            "Feature"
        }
        );

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            2
        );

    for (const auto& set : sets)
    {
        auto gaussian =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction()
                );

        ASSERT_NE(gaussian, nullptr);

        EXPECT_TRUE(
            std::isfinite(
                gaussian->getMean()
            )
        );

        EXPECT_TRUE(
            std::isfinite(
                gaussian->getSigma()
            )
        );
    }
}
TEST(FuzzySetGeneratorTest, DuplicateValuesGenerateValidGaussians)
{
    Dataset dataset(
        {
            {10},
            {10},
            {10},
            {50},
            {50},
            {50},
            {90},
            {90},
            {90}
        },
        {
            "Feature"
        }
        );

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    EXPECT_EQ(
        sets.size(),
        3u
    );

    for (const auto& set : sets)
    {
        auto gaussian =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction()
                );

        ASSERT_NE(gaussian, nullptr);

        EXPECT_GT(
            gaussian->getSigma(),
            0.0
        );
    }
}
TEST(FuzzySetGeneratorTest, SingleGaussianClusterGeneration)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            1
        );

    ASSERT_EQ(
        sets.size(),
        1u
    );

    auto gaussian =
        dynamic_cast<const Gaussian*>(
            sets.front()->getMembershipFunction()
            );

    ASSERT_NE(
        gaussian,
        nullptr
    );

    EXPECT_GT(
        gaussian->getSigma(),
        0.0
    );
}
TEST(FuzzySetGeneratorTest, InvalidFeatureIndexThrows)
{
    Dataset dataset =
        createGaussianDataset();

    EXPECT_THROW(
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            10,
            3
        ),
        std::out_of_range
    );
}
TEST(FuzzySetGeneratorTest, GenerateGaussianVariableHasCorrectName)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateGaussianVariable(
            dataset,
            0,
            3
        );

    EXPECT_EQ(
        variable.getName(),
        "Temperature"
    );
}
TEST(FuzzySetGeneratorTest, GenerateGaussianVariableContainsCorrectNumberOfSets)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateGaussianVariable(
            dataset,
            0,
            3
        );

    EXPECT_EQ(
        variable.getSets().size(),
        3u
    );
}
TEST(FuzzySetGeneratorTest, GenerateGaussianVariableStoresDatasetRange)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateGaussianVariable(
            dataset,
            0,
            3
        );

    EXPECT_DOUBLE_EQ(
        variable.getMinValue(),
        10.0
    );

    EXPECT_DOUBLE_EQ(
        variable.getMaxValue(),
        92.0
    );
}
TEST(FuzzySetGeneratorTest, GenerateGaussianVariablePreservesSetNames)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateGaussianVariable(
            dataset,
            0,
            3
        );

    const auto& sets =
        variable.getSets();

    ASSERT_EQ(
        sets.size(),
        3u
    );

    EXPECT_EQ(
        sets[0].getName(),
        "Temperature_1"
    );

    EXPECT_EQ(
        sets[1].getName(),
        "Temperature_2"
    );

    EXPECT_EQ(
        sets[2].getName(),
        "Temperature_3"
    );
}
TEST(FuzzySetGeneratorTest, GenerateGaussianVariableStoresGaussianMembershipFunctions)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateGaussianVariable(
            dataset,
            0,
            3
        );

    for (const auto& set : variable.getSets())
    {
        EXPECT_NE(
            dynamic_cast<const Gaussian*>(
                set.getMembershipFunction()
                ),
            nullptr
        );
    }
}
TEST(FuzzySetGeneratorTest, GeneratedGaussianVariableMembershipFunctionsRemainValid)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateGaussianVariable(
            dataset,
            0,
            3
        );

    for (const auto& set : variable.getSets())
    {
        auto gaussian =
            dynamic_cast<const Gaussian*>(
                set.getMembershipFunction()
                );

        ASSERT_NE(
            gaussian,
            nullptr
        );

        EXPECT_NEAR(
            set.membership(
                gaussian->getMean()
            ),
            1.0,
            1e-12
        );
    }
}
TEST(FuzzySetGeneratorTest, GaussianVariableMatchesGeneratedSets)
{
    Dataset dataset = createGaussianDataset();

    auto generated =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        );

    FuzzyVariable variable =
        FuzzySetGenerator::generateGaussianVariable(
            dataset,
            0,
            3
        );

    ASSERT_EQ(
        generated.size(),
        variable.getSets().size()
    );

    for (size_t i = 0; i < generated.size(); ++i)
    {
        EXPECT_EQ(
            generated[i]->getName(),
            variable.getSets()[i].getName()
        );

        auto g1 =
            dynamic_cast<const Gaussian*>(
                generated[i]->getMembershipFunction()
                );

        auto g2 =
            dynamic_cast<const Gaussian*>(
                variable.getSets()[i].getMembershipFunction()
                );

        ASSERT_NE(g1, nullptr);
        ASSERT_NE(g2, nullptr);

        EXPECT_NEAR(
            g1->getMean(),
            g2->getMean(),
            1e-3
        );

        EXPECT_NEAR(
            g1->getSigma(),
            g2->getSigma(),
            1e-6
        );
    }
}
TEST(FuzzySetGeneratorTest, GenerateGaussianVariableInvalidFeatureThrows)
{
    Dataset dataset = createGaussianDataset();

    EXPECT_THROW(
        FuzzySetGenerator::generateGaussianVariable(
            dataset,
            10,
            3
        ),
        std::out_of_range
    );
}
TEST(FuzzySetGeneratorTest, GenerateGaussianVariableSingleCluster)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateGaussianVariable(
            dataset,
            0,
            1
        );

    ASSERT_EQ(
        variable.getSets().size(),
        1u
    );

    auto gaussian =
        dynamic_cast<const Gaussian*>(
            variable.getSets()[0].getMembershipFunction()
            );

    ASSERT_NE(
        gaussian,
        nullptr
    );

    EXPECT_GT(
        gaussian->getSigma(),
        0.0
    );
}
TEST(FuzzySetGeneratorTest, GenerateGaussianVariableGetSetByName)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateGaussianVariable(
            dataset,
            0,
            3
        );

    EXPECT_NE(
        variable.getSet("Temperature_1"),
        nullptr
    );

    EXPECT_NE(
        variable.getSet("Temperature_2"),
        nullptr
    );

    EXPECT_NE(
        variable.getSet("Temperature_3"),
        nullptr
    );

    EXPECT_EQ(
        variable.getSet("Unknown"),
        nullptr
    );
}
TEST(FuzzySetGeneratorTest, GeneratesRequestedNumberOfTriangleSets)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    EXPECT_EQ(
        sets.size(),
        3u
    );
}
TEST(FuzzySetGeneratorTest, TriangleNamesAreCorrect)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    EXPECT_EQ(sets[0]->getName(), "Temperature_1");
    EXPECT_EQ(sets[1]->getName(), "Temperature_2");
    EXPECT_EQ(sets[2]->getName(), "Temperature_3");
}
TEST(FuzzySetGeneratorTest, GeneratedMembershipFunctionsAreTriangle)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        EXPECT_NE(
            dynamic_cast<const Triangle*>(
                set->getMembershipFunction()
                ),
            nullptr
        );
    }
}
TEST(FuzzySetGeneratorTest, TrianglePeaksAreSorted)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    auto t1 =
        dynamic_cast<const Triangle*>(
            sets[0]->getMembershipFunction());

    auto t2 =
        dynamic_cast<const Triangle*>(
            sets[1]->getMembershipFunction());

    auto t3 =
        dynamic_cast<const Triangle*>(
            sets[2]->getMembershipFunction());

    ASSERT_NE(t1, nullptr);
    ASSERT_NE(t2, nullptr);
    ASSERT_NE(t3, nullptr);

    EXPECT_LT(
        t1->getPeak(),
        t2->getPeak());

    EXPECT_LT(
        t2->getPeak(),
        t3->getPeak());
}
TEST(FuzzySetGeneratorTest, TrianglePeaksLieInsideDatasetRange)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto triangle =
            dynamic_cast<const Triangle*>(
                set->getMembershipFunction());

        ASSERT_NE(
            triangle,
            nullptr);

        EXPECT_GE(
            triangle->getPeak(),
            10.0);

        EXPECT_LE(
            triangle->getPeak(),
            92.0);
    }
}
TEST(FuzzySetGeneratorTest, TriangleParametersAreOrdered)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto triangle =
            dynamic_cast<const Triangle*>(
                set->getMembershipFunction());

        ASSERT_NE(
            triangle,
            nullptr);

        EXPECT_LT(
            triangle->getLeft(),
            triangle->getPeak());

        EXPECT_LT(
            triangle->getPeak(),
            triangle->getRight());
    }
}
TEST(FuzzySetGeneratorTest, TriangleMembershipEqualsOneAtPeak)
{
    Dataset dataset =
        createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto triangle =
            dynamic_cast<const Triangle*>(
                set->getMembershipFunction());

        ASSERT_NE(
            triangle,
            nullptr);

        EXPECT_DOUBLE_EQ(
            set->membership(
                triangle->getPeak()),
            1.0);
    }
}
TEST(FuzzySetGeneratorTest, TriangleMembershipOutsideSupportIsZero)
{
    Dataset dataset =
        createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto triangle =
            dynamic_cast<const Triangle*>(
                set->getMembershipFunction());

        ASSERT_NE(
            triangle,
            nullptr);

        EXPECT_DOUBLE_EQ(
            set->membership(
                triangle->getLeft() - 1.0),
            0.0);

        EXPECT_DOUBLE_EQ(
            set->membership(
                triangle->getRight() + 1.0),
            0.0);
    }
}
TEST(FuzzySetGeneratorTest, TriangleMembershipIncreasesLinearly)
{
    Dataset dataset =
        createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto triangle =
            dynamic_cast<const Triangle*>(
                set->getMembershipFunction());

        ASSERT_NE(
            triangle,
            nullptr);

        double left =
            triangle->getLeft();

        double peak =
            triangle->getPeak();

        double x1 =
            (left + peak) / 2.0;

        double x2 =
            (x1 + peak) / 2.0;

        EXPECT_LT(
            set->membership(x1),
            set->membership(x2));
    }
}
TEST(FuzzySetGeneratorTest, TriangleMembershipDecreasesLinearly)
{
    Dataset dataset =
        createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto triangle =
            dynamic_cast<const Triangle*>(
                set->getMembershipFunction());

        ASSERT_NE(
            triangle,
            nullptr);

        double peak =
            triangle->getPeak();

        double right =
            triangle->getRight();

        double x1 =
            (peak + right) / 2.0;

        double x2 =
            (x1 + right) / 2.0;

        EXPECT_GT(
            set->membership(x1),
            set->membership(x2));
    }
}
TEST(FuzzySetGeneratorTest, FirstTriangleStartsAtDatasetMinimum)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    auto triangle =
        dynamic_cast<const Triangle*>(
            sets.front()->getMembershipFunction()
            );

    ASSERT_NE(triangle, nullptr);

    EXPECT_DOUBLE_EQ(
        triangle->getLeft(),
        10.0
    );
}
TEST(FuzzySetGeneratorTest, LastTriangleEndsAtDatasetMaximum)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    auto triangle =
        dynamic_cast<const Triangle*>(
            sets.back()->getMembershipFunction()
            );

    ASSERT_NE(triangle, nullptr);

    EXPECT_DOUBLE_EQ(
        triangle->getRight(),
        92.0
    );
}
TEST(FuzzySetGeneratorTest, AdjacentTrianglesOverlap)
{
    Dataset dataset =
        createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    for (size_t i = 0; i < sets.size() - 1; i++)
    {
        auto t1 =
            dynamic_cast<const Triangle*>(
                sets[i]->getMembershipFunction());

        auto t2 =
            dynamic_cast<const Triangle*>(
                sets[i + 1]->getMembershipFunction());

        ASSERT_NE(t1, nullptr);
        ASSERT_NE(t2, nullptr);

        EXPECT_GT(
            t1->getRight(),
            t2->getLeft()
        );
    }
}

TEST(FuzzySetGeneratorTest, TrianglePeaksAreUnique)
{
    Dataset dataset =
        createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    std::set<double> peaks;

    for (const auto& set : sets)
    {
        auto triangle =
            dynamic_cast<const Triangle*>(
                set->getMembershipFunction());

        ASSERT_NE(triangle, nullptr);

        peaks.insert(
            triangle->getPeak()
        );
    }

    EXPECT_EQ(
        peaks.size(),
        sets.size()
    );
}
TEST(FuzzySetGeneratorTest, SingleTriangleSpansDataset)
{
    Dataset dataset =
        createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            1
        );

    ASSERT_EQ(
        sets.size(),
        1u
    );

    auto triangle =
        dynamic_cast<const Triangle*>(
            sets[0]->getMembershipFunction());

    ASSERT_NE(
        triangle,
        nullptr
    );

    EXPECT_DOUBLE_EQ(
        triangle->getLeft(),
        10.0
    );

    EXPECT_DOUBLE_EQ(
        triangle->getRight(),
        92.0
    );
}
TEST(FuzzySetGeneratorTest, DuplicateValuesGenerateValidTriangles)
{
    Dataset dataset(
        {
            {10},
            {10},
            {10},
            {50},
            {50},
            {50},
            {90},
            {90}
        },
        {
            "Feature"
        }
        );

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    EXPECT_EQ(
        sets.size(),
        3u
    );

    for (const auto& set : sets)
    {
        auto triangle =
            dynamic_cast<const Triangle*>(
                set->getMembershipFunction());

        ASSERT_NE(
            triangle,
            nullptr
        );

        EXPECT_LT(
            triangle->getLeft(),
            triangle->getPeak()
        );

        EXPECT_LT(
            triangle->getPeak(),
            triangle->getRight()
        );
    }
}
TEST(FuzzySetGeneratorTest, NegativeDatasetProducesValidTriangles)
{
    Dataset dataset(
        {
            {-40},
            {-35},
            {-30},
            {-10},
            {-5},
            {-2}
        },
        {
            "Feature"
        }
        );

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            2
        );

    for (const auto& set : sets)
    {
        auto triangle =
            dynamic_cast<const Triangle*>(
                set->getMembershipFunction());

        ASSERT_NE(
            triangle,
            nullptr
        );

        EXPECT_LT(
            triangle->getPeak(),
            0.0
        );
    }
}
TEST(FuzzySetGeneratorTest, TriangleGenerationInvalidFeatureThrows)
{
    Dataset dataset =
        createGaussianDataset();

    EXPECT_THROW(
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            4,
            3
        ),
        std::out_of_range
    );
}
TEST(FuzzySetGeneratorTest, EmptyDatasetThrowsDuringTriangleGeneration)
{
    Dataset dataset(
        {},
        {
            "Feature"
        }
        );

    EXPECT_THROW(
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            2
        ),
        std::exception
    );
}
TEST(FuzzySetGeneratorTest, GenerateTriangleVariableHasCorrectName)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTriangleVariable(
            dataset,
            0,
            3
        );

    EXPECT_EQ(
        variable.getName(),
        "Temperature"
    );
}
TEST(FuzzySetGeneratorTest, GenerateTriangleVariableStoresDatasetRange)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTriangleVariable(
            dataset,
            0,
            3
        );

    EXPECT_DOUBLE_EQ(
        variable.getMinValue(),
        10.0
    );

    EXPECT_DOUBLE_EQ(
        variable.getMaxValue(),
        92.0
    );
}
TEST(FuzzySetGeneratorTest, GenerateTriangleVariableContainsCorrectNumberOfSets)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTriangleVariable(
            dataset,
            0,
            3
        );

    EXPECT_EQ(
        variable.getSets().size(),
        3u
    );
}
TEST(FuzzySetGeneratorTest, GenerateTriangleVariablePreservesSetNames)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTriangleVariable(
            dataset,
            0,
            3
        );

    const auto& sets =
        variable.getSets();

    ASSERT_EQ(
        sets.size(),
        3u
    );

    EXPECT_EQ(sets[0].getName(), "Temperature_1");
    EXPECT_EQ(sets[1].getName(), "Temperature_2");
    EXPECT_EQ(sets[2].getName(), "Temperature_3");
}
TEST(FuzzySetGeneratorTest, GenerateTriangleVariableStoresTriangleMembershipFunctions)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTriangleVariable(
            dataset,
            0,
            3
        );

    for (const auto& set : variable.getSets())
    {
        EXPECT_NE(
            dynamic_cast<const Triangle*>(
                set.getMembershipFunction()
                ),
            nullptr
        );
    }
}
TEST(FuzzySetGeneratorTest, GenerateTriangleVariableMembershipFunctionsRemainValid)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTriangleVariable(
            dataset,
            0,
            3
        );

    for (const auto& set : variable.getSets())
    {
        auto triangle =
            dynamic_cast<const Triangle*>(
                set.getMembershipFunction());

        ASSERT_NE(
            triangle,
            nullptr
        );

        EXPECT_DOUBLE_EQ(
            set.membership(
                triangle->getPeak()
            ),
            1.0
        );
    }
}
TEST(FuzzySetGeneratorTest, TriangleVariableMatchesGeneratedSets)
{
    Dataset dataset = createGaussianDataset();

    auto generated =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        );

    FuzzyVariable variable =
        FuzzySetGenerator::generateTriangleVariable(
            dataset,
            0,
            3
        );

    ASSERT_EQ(
        generated.size(),
        variable.getSets().size()
    );

    for (size_t i = 0; i < generated.size(); i++)
    {
        EXPECT_EQ(
            generated[i]->getName(),
            variable.getSets()[i].getName()
        );
    }
}
TEST(FuzzySetGeneratorTest, GenerateTriangleVariableGetSetByName)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTriangleVariable(
            dataset,
            0,
            3
        );

    EXPECT_NE(
        variable.getSet("Temperature_1"),
        nullptr
    );

    EXPECT_NE(
        variable.getSet("Temperature_2"),
        nullptr
    );

    EXPECT_NE(
        variable.getSet("Temperature_3"),
        nullptr
    );

    EXPECT_EQ(
        variable.getSet("Unknown"),
        nullptr
    );
}
TEST(FuzzySetGeneratorTest, GenerateTriangleVariableInvalidFeatureThrows)
{
    Dataset dataset = createGaussianDataset();

    EXPECT_THROW(
        FuzzySetGenerator::generateTriangleVariable(
            dataset,
            5,
            3
        ),
        std::out_of_range
    );
}
TEST(FuzzySetGeneratorTest, TrapezoidalNamesAreCorrect)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    EXPECT_EQ(sets[0]->getName(), "Temperature_1");
    EXPECT_EQ(sets[1]->getName(), "Temperature_2");
    EXPECT_EQ(sets[2]->getName(), "Temperature_3");
}
TEST(FuzzySetGeneratorTest, GeneratedMembershipFunctionsAreTrapezoidal)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        EXPECT_NE(
            dynamic_cast<const Trapezoidal*>(
                set->getMembershipFunction()
                ),
            nullptr
        );
    }
}
TEST(FuzzySetGeneratorTest, TrapezoidCentersAreSorted)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    std::vector<double> centers;

    for (const auto& set : sets)
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                set->getMembershipFunction());

        ASSERT_NE(trap, nullptr);

        centers.push_back(
            (trap->getB() + trap->getC()) / 2.0
        );
    }

    EXPECT_LT(centers[0], centers[1]);
    EXPECT_LT(centers[1], centers[2]);
}
TEST(FuzzySetGeneratorTest, TrapezoidParametersAreOrdered)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                set->getMembershipFunction());

        ASSERT_NE(trap, nullptr);

        EXPECT_LE(trap->getA(), trap->getB());
        EXPECT_LE(trap->getB(), trap->getC());
        EXPECT_LE(trap->getC(), trap->getD());
    }
}
TEST(FuzzySetGeneratorTest, TrapezoidPlateauMembershipIsOne)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                set->getMembershipFunction());

        ASSERT_NE(trap, nullptr);

        EXPECT_DOUBLE_EQ(
            set->membership(trap->getB()),
            1.0);

        EXPECT_DOUBLE_EQ(
            set->membership(trap->getC()),
            1.0);

        EXPECT_DOUBLE_EQ(
            set->membership(
                (trap->getB() + trap->getC()) / 2.0),
            1.0);
    }
}
TEST(FuzzySetGeneratorTest, TrapezoidOutsideSupportIsZero)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                set->getMembershipFunction());

        ASSERT_NE(trap, nullptr);

        EXPECT_DOUBLE_EQ(
            set->membership(trap->getA() - 1.0),
            0.0);

        EXPECT_DOUBLE_EQ(
            set->membership(trap->getD() + 1.0),
            0.0);
    }
}
TEST(FuzzySetGeneratorTest, TrapezoidLeftRampIncreases)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                set->getMembershipFunction());

        ASSERT_NE(trap, nullptr);

        if (trap->getA() == trap->getB())
            continue;

        double x1 =
            (trap->getA() + trap->getB()) / 2.0;

        double x2 =
            (x1 + trap->getB()) / 2.0;

        EXPECT_LT(
            set->membership(x1),
            set->membership(x2));
    }
}
TEST(FuzzySetGeneratorTest, TrapezoidRightRampDecreases)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                set->getMembershipFunction());

        ASSERT_NE(trap, nullptr);

        if (trap->getC() == trap->getD())
            continue;

        double x1 =
            (trap->getC() + trap->getD()) / 2.0;

        double x2 =
            (x1 + trap->getD()) / 2.0;

        EXPECT_GT(
            set->membership(x1),
            set->membership(x2));
    }
}
TEST(FuzzySetGeneratorTest, TrapezoidMembershipAlwaysWithinUnitInterval)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    for (const auto& set : sets)
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                set->getMembershipFunction());

        ASSERT_NE(trap, nullptr);

        for (double x = trap->getA() - 2;
            x <= trap->getD() + 2;
            x += 0.25)
        {
            double mu =
                set->membership(x);

            EXPECT_GE(mu, 0.0);
            EXPECT_LE(mu, 1.0);
        }
    }
}
TEST(FuzzySetGeneratorTest, FirstTrapezoidStartsAtDatasetMinimum)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    auto trap =
        dynamic_cast<const Trapezoidal*>(
            sets.front()->getMembershipFunction());

    ASSERT_NE(trap, nullptr);

    EXPECT_DOUBLE_EQ(
        trap->getA(),
        10.0
    );
}
TEST(FuzzySetGeneratorTest, AdjacentTrapezoidsOverlap)
{
    Dataset dataset =
        createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    for (size_t i = 0; i + 1 < sets.size(); i++)
    {
        auto t1 =
            dynamic_cast<const Trapezoidal*>(
                sets[i]->getMembershipFunction());

        auto t2 =
            dynamic_cast<const Trapezoidal*>(
                sets[i + 1]->getMembershipFunction());

        ASSERT_NE(t1, nullptr);
        ASSERT_NE(t2, nullptr);

        EXPECT_GT(
            t1->getD(),
            t2->getA()
        );
    }
}
TEST(FuzzySetGeneratorTest, AdjacentTrapezoidsIntersect)
{
    Dataset dataset =
        createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    for (size_t i = 0; i + 1 < sets.size(); i++)
    {
        auto t1 =
            dynamic_cast<const Trapezoidal*>(
                sets[i]->getMembershipFunction());

        auto t2 =
            dynamic_cast<const Trapezoidal*>(
                sets[i + 1]->getMembershipFunction());

        ASSERT_NE(t1, nullptr);
        ASSERT_NE(t2, nullptr);

        double overlapStart =
            std::max(
                t1->getA(),
                t2->getA());

        double overlapEnd =
            std::min(
                t1->getD(),
                t2->getD());

        EXPECT_LT(
            overlapStart,
            overlapEnd
        );
    }
}
TEST(FuzzySetGeneratorTest, InteriorTrapezoidsHavePositivePlateauWidth)
{
    Dataset dataset =
        createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    for (size_t i = 1; i + 1 < sets.size(); i++)
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                sets[i]->getMembershipFunction());

        ASSERT_NE(
            trap,
            nullptr
        );

        EXPECT_LT(
            trap->getB(),
            trap->getC()
        );
    }
}
TEST(FuzzySetGeneratorTest, DuplicateValuesGenerateValidTrapezoids)
{
    Dataset dataset(
        {
            {10},
            {10},
            {10},
            {50},
            {50},
            {50},
            {90},
            {90},
            {90}
        },
        {
            "Feature"
        }
        );

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    EXPECT_EQ(
        sets.size(),
        3u
    );

    for (const auto& set : sets)
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                set->getMembershipFunction());

        ASSERT_NE(
            trap,
            nullptr
        );

        EXPECT_LE(trap->getA(), trap->getB());
        EXPECT_LE(trap->getB(), trap->getC());
        EXPECT_LE(trap->getC(), trap->getD());
    }
}
TEST(FuzzySetGeneratorTest, NegativeDatasetProducesValidTrapezoids)
{
    Dataset dataset(
        {
            {-40},
            {-35},
            {-30},
            {-10},
            {-5},
            {-2}
        },
        {
            "Feature"
        }
        );

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            2
        );

    for (const auto& set : sets)
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                set->getMembershipFunction());

        ASSERT_NE(
            trap,
            nullptr
        );

        EXPECT_LT(
            (trap->getB() + trap->getC()) / 2.0,
            0.0
        );
    }
}
TEST(FuzzySetGeneratorTest, DecimalDatasetProducesFiniteTrapezoids)
{
    Dataset dataset(
        {
            {1.2},
            {1.8},
            {2.4},
            {5.5},
            {5.8},
            {6.3}
        },
        {
            "Feature"
        }
        );

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            2
        );

    for (const auto& set : sets)
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                set->getMembershipFunction());

        ASSERT_NE(
            trap,
            nullptr
        );

        EXPECT_TRUE(std::isfinite(trap->getA()));
        EXPECT_TRUE(std::isfinite(trap->getB()));
        EXPECT_TRUE(std::isfinite(trap->getC()));
        EXPECT_TRUE(std::isfinite(trap->getD()));
    }
}
TEST(FuzzySetGeneratorTest, SingleTrapezoidSpansEntireDataset)
{
    Dataset dataset =
        createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            1
        );

    ASSERT_EQ(
        sets.size(),
        1u
    );

    auto trap =
        dynamic_cast<const Trapezoidal*>(
            sets.front()->getMembershipFunction());

    ASSERT_NE(
        trap,
        nullptr
    );

    EXPECT_DOUBLE_EQ(
        trap->getA(),
        10.0
    );

    EXPECT_DOUBLE_EQ(
        trap->getD(),
        92.0
    );
}

TEST(FuzzySetGeneratorTest, GenerateTrapezoidalVariableHasCorrectName)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTrapezoidalVariable(
            dataset,
            0,
            3
        );

    EXPECT_EQ(
        variable.getName(),
        "Temperature"
    );
}
TEST(FuzzySetGeneratorTest, GenerateTrapezoidalVariableStoresDatasetRange)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTrapezoidalVariable(
            dataset,
            0,
            3
        );

    EXPECT_DOUBLE_EQ(
        variable.getMinValue(),
        10.0
    );

    EXPECT_DOUBLE_EQ(
        variable.getMaxValue(),
        92.0
    );
}
TEST(FuzzySetGeneratorTest, GenerateTrapezoidalVariableContainsCorrectNumberOfSets)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTrapezoidalVariable(
            dataset,
            0,
            3
        );

    EXPECT_EQ(
        variable.getSets().size(),
        3u
    );
}
TEST(FuzzySetGeneratorTest, GenerateTrapezoidalVariablePreservesSetNames)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTrapezoidalVariable(
            dataset,
            0,
            3
        );

    const auto& sets =
        variable.getSets();

    ASSERT_EQ(
        sets.size(),
        3u
    );

    EXPECT_EQ(
        sets[0].getName(),
        "Temperature_1"
    );

    EXPECT_EQ(
        sets[1].getName(),
        "Temperature_2"
    );

    EXPECT_EQ(
        sets[2].getName(),
        "Temperature_3"
    );
}
TEST(FuzzySetGeneratorTest, GenerateTrapezoidalVariableStoresTrapezoidalMembershipFunctions)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTrapezoidalVariable(
            dataset,
            0,
            3
        );

    for (const auto& set : variable.getSets())
    {
        EXPECT_NE(
            dynamic_cast<const Trapezoidal*>(
                set.getMembershipFunction()
                ),
            nullptr
        );
    }
}
TEST(FuzzySetGeneratorTest, GenerateTrapezoidalVariablePreservesParameters)
{
    Dataset dataset = createGaussianDataset();

    auto generated =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    FuzzyVariable variable =
        FuzzySetGenerator::generateTrapezoidalVariable(
            dataset,
            0,
            3
        );

    ASSERT_EQ(
        generated.size(),
        variable.getSets().size()
    );

    for (size_t i = 0; i < generated.size(); i++)
    {
        auto t1 =
            dynamic_cast<const Trapezoidal*>(
                generated[i]->getMembershipFunction());

        auto t2 =
            dynamic_cast<const Trapezoidal*>(
                variable.getSets()[i].getMembershipFunction());

        ASSERT_NE(t1, nullptr);
        ASSERT_NE(t2, nullptr);

        constexpr double tolerance = 1e-3;
        EXPECT_NEAR(t1->getA(), t2->getA(), tolerance);
        EXPECT_NEAR(t1->getB(), t2->getB(), tolerance);
        EXPECT_NEAR(t1->getC(), t2->getC(), tolerance);
        EXPECT_NEAR(t1->getD(), t2->getD(), tolerance);
    }
}
TEST(FuzzySetGeneratorTest, GenerateTrapezoidalVariableMembershipFunctionsRemainValid)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTrapezoidalVariable(
            dataset,
            0,
            3
        );

    for (const auto& set : variable.getSets())
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                set.getMembershipFunction());

        ASSERT_NE(
            trap,
            nullptr
        );

        EXPECT_DOUBLE_EQ(
            set.membership(
                (trap->getB() + trap->getC()) / 2.0
            ),
            1.0
        );
    }
}
TEST(FuzzySetGeneratorTest, TrapezoidalVariableMatchesGeneratedSets)
{
    Dataset dataset = createGaussianDataset();

    auto generated =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        );

    FuzzyVariable variable =
        FuzzySetGenerator::generateTrapezoidalVariable(
            dataset,
            0,
            3
        );

    ASSERT_EQ(
        generated.size(),
        variable.getSets().size()
    );

    for (size_t i = 0; i < generated.size(); i++)
    {
        EXPECT_EQ(
            generated[i]->getName(),
            variable.getSets()[i].getName()
        );
    }
}
TEST(FuzzySetGeneratorTest, GenerateTrapezoidalVariableGetSetByName)
{
    Dataset dataset = createGaussianDataset();

    FuzzyVariable variable =
        FuzzySetGenerator::generateTrapezoidalVariable(
            dataset,
            0,
            3
        );

    EXPECT_NE(
        variable.getSet("Temperature_1"),
        nullptr
    );

    EXPECT_NE(
        variable.getSet("Temperature_2"),
        nullptr
    );

    EXPECT_NE(
        variable.getSet("Temperature_3"),
        nullptr
    );

    EXPECT_EQ(
        variable.getSet("Unknown"),
        nullptr
    );
}
TEST(FuzzySetGeneratorTest, GenerateTrapezoidalVariableInvalidFeatureThrows)
{
    Dataset dataset = createGaussianDataset();

    EXPECT_THROW(
        FuzzySetGenerator::generateTrapezoidalVariable(
            dataset,
            5,
            3
        ),
        std::out_of_range
    );
}
TEST(FuzzySetGeneratorTest, EmptyDatasetThrows)
{
    Dataset dataset;

    EXPECT_THROW(
        FuzzySetGenerator::generateTriangleSets(dataset, 0, 3),
        std::invalid_argument);

    EXPECT_THROW(
        FuzzySetGenerator::generateGaussianSets(dataset, 0, 3),
        std::invalid_argument);

    EXPECT_THROW(
        FuzzySetGenerator::generateTrapezoidalSets(dataset, 0, 3),
        std::invalid_argument);
}

TEST(FuzzySetGeneratorTest, ZeroClustersThrows)
{
    Dataset dataset = createGaussianDataset();

    EXPECT_THROW(
        FuzzySetGenerator::generateTriangleSets(dataset, 0, 0),
        std::invalid_argument);

    EXPECT_THROW(
        FuzzySetGenerator::generateGaussianSets(dataset, 0, 0),
        std::invalid_argument);

    EXPECT_THROW(
        FuzzySetGenerator::generateTrapezoidalSets(dataset, 0, 0),
        std::invalid_argument);
}
TEST(FuzzySetGeneratorTest, NegativeClustersThrows)
{
    Dataset dataset = createGaussianDataset();

    EXPECT_THROW(
        FuzzySetGenerator::generateTriangleSets(dataset, 0, -1),
        std::invalid_argument);

    EXPECT_THROW(
        FuzzySetGenerator::generateGaussianSets(dataset, 0, -1),
        std::invalid_argument);

    EXPECT_THROW(
        FuzzySetGenerator::generateTrapezoidalSets(dataset, 0, -1),
        std::invalid_argument);
}
TEST(FuzzySetGeneratorTest, MoreClustersThanSamplesThrows)
{
    Dataset dataset(
        {
            {10},
            {20}
        },
        { "Temperature" }
    );

    EXPECT_THROW(
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3
        ),
        std::invalid_argument
    );

    EXPECT_THROW(
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3
        ),
        std::invalid_argument
    );

    EXPECT_THROW(
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3
        ),
        std::invalid_argument
    );
}
TEST(FuzzySetGeneratorTest, TriangleParametersRemainOrdered)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTriangleSets(
            dataset,
            0,
            3);

    for (const auto& set : sets)
    {
        auto triangle =
            dynamic_cast<const Triangle*>(
                set->getMembershipFunction());

        ASSERT_NE(triangle, nullptr);

        EXPECT_LT(
            triangle->getLeft(),
            triangle->getPeak());

        EXPECT_LT(
            triangle->getPeak(),
            triangle->getRight());
    }
}
TEST(FuzzySetGeneratorTest, TrapezoidParametersRemainOrdered)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateTrapezoidalSets(
            dataset,
            0,
            3);

    for (const auto& set : sets)
    {
        auto trap =
            dynamic_cast<const Trapezoidal*>(
                set->getMembershipFunction());

        ASSERT_NE(trap, nullptr);

        EXPECT_LE(trap->getA(), trap->getB());
        EXPECT_LE(trap->getB(), trap->getC());
        EXPECT_LE(trap->getC(), trap->getD());
    }
}
TEST(FuzzySetGeneratorTest, GeneratedParametersAreFinite)
{
    Dataset dataset = createGaussianDataset();

    auto sets =
        FuzzySetGenerator::generateGaussianSets(
            dataset,
            0,
            3);

    for (const auto& set : sets)
    {
        auto gaussian =
            dynamic_cast<const Gaussian*>(
                set->getMembershipFunction());

        ASSERT_NE(gaussian, nullptr);

        EXPECT_TRUE(std::isfinite(gaussian->getMean()));
        EXPECT_TRUE(std::isfinite(gaussian->getSigma()));
    }
}