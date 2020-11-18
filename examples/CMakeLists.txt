add_executable(lc_example leetcode_example.cpp)
target_link_libraries(lc_example leetcode)

add_executable(spdlog_example spdlog_example.cpp)
target_link_libraries(spdlog_example spdlog::spdlog)

configure_file(
        ../data/yaml-cpp.yaml
        ./data/yaml-cpp.yaml
)
add_executable(yaml_example yaml_example.cpp)
target_link_libraries(yaml_example yaml-cpp)

add_executable(rapidxml_example rapidxml_example.cpp)
configure_file(
        ../data/rapidxml.xml
        ./data/rapidxml.xml
)

add_executable(json_example json_example.cpp)
configure_file(
        ../data/json.json
        ./data/json.json
)
