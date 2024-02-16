#ifndef TEST_UNIQUE
#define TEST_UNIQUE 1
#endif // TEST_UNIQUE
#ifndef TEST_SHARED
#define TEST_SHARED 1
#endif // TEST_SHARED
#ifndef TEST_WEAK
#define TEST_WEAK 1
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

TEST(UniqueTest, MoveConstructor) {
  sp::Unique<int> unique1(new int(5));
  sp::Unique<int> unique2(std::move(unique1));
  EXPECT_EQ(*unique2, 5);
  EXPECT_EQ(unique1.get(), nullptr);
}

TEST(UniqueTest, MoveAssignment) {
  sp::Unique<int> unique1(new int(5));
  sp::Unique<int> unique2;
  unique2 = std::move(unique1);
  EXPECT_EQ(*unique2, 5);
  EXPECT_EQ(unique1.get(), nullptr);
}

TEST(UniqueTest, NonCopyable) {
  sp::Unique<int> unique1(new int(5));
  // Uncommenting the following lines should cause a compile error
  // sp::Unique<int> unique2(unique1);
  // sp::Unique<int> unique3 = unique1;
}

TEST(UniqueTest, BasicUsage) {
  sp::Unique<int> ptr(new int(5));
  ASSERT_EQ(*ptr, 5);
  ASSERT_TRUE(ptr.exists());
}

TEST(UniqueTest, MoveSemantics) {
  sp::Unique<int> ptr1(new int(5));
  sp::Unique<int> ptr2 = std::move(ptr1);
  ASSERT_EQ(*ptr2, 5);
  ASSERT_FALSE(ptr1.exists());
}
#endif // TEST_UNIQUE

#if TEST_SHARED
/******************************************
 * Test the Shared class                  *
 ******************************************/
  TEST(SharedTest, Constructor_default)
  {
    sp::Shared<int> shared;
    EXPECT_EQ(shared.get(), nullptr);
    EXPECT_EQ(shared.count(), 0);
  }

  TEST(SharedTest, Constructor_dynamic)
  {
    sp::Shared<int> sharedI(new int(5));
    EXPECT_EQ(*sharedI.get(), 5);
    EXPECT_EQ(sharedI.count(), 1);
  }

  TEST(SharedTest, Constructor_dynamic_string)
  {
    sp::Shared<std::string> sharedS(new std::string("Hello"));
    EXPECT_EQ(*sharedS.get(), "Hello");
    EXPECT_EQ(sharedS.count(), 1);
  }

  TEST(SharedTest, CopyConstructor)
  {
    sp::Shared<int> sharedI(new int(5));
    sp::Shared<int> sharedCopy(sharedI);
    EXPECT_EQ(*sharedCopy.get(), 5);
    EXPECT_EQ(sharedCopy.count(), 2);
  }

  TEST(SharedTest, CopyConstructor_string)
  {
    sp::Shared<std::string> sharedS(new std::string("Hello"));
    sp::Shared<std::string> sharedCopy(sharedS);
    EXPECT_EQ(*sharedCopy.get(), "Hello");
    EXPECT_EQ(sharedCopy.count(), 2);
  }

  TEST(SharedTest, Get)
  {
    sp::Shared<int> sharedI(new int(5));
    EXPECT_EQ(*sharedI.get(), 5);
    EXPECT_FALSE(sharedI.get() == nullptr);
    EXPECT_EQ(sharedI.count(), 1);
  }

  TEST(SharedTest, BasicUsage) {
    sp::Shared<int> ptr(new int(5));
    ASSERT_EQ(*ptr, 5);
    ASSERT_EQ(ptr.count(), 1);
    ASSERT_TRUE(ptr.exists());
  }

  TEST(SharedTest, CopySemantics) {
    sp::Shared<int> ptr1(new int(5));
    sp::Shared<int> ptr2 = ptr1;
    ASSERT_EQ(*ptr2, 5);
    ASSERT_EQ(ptr1.count(), 2);
    ASSERT_EQ(ptr2.count(), 2);
  }

  TEST(SharedTest, MoveSemantics) {
    sp::Shared<int> ptr1(new int(5));
    sp::Shared<int> ptr2 = std::move(ptr1);
    ASSERT_EQ(*ptr2, 5);
    ASSERT_EQ(ptr1.count(), 0);
    ASSERT_EQ(ptr2.count(), 1);
  }

  TEST(SharedTest, ReferenceCounting) {
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

TEST(WeakTest, DefaultConstructor) {
  sp::Weak<int> weak;
  EXPECT_TRUE(weak.expired());
}

TEST(WeakTest, ConstructorFromShared) {
  sp::Shared<int> shared(new int(5));
  sp::Weak<int> weak(shared);
  EXPECT_FALSE(weak.expired());
  EXPECT_EQ(*weak.lock(), 5);
}

TEST(WeakTest, CopyConstructor) {
  sp::Shared<int> shared(new int(5));
  sp::Weak<int> weak1(shared);
  sp::Weak<int> weak2(weak1);
  EXPECT_FALSE(weak2.expired());
  EXPECT_EQ(*weak2.lock(), 5);
}

TEST(WeakTest, MoveConstructor) {
  sp::Shared<int> shared(new int(5));
  sp::Weak<int> weak1(shared);
  sp::Weak<int> weak2(std::move(weak1));
  EXPECT_FALSE(weak2.expired());
  EXPECT_EQ(*weak2.lock(), 5);
  EXPECT_TRUE(weak1.expired());
}

TEST(WeakTest, MoveAssignment) {
  sp::Shared<int> shared(new int(5));
  sp::Weak<int> weak1(shared);
  sp::Weak<int> weak2;
  weak2 = std::move(weak1);
  EXPECT_FALSE(weak2.expired());
  EXPECT_EQ(*weak2.lock(), 5);
  EXPECT_TRUE(weak1.expired());
}

TEST(WeakTest, LockExpired) {
  sp::Weak<int> weak;
  sp::Shared<int> locked = weak.lock();
  EXPECT_EQ(locked.get(), nullptr);
}

TEST(WeakTest, LockNotExpired) {
  sp::Shared<int> shared(new int(5));
  sp::Weak<int> weak(shared);
  sp::Shared<int> locked = weak.lock();
  EXPECT_EQ(*locked, 5);
}

TEST(WeakTest, Expired) {
  sp::Weak<int> weak;
  EXPECT_TRUE(weak.expired());
}

TEST(WeakTest, NotExpired) {
  sp::Shared<int> shared(new int(5));
  sp::Weak<int> weak(shared);
  EXPECT_FALSE(weak.expired());
}

#endif // TEST_WEAK

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}