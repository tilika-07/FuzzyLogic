#include <gtest/gtest.h>
#include "InputVar.h"
TEST(InputVarTest, ConstructorWorks) {
    InputVar temperature("Temperature",0.0,100.0);
    EXPECT_EQ(temperature.getName(), "Temperature");
}