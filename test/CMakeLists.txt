include_directories(3party/googletest/googlemock/include)
include_directories(3party/googletest/googletest/include)

add_executable(leetcode_test leetcode_test.cpp)
target_link_libraries(leetcode_test leetcode gtest)

add_executable(gmock_test gmock_test.cpp)
target_link_libraries(gmock_test gmock)

add_executable(gtest_test gtest_test.cpp)
target_link_libraries(gtest_test gtest)
