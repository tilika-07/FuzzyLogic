#include <gtest/gtest.h>
#include "Singleton.h"

TEST(SingletonTest, MembershipAtSingletonValue) {
    Singleton s("single", 5.0);

    EXPECT_DOUBLE_EQ(s.compute(5.0), 1.0);
}

TEST(SingletonTest, MembershipElsewhereZero) {
    Singleton s("single", 5.0);

    EXPECT_DOUBLE_EQ(s.compute(4.0), 0.0);
    EXPECT_DOUBLE_EQ(s.compute(6.0), 0.0);
}

TEST(SingletonTest, NameStoredCorrectly) {
    Singleton s("singletonName", 5.0);

    EXPECT_EQ(s.getName(), "singletonName");
}