#include "gtest/gtest.h"

#include <cstring>

#include "vm/shared_object.h"

using namespace vm;

class SharedObjectTest : public ::testing::Test {};

TEST_F(SharedObjectTest, ShouldNotRelease) {
    strong_ref ref = make_object(10);
    strong_ref ref2;
    ref2.acquire(ref);

    EXPECT_FALSE(ref.release());
    EXPECT_TRUE(ref2.release());
    ref2.finalize();
}

TEST_F(SharedObjectTest, ShouldFinalizeWhenWeakRefPresent) {
    strong_ref ref = make_object(10);
    weak_ref ref2;
    ref2.from(ref);

    EXPECT_TRUE(ref.release());
    ref.finalize();
    ref2.release();
}

TEST_F(SharedObjectTest, WeakPtrAquare) {
    strong_ref ref = make_object(10);
    weak_ref ref2;
    weak_ref ref3;
    ref2.from(ref);
    ref3.acquire(ref2);

    EXPECT_TRUE(ref.release());
    EXPECT_TRUE(ref2.expired());
    EXPECT_TRUE(ref3.expired());

    ref.finalize();
    ref2.release();
    ref3.release();
}

TEST_F(SharedObjectTest, WeakRefPromoteToStrong) {
    strong_ref ref = make_object(10);
    weak_ref ref2;
    ref2.from(ref);

    strong_ref ref3 = ref2.lock();
    EXPECT_TRUE(ref3.get_buf());
    EXPECT_EQ(ref3.use_count(), 2);

    ref.release();
    ref.finalize();
    ref2.release();
}

TEST_F(SharedObjectTest, WeakRefPromoteToStrongFailure) {
    strong_ref ref = make_object(10);
    weak_ref ref2;
    ref2.from(ref);

    if (ref.release()) {
        ref.finalize();
    }

    strong_ref ref3 = ref2.lock();
    EXPECT_FALSE(ref3);
    EXPECT_EQ(ref2.use_count(), 0);
    EXPECT_TRUE(ref2.expired());
    ref2.release();
}

TEST_F(SharedObjectTest, MemoryAllocation) {
    char buf[] = "Hello World!";

    strong_ref ref = make_object(sizeof(buf));
    std::memcpy(ref.get_buf(), buf, sizeof(buf));

    EXPECT_EQ(sizeof(buf), 13);
    EXPECT_EQ(std::strcmp(buf, static_cast<const char *>(ref.get_buf())), 0);

    ref.release();
    ref.finalize();
}
