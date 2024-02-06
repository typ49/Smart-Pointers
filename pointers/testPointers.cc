#include <gtest/gtest.h>

#include "Shared.h"
#include "Weak.h"
#include "Unique.h"

TEST(test, test)
{
  sp::Shared<int> shared;
  sp::Weak<int> weak;
  sp::Unique<int> unique;
}

/******************************************
 * Test the Unique class                  *
 ******************************************/
class UniqueTest
{
};

/******************************************
 * Test the Shared class                  *
 ******************************************/
class SharedTest
{
};

/******************************************
 * Test the Weak class                    *
 ******************************************/
class WeakTest
{
};

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
