#ifndef TEST_UNIQUE
#define TEST_UNIQUE 1
#endif // TEST_UNIQUE
#ifndef TEST_SHARED
#define TEST_SHARED 1
#endif // TEST_SHARED
#ifndef TEST_WEAK
#define TEST_WEAK 0
#endif // TEST_WEAK

#include <gtest/gtest.h>

#include <iostream>

#include "Shared.h"
#include "Weak.h"
#include "Unique.h"

#if TEST_UNIQUE
/******************************************
 * Test the Unique class                  *
 ******************************************/

class UniqueTest : public ::testing::Test
{
  protected:

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(UniqueTest, MoveConstructor) {
  sp::Unique<int> unique1(new int(5));
  sp::Unique<int> unique2(std::move(unique1));
  EXPECT_EQ(*unique2, 5);
  EXPECT_EQ(unique1.get(), nullptr);
}

TEST_F(UniqueTest, MoveAssignment) {
  sp::Unique<int> unique1(new int(5));
  sp::Unique<int> unique2;
  unique2 = std::move(unique1);
  EXPECT_EQ(*unique2, 5);
  EXPECT_EQ(unique1.get(), nullptr);
}

TEST_F(UniqueTest, NonCopyable) {
  sp::Unique<int> unique1(new int(5));
  // Uncommenting the following lines should cause a compile error
  // sp::Unique<int> unique2(unique1);
  // sp::Unique<int> unique3 = unique1;
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