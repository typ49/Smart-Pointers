#define TEST_UNIQUE 0
#define TEST_SHARED 1
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
class SharedTest : public ::testing::Test
{
  protected:
    sp::Shared<int> shared;
    sp::Shared<int> sharedI;
    sp::Shared<std::string> sharedS;
    void SetUp() override
    {
      // init shared pointer with int
      sharedI = sp::Shared<int>(new int(5));
      // init shared pointer with string
      sharedS = sp::Shared<std::string>(new std::string("Hello"));
    }

    void TearDown() override
    {
    }
};
  TEST_F(SharedTest, Constructor_default)
  {
    EXPECT_EQ(shared.get(), nullptr);
    EXPECT_EQ(shared.count(), 0);
  }

  TEST_F(SharedTest, Constructor_dynamic)
  {
    EXPECT_EQ(*sharedI.get(), 5);
    EXPECT_EQ(sharedI.count(), 1);
  }

  TEST_F(SharedTest, Constructor_dynamic_string)
  {
    EXPECT_EQ(*sharedS.get(), "Hello");
    EXPECT_EQ(sharedS.count(), 1);
  }

  TEST_F(SharedTest, CopyConstructor)
  {
    sp::Shared<int> sharedCopy(sharedI);
    EXPECT_EQ(*sharedCopy.get(), 5);
    EXPECT_EQ(sharedCopy.count(), 2);
  }

  TEST_F(SharedTest, CopyConstructor_string)
  {
    sp::Shared<std::string> sharedCopy(sharedS);
    EXPECT_EQ(*sharedCopy.get(), "Hello");
    EXPECT_EQ(sharedCopy.count(), 2);
  }

  TEST_F(SharedTest, Get)
  {
    EXPECT_EQ(*sharedI.get(), 5);
    EXPECT_FALSE(sharedI.get() == nullptr);
    EXPECT_EQ(sharedI.count(), 1);
  
  }

  TEST_F(SharedTest, BasicUsage) {
    sp::Shared<int> ptr(new int(5));
    ASSERT_EQ(*ptr, 5);
    ASSERT_EQ(ptr.count(), 1);
    ASSERT_TRUE(ptr.exists());
  }

  TEST_F(SharedTest, CopySemantics) {
    sp::Shared<int> ptr1(new int(5));
    sp::Shared<int> ptr2 = ptr1;
    ASSERT_EQ(*ptr2, 5);
    ASSERT_EQ(ptr1.count(), 2);
    ASSERT_EQ(ptr2.count(), 2);
  }

  TEST_F(SharedTest, MoveSemantics) {
    sp::Shared<int> ptr1(new int(5));
    sp::Shared<int> ptr2 = std::move(ptr1);
    ASSERT_EQ(*ptr2, 5);
    ASSERT_EQ(ptr1.count(), 0);
    ASSERT_EQ(ptr2.count(), 1);
  }

  TEST_F(SharedTest, ReferenceCounting) {
    {
      sp::Shared<int> ptr1(new int(5));
      {
        sp::Shared<int> ptr2 = ptr1;
        ASSERT_EQ(ptr1.count(), 2);
        ASSERT_EQ(ptr2.count(), 2);
      }
      ASSERT_EQ(ptr1.count(), 1);
    }
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