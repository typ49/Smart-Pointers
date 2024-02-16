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

//--- Test on unique pointers ---

TEST(UniqueTest, EmptyConstructionTest)
{
    sp::Unique<int> uniquePtr;
    EXPECT_FALSE(uniquePtr.exists());
    EXPECT_FALSE(uniquePtr);
}

TEST(UniqueTest, ConstructionTest)
{
    sp::Unique<int> uniquePtr = sp::Unique<int>::makeUnique<int>(42);
    EXPECT_TRUE(uniquePtr);
}

TEST(UniqueTest, DereferenceTest)
{
    sp::Unique<int> uniquePtr = sp::Unique<int>::makeUnique<int>(42);
    EXPECT_EQ(*uniquePtr, 42);
    EXPECT_TRUE(uniquePtr);
}

TEST(UniqueTest, ModificationTest)
{
    sp::Unique<int> uniquePtr = sp::Unique<int>::makeUnique<int>(42);
    *uniquePtr = 69;
    EXPECT_EQ(*uniquePtr, 69u);
}

TEST(UniqueTest, MoveConstructionTest)
{
    sp::Unique<int> original = sp::Unique<int>::makeUnique<int>(42);
    sp::Unique<int> moved(std::move(original));
    EXPECT_FALSE(original);
    EXPECT_EQ(*moved, 42);
}

TEST(UniqueTest, MoveAssignmentTest)
{
    sp::Unique<int> original = sp::Unique<int>::makeUnique<int>(42);
    sp::Unique<int> moved = std::move(original);
    EXPECT_TRUE(original.get() == nullptr);
    EXPECT_EQ(*moved, 42);
}

TEST(UniqueTest, NullptrAssignmentTest)
{
    sp::Unique<int> uniquePtr = sp::Unique<int>::makeUnique<int>(42);
    uniquePtr = nullptr; // this is using the move-assignment method !
    EXPECT_FALSE(uniquePtr);
}

TEST(UniqueTest, GetTest)
{
    int *rawPtr = new int(42);
    sp::Unique<int> uniquePtr(rawPtr);
    EXPECT_EQ(uniquePtr.get(), rawPtr);
}

TEST(UniqueTest, ResetTest)
{
    sp::Unique<int> uniquePtr = sp::Unique<int>::makeUnique<int>(42);
    uniquePtr.reset();
    EXPECT_FALSE(uniquePtr);
}

//--- Test on shared pointers ---

TEST(SharedTest, EmptyConstructionTest)
{
    sp::Shared<int> sharedPtr;
    EXPECT_FALSE(sharedPtr.exists());
    EXPECT_FALSE(sharedPtr);
}

TEST(SharedTest, ConstructionTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    EXPECT_TRUE(sharedPtr);
    EXPECT_EQ(sharedPtr.count(), 1u);
    EXPECT_EQ(*sharedPtr, 42u);
}

TEST(SharedTest, ModificationTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    *sharedPtr = 69;
    EXPECT_EQ(*sharedPtr, 69u);
}

TEST(SharedTest, CopyConstructionTest)
{
    sp::Shared<int> original = sp::Shared<int>::makeShared<int>(42);
    sp::Shared<int> copy(original);
    EXPECT_EQ(original.count(), 2u);
    EXPECT_EQ(copy.count(), 2u);
    EXPECT_EQ(*original, *copy);
    *copy = 69;
    EXPECT_EQ(*original, 69u);
    EXPECT_EQ(*copy, 69u);
}

TEST(SharedTest, CopyAssignmentTest)
{
    sp::Shared<int> original = sp::Shared<int>::makeShared<int>(42);
    sp::Shared<int> copy;
    copy = original;
    EXPECT_TRUE(original);
    EXPECT_TRUE(copy);
    EXPECT_EQ(original.count(), 2u);
    EXPECT_EQ(copy.count(), 2u);
    EXPECT_EQ(*original, 42u);
    EXPECT_EQ(*copy, 42u);
    *copy = 69;
    EXPECT_EQ(*original, 69u);
    EXPECT_EQ(*copy, 69u);
}

TEST(SharedTest, MoveConstructionTest)
{
    sp::Shared<int> original = sp::Shared<int>::makeShared<int>(42);
    sp::Shared<int> moved(std::move(original));
    EXPECT_FALSE(original);
    EXPECT_TRUE(moved);
    EXPECT_EQ(original.count(), 0u);
    EXPECT_EQ(moved.count(), 1u);
}

TEST(SharedTest, MoveAssignmentTest)
{
    sp::Shared<int> sharedPtr1 = sp::Shared<int>::makeShared<int>(42);
    sp::Shared<int> sharedPtr2 = sp::Shared<int>::makeShared<int>(69);
    sharedPtr2 = std::move(sharedPtr1);
    EXPECT_FALSE(sharedPtr1);
    EXPECT_TRUE(sharedPtr2);
    EXPECT_EQ(sharedPtr1.count(), 0u);
    EXPECT_EQ(sharedPtr2.count(), 1u);
    EXPECT_EQ(*sharedPtr2, 42u);
}

TEST(SharedTest, NullptrAssignmentTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    sharedPtr = nullptr;
    EXPECT_FALSE(sharedPtr);
}

TEST(SharedTest, CountTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    {
        sp::Shared<int> copy(sharedPtr);
        EXPECT_EQ(sharedPtr.count(), 2u);
        EXPECT_EQ(copy.count(), 2u);
    }
    EXPECT_EQ(sharedPtr.count(), 1u);
}

TEST(SharedTest, GetTest)
{
    int *rawPtr = new int(42);
    sp::Shared<int> sharedPtr(rawPtr);
    EXPECT_EQ(sharedPtr.get(), rawPtr);
}

TEST(SharedTest, ResetTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    sharedPtr.reset();
    EXPECT_FALSE(sharedPtr);
}

//--- Test on weak pointers ---

TEST(WeakTest, EmptyConstructionTest)
{
    sp::Weak<int> weakPtr;
    sp::Shared<int> lockedPtr = weakPtr.lock();
    EXPECT_FALSE(lockedPtr.exists());
    EXPECT_FALSE(lockedPtr);
}

TEST(WeakTest, ConstructionFromSharedTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    sp::Weak<int> weakPtr(sharedPtr);
    sp::Shared<int> lockedPtr = weakPtr.lock();
    EXPECT_TRUE(lockedPtr);
    EXPECT_EQ(lockedPtr.count(), 2u);
    EXPECT_EQ(*lockedPtr, 42u);
}

TEST(WeakTest, CountTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    sp::Weak<int> weakPtr(sharedPtr);
    {
        sp::Shared<int> sharedPtrLocked = weakPtr.lock();
        EXPECT_EQ(sharedPtrLocked.count(), 2u);
    }
    EXPECT_EQ(sharedPtr.count(), 1u);
}

TEST(WeakTest, CopyConstructionTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    sp::Weak<int> weakPtr(sharedPtr);
    sp::Weak<int> weakPtrCopy(weakPtr);
    sp::Shared<int> lockedPtrCopy = weakPtrCopy.lock();
    EXPECT_TRUE(lockedPtrCopy);
    EXPECT_EQ(lockedPtrCopy.count(), 2u);
    EXPECT_EQ(*lockedPtrCopy, 42u);
}

TEST(WeakTest, CopyAssignmentTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    sp::Weak<int> weakPtr(sharedPtr);
    sp::Weak<int> weakPtrCopy;
    weakPtrCopy = weakPtr;
    sp::Shared<int> lockedPtrCopy = weakPtrCopy.lock();
    EXPECT_TRUE(lockedPtrCopy);
    EXPECT_EQ(lockedPtrCopy.count(), 2u);
    EXPECT_EQ(*lockedPtrCopy, 42u);
}

TEST(WeakTest, MoveConstructionTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    sp::Weak<int> weakPtr(std::move(sharedPtr));
    sp::Shared<int> lockedPtr = weakPtr.lock();
    EXPECT_TRUE(lockedPtr);
    EXPECT_EQ(lockedPtr.count(), 2u);
    EXPECT_EQ(*lockedPtr, 42u);
}

TEST(WeakTest, MoveAssignmentTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    sp::Weak<int> weakPtr;
    weakPtr = std::move(sharedPtr);
    sp::Shared<int> lockedPtr = weakPtr.lock();
    EXPECT_TRUE(lockedPtr);
    EXPECT_EQ(lockedPtr.count(), 2u);
    EXPECT_EQ(*lockedPtr, 42u);
}

TEST(WeakTest, LockTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    sp::Weak<int> weakPtr(sharedPtr);
    sp::Shared<int> lockedPtr = weakPtr.lock();
    EXPECT_TRUE(lockedPtr);
    EXPECT_EQ(lockedPtr.count(), 2u);
    EXPECT_EQ(*lockedPtr, 42u);
}

TEST(WeakTest, LockAfterDestructionTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    sp::Weak<int> weakPtr(sharedPtr);
    sharedPtr = nullptr;
    sp::Shared<int> lockedPtr = weakPtr.lock();
    EXPECT_FALSE(lockedPtr);
}

TEST(WeakTest, ResetTest)
{
    sp::Shared<int> sharedPtr = sp::Shared<int>::makeShared<int>(42);
    sp::Weak<int> weakPtr(sharedPtr);
    weakPtr.reset();
    sp::Shared<int> lockedPtr = weakPtr.lock();
    EXPECT_FALSE(lockedPtr);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
