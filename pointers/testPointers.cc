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

TEST(UniqueTest, makeUnique)
{
  auto ptr = sp::Unique<int>::makeUnique(5);
  ASSERT_EQ(*ptr, 5);
  ASSERT_TRUE(ptr.exists());
}

TEST(UniqueTest, MoveConstructor)
{
  sp::Unique<int> unique1(new int(5));
  sp::Unique<int> unique2(std::move(unique1));
  EXPECT_EQ(*unique2, 5);
  EXPECT_EQ(unique1.get(), nullptr);
}

TEST(UniqueTest, MoveAssignment)
{
  sp::Unique<int> unique1(new int(5));
  sp::Unique<int> unique2;
  unique2 = std::move(unique1);
  EXPECT_EQ(*unique2, 5);
  EXPECT_EQ(unique1.get(), nullptr);
}

TEST(UniqueTest, NonCopyable)
{
  sp::Unique<int> unique1(new int(5));
  // Uncommenting the following lines should cause a compile error
  // sp::Unique<int> unique2(unique1);
  // sp::Unique<int> unique3 = unique1;
}

TEST(UniqueTest, BasicUsage)
{
  sp::Unique<int> ptr(new int(5));
  ASSERT_EQ(*ptr, 5);
  ASSERT_TRUE(ptr.exists());
}

TEST(UniqueTest, MoveSemantics)
{
  sp::Unique<int> ptr1(new int(5));
  sp::Unique<int> ptr2 = std::move(ptr1);
  ASSERT_EQ(*ptr2, 5);
  ASSERT_FALSE(ptr1.exists());
}

TEST(UniqueTest, ObjectDestruction)
{
  auto ptr = std::make_unique<int>(10);
  ASSERT_EQ(*ptr, 10);
  ptr.reset();
  ASSERT_EQ(ptr.get(), nullptr);
}

#endif // TEST_UNIQUE

#if TEST_SHARED
/******************************************
 * Test the Shared class                  *
 ******************************************/

TEST(SharedTest, makeShared)
{
  auto shared = sp::Shared<int>::makeShared(5);
  EXPECT_EQ(*shared, 5);
  EXPECT_EQ(shared.count(), 1); // Only one Shared instance
}

TEST(SharedTest, ConstructorDefault)
{
  sp::Shared<int> shared;
  EXPECT_EQ(shared.get(), nullptr);
  EXPECT_EQ(shared.count(), 0); // No more objects are pointing to the memory
}

TEST(SharedTest, ConstructorDynamic)
{
  sp::Shared<int> shared(new int(5));
  EXPECT_EQ(*shared, 5);
  EXPECT_EQ(shared.count(), 1); // Only one Shared instance
}

TEST(SharedTest, CopyConstructor)
{
  sp::Shared<int> original(new int(10));
  sp::Shared<int> copy(original);
  EXPECT_EQ(*copy, 10);
  EXPECT_EQ(copy.count(), 2); // Both instances share ownership
  EXPECT_EQ(original.count(), 2);
}

TEST(SharedTest, MoveConstructor)
{
  sp::Shared<int> original(new int(20));
  sp::Shared<int> moved(std::move(original));
  EXPECT_EQ(*moved, 20);
  EXPECT_EQ(original.get(), nullptr); // original has been moved from
  EXPECT_EQ(moved.count(), 1);        // Moved instance has sole ownership
}

TEST(SharedTest, ReferenceCounting)
{
  sp::Shared<int> first(new int(30));
  {
    sp::Shared<int> second = first; // Reference count should increase
    EXPECT_EQ(first.count(), 2);
    EXPECT_EQ(second.count(), 2);
  }                            // second goes out of scope here
  EXPECT_EQ(first.count(), 1); // Reference count should decrease after second is destroyed
}

TEST(SharedTest, MultipleCopies)
{
  sp::Shared<int> original(new int(100));
  {
    sp::Shared<int> copy1 = original;
    sp::Shared<int> copy2 = original;
    ASSERT_EQ(original.count(), 3);
  }
  ASSERT_EQ(original.count(), 1);
}

TEST(SharedTest, Assignment)
{
  sp::Shared<int> ptr1(new int(100));
  sp::Shared<int> ptr2(new int(200));
  ASSERT_EQ(*ptr1, 100);
  ASSERT_EQ(*ptr2, 200);
  ptr1 = ptr2;
  ASSERT_EQ(*ptr1, 200);
  ASSERT_EQ(ptr1.count(), 2);
}

#endif // TEST_SHARED

#if TEST_WEAK
/******************************************
 * Test the Weak class                    *
 ******************************************/
TEST(WeakTest, DefaultConstructor)
{
  sp::Weak<int> weak;
  EXPECT_TRUE(weak.expired()); // Default constructed Weak should be expired
}

TEST(WeakTest, ConstructorFromShared)
{
  sp::Shared<int> shared(new int(5));
  sp::Weak<int> weak(shared);
  EXPECT_FALSE(weak.expired()); // Weak pointer to a valid Shared instance should not be expired
  sp::Shared<int> locked = weak.lock();
  EXPECT_EQ(*locked, 5);
}

TEST(WeakTest, CopyConstructor)
{
  sp::Shared<int> shared(new int(10));
  sp::Weak<int> original(shared);
  sp::Weak<int> copy(original);
  EXPECT_FALSE(copy.expired()); // Copy should not be expired
  sp::Shared<int> locked = copy.lock();
  EXPECT_EQ(*locked, 10); // Locked Shared should have the correct value
}

TEST(WeakTest, MoveConstructor)
{
  sp::Shared<int> shared(new int(20));
  sp::Weak<int> original(shared);
  sp::Weak<int> moved(std::move(original));
  EXPECT_TRUE(original.expired()); // Original should be expired after move
  EXPECT_FALSE(moved.expired());   // Moved should not be expired
  sp::Shared<int> locked = moved.lock();
  EXPECT_EQ(*locked, 20); // Locked Shared should have the correct value
}

TEST(WeakTest, LockExpired)
{
  sp::Weak<int> weak;
  sp::Shared<int> locked = weak.lock();
  EXPECT_EQ(locked.get(), nullptr); // Locked Shared should be nullptr for an expired Weak
}

TEST(WeakTest, LockNotExpired)
{
  sp::Shared<int> shared(new int(30));
  sp::Weak<int> weak(shared);
  sp::Shared<int> locked = weak.lock();
  EXPECT_EQ(*locked, 30); // Locked Shared should have the correct value if Weak is not expired
}

TEST(WeakTest, ExpiredAfterSharedDestruction)
{
  sp::Weak<int> weak;
  {
    sp::Shared<int> shared(new int(5));
    weak = shared;
    ASSERT_FALSE(weak.expired());
  }
  ASSERT_TRUE(weak.expired());
}

TEST(WeakTest, ConvertToSharedAfterExpiry)
{
  sp::Weak<int> weak;
  {
    sp::Shared<int> shared(new int(10));
    weak = shared;
  }
  sp::Shared<int> locked = weak.lock();
  ASSERT_EQ(locked.get(), nullptr);
}

#endif // TEST_WEAK

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}