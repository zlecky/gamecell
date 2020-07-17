add_executable(leetcode_test leetcode_test.cpp)
target_link_libraries(leetcode_test leetcode gtest)

add_executable(gmock_test gmock_test.cpp)
target_link_libraries(gmock_test gmock)