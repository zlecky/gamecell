//
// Created by zhaoyongle on 2020/5/20.
//

#include <gmock/gmock.h>

// 1.simple example
class FooInterface {
public:
    virtual ~FooInterface() {}

public:
    virtual std::string get_str() = 0;
    
    virtual void set_value(const std::string& val) = 0;
    virtual void set_2value(int x, int y) = 0;
};

class MockFoo : public FooInterface {
public:
    MOCK_METHOD0(get_str, std::string());
    MOCK_METHOD1(set_value, void(const std::string&));
    MOCK_METHOD2(set_2value, void(int, int));
};

TEST(TestFoo, Behavior_1) {
    std::string val("hello, world");

    MockFoo mock_foo;
    EXPECT_CALL(mock_foo, get_str()).Times(1).WillOnce(testing::Return(val));

    std::cout << "Return value " << mock_foo.get_str() << std::endl;
}

TEST(TestFoo, Behavior_2) {
    MockFoo mock_foo;
    EXPECT_CALL(mock_foo, get_str()).Times(testing::AtLeast(5)).
                WillOnce(testing::Return("Yes, 1th")).
                WillOnce(testing::Return("Yes, 2th")).
                WillRepeatedly(testing::Return("Enn, others"));

    for (int i = 0; i < 5; ++i) {
        std::cout << mock_foo.get_str() << std::endl;
    }
}

// match example
TEST(TestFoo, Match_1) {
    MockFoo mock_foo;

    EXPECT_CALL(mock_foo, set_value(testing::_)).Times(2);
    mock_foo.set_value("hello, world");
    mock_foo.set_value("fuck you");

    EXPECT_CALL(mock_foo, set_2value(testing::Eq(1), testing::Ge(2)));
    //mock_foo.set_2value(1, 1);
    mock_foo.set_2value(1, 2);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}