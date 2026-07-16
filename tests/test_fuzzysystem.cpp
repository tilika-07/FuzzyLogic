#include <gtest/gtest.h>

#include "../include/FuzzySystem.h"
#include "../include/InputVar.h"
#include "../include/OutputVar.h"
#include "../include/FuzzySet.h"
#include "../include/Triangle.h"

#include <memory>
#include <stdexcept>


class TestFuzzySystem : public FuzzySystem
{
public:

    void validate() const override
    {
        validateCommon();
    }
};


class FuzzySystemTest : public ::testing::Test
{
protected:

    TestFuzzySystem system;

    InputVar temperature{
        "Temperature",
        0,
        100
    };

    OutputVar fan{
        "FanSpeed",
        0,
        100
    };

    void SetUp() override
    {
        temperature.addSet(
            FuzzySet(
                "Cold",
                std::make_shared<Triangle>(
                    "Cold",
                    0,
                    0,
                    50
                    )
            )
        );

        fan.addSet(
            FuzzySet(
                "Slow",
                std::make_shared<Triangle>(
                    "Slow",
                    0,
                    0,
                    50
                    )
            )
        );
    }
};

TEST_F(
    FuzzySystemTest,
    InitiallyEmpty
)
{
    EXPECT_TRUE(
        system.empty()
    );

    EXPECT_EQ(
        system.inputCount(),
        0
    );

    EXPECT_EQ(
        system.outputCount(),
        0
    );
}

TEST_F(
    FuzzySystemTest,
    AddInputVariable
)
{
    system.addInputVar(
        temperature
    );

    EXPECT_EQ(
        system.inputCount(),
        1
    );

    EXPECT_TRUE(
        system.hasInputVar(
            "Temperature"
        )
    );
}


TEST_F(
    FuzzySystemTest,
    AddMultipleInputVariables
)
{
    InputVar humidity(
        "Humidity",
        0,
        100
    );

    system.addInputVar(
        temperature
    );

    system.addInputVar(
        humidity
    );

    EXPECT_EQ(
        system.inputCount(),
        2
    );
}


TEST_F(
    FuzzySystemTest,
    GetExistingInputVariable
)
{
    system.addInputVar(
        temperature
    );

    auto result =
        system.getInputVar(
            "Temperature"
        );

    ASSERT_NE(
        result,
        nullptr
    );

    EXPECT_EQ(
        result->getName(),
        "Temperature"
    );
}


TEST_F(
    FuzzySystemTest,
    UnknownInputReturnsNullptr
)
{
    EXPECT_EQ(
        system.getInputVar(
            "Missing"
        ),
        nullptr
    );
}


TEST_F(
    FuzzySystemTest,
    DuplicateInputThrows
)
{
    system.addInputVar(
        temperature
    );

    EXPECT_THROW(
        system.addInputVar(
            temperature
        ),
        std::runtime_error
    );
}


TEST_F(
    FuzzySystemTest,
    RemoveInputVariable
)
{
    system.addInputVar(
        temperature
    );

    system.removeInputVar(
        "Temperature"
    );

    EXPECT_FALSE(
        system.hasInputVar(
            "Temperature"
        )
    );

    EXPECT_EQ(
        system.inputCount(),
        0
    );
}


TEST_F(
    FuzzySystemTest,
    RemoveUnknownInputThrows
)
{
    EXPECT_THROW(
        system.removeInputVar(
            "Unknown"
        ),
        std::runtime_error
    );
}



TEST_F(
    FuzzySystemTest,
    AddOutputVariable
)
{
    system.addOutputVar(
        fan
    );

    EXPECT_EQ(
        system.outputCount(),
        1
    );

    EXPECT_TRUE(
        system.hasOutputVar(
            "FanSpeed"
        )
    );
}


TEST_F(
    FuzzySystemTest,
    GetExistingOutputVariable
)
{
    system.addOutputVar(
        fan
    );

    auto result =
        system.getOutputVar(
            "FanSpeed"
        );

    ASSERT_NE(
        result,
        nullptr
    );

    EXPECT_EQ(
        result->getName(),
        "FanSpeed"
    );
}


TEST_F(
    FuzzySystemTest,
    UnknownOutputReturnsNullptr
)
{
    EXPECT_EQ(
        system.getOutputVar(
            "Unknown"
        ),
        nullptr
    );
}


TEST_F(
    FuzzySystemTest,
    DuplicateOutputThrows
)
{
    system.addOutputVar(
        fan
    );

    EXPECT_THROW(
        system.addOutputVar(
            fan
        ),
        std::runtime_error
    );
}


TEST_F(
    FuzzySystemTest,
    RemoveOutputVariable
)
{
    system.addOutputVar(
        fan
    );

    system.removeOutputVar(
        "FanSpeed"
    );

    EXPECT_FALSE(
        system.hasOutputVar(
            "FanSpeed"
        )
    );

    EXPECT_EQ(
        system.outputCount(),
        0
    );
}


TEST_F(
    FuzzySystemTest,
    RemoveUnknownOutputThrows
)
{
    EXPECT_THROW(
        system.removeOutputVar(
            "Unknown"
        ),
        std::runtime_error
    );
}


TEST_F(
    FuzzySystemTest,
    ClearRemovesEverything
)
{
    system.addInputVar(
        temperature
    );

    system.addOutputVar(
        fan
    );

    system.clear();

    EXPECT_TRUE(
        system.empty()
    );

    EXPECT_EQ(
        system.inputCount(),
        0
    );

    EXPECT_EQ(
        system.outputCount(),
        0
    );
}


TEST_F(
    FuzzySystemTest,
    NotEmptyAfterAddingVariable
)
{
    system.addInputVar(
        temperature
    );

    EXPECT_FALSE(
        system.empty()
    );
}



TEST_F(
    FuzzySystemTest,
    ValidateThrowsWithoutVariables
)
{
    EXPECT_THROW(
        system.validate(),
        std::runtime_error
    );
}


TEST_F(
    FuzzySystemTest,
    ValidateThrowsWithoutOutput
)
{
    system.addInputVar(
        temperature
    );

    EXPECT_THROW(
        system.validate(),
        std::runtime_error
    );
}


TEST_F(
    FuzzySystemTest,
    ValidateThrowsWithoutInput
)
{
    system.addOutputVar(
        fan
    );

    EXPECT_THROW(
        system.validate(),
        std::runtime_error
    );
}


TEST_F(
    FuzzySystemTest,
    ValidateSucceedsWithInputAndOutput
)
{
    system.addInputVar(
        temperature
    );

    system.addOutputVar(
        fan
    );

    EXPECT_NO_THROW(
        system.validate()
    );
}