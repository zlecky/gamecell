add_executable(lc_example leetcode_example.cpp)
target_link_libraries(lc_example leetcode)

add_executable(spdlog_example spdlog_example.cpp)
target_link_libraries(spdlog_example spdlog::spdlog)

configure_file(
    ../data/helloworld.yaml
    ./data/helloworld.yaml
    )
add_executable(yaml_example yaml_example.cpp)
target_link_libraries(yaml_example yaml-cpp)
