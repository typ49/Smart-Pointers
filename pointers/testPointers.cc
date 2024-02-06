#ifndef TEST_UNIQUE
#define TEST_UNIQUE 0
#ifndef TEST_SHARED
#define TEST_SHARED 1
#ifndef TEST_WEAK
#define TEST_WEAK 0

#include <gtest/gtest.h>

#include "Shared.h"
#include "Weak.h"
#include "Unique.h"

#if TEST_UNIQUE
/******************************************
 * Test the Unique class                  *
 ******************************************/
TEST_F(UniqueTest, testConstructor)
{
  sp::Unique<int> unique;
  EXPECT_EQ(unique.get(), nullptr);
}
#endif // TEST_UNIQUE

#if TEST_SHARED
/******************************************
 * Test the Shared class                  *
 ******************************************/
TEST(SharedTest, testConstructor)
{
  sp::Shared<int> shared(new int(5));
  EXPECT_EQ(shared.get(), 5);
  EXPECT_EQ(shared.count(), 1);
}
#endif // TEST_SHARED

#if TEST_WEAK
/******************************************
 * Test the Weak class                    *
 ******************************************/
TEST_F(WeakTest, testConstructor)
{
  sp::Weak<int> weak;
  EXPECT_EQ(weak.use_count(), 0);
}
#endif // TEST_WEAK

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif // TEST_UNIQUE
#endif // TEST_SHARED
#endif // TEST_WEAK