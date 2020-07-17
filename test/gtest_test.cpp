#include <gtest/gtest.h>

// TEST example
bool is_prime(int num) {
    return (num % 2) != 0;
}

// Global event
class GlobalEvent : public testing::Environment {
public:
    virtual void SetUp() override {
        std::cout << "Global event setup" << std::endl;
    }

    virtual void TearDown() override {
        std::cout << "Global event tear down" << std::endl;
    }
};

// TestSuit event
class TestSuit : public testing::Test {
public:
    static void SetUpTestCase() {
        std::cout << "Test suit event set up" << std::endl;
    }

    static void TearDownTestCase() {
        std::cout << "Test suit event tear down" << std::endl;
    }
};

// TestCase event
class TestCase : public testing::Test {
public:
    virtual void SetUp() override {
        std::cout << "Test case event set up" << std::endl;
    }

    virtual void TearDown() override {
        std::cout << "Test case event tear down" << std::endl;
    }
};

TEST(TestPrime, is_true) {
    EXPECT_TRUE(is_prime(1));
    EXPECT_TRUE(is_prime(3));
    ASSERT_TRUE(is_prime(5));
}

TEST(TestPrime, is_false) {
    EXPECT_FALSE(is_prime(0));
    EXPECT_FALSE(is_prime(2));
    ASSERT_FALSE(is_prime(4));
}

TEST_F(TestSuit, 0_equal) {
    EXPECT_EQ(0, 0);
}

TEST_F(TestSuit, 1_equal) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestCase, 0_equal) {
    EXPECT_EQ(0, 0);
}

TEST_F(TestCase, 1_equal) {
    EXPECT_EQ(1, 1);
}

int main(int argc, char *argv[]) {
    testing::AddGlobalTestEnvironment(new GlobalEvent);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}