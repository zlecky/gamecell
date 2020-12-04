add_executable(lc_example leetcode_example.cpp)
target_link_libraries(lc_example leetcode)

# spdlog
add_executable(spdlog_example spdlog_example.cpp)
target_link_libraries(spdlog_example spdlog::spdlog)

# yaml
configure_file(
        ../data/yaml-cpp.yaml
        ./data/yaml-cpp.yaml
)
add_executable(yaml_example yaml_example.cpp)
target_link_libraries(yaml_example yaml-cpp)

# rapidxml
add_executable(rapidxml_example rapidxml_example.cpp)
configure_file(
        ../data/rapidxml.xml
        ./data/rapidxml.xml
)

# json
add_executable(json_example json_example.cpp)
configure_file(
        ../data/json.json
        ./data/json.json
)

# curl
find_package(CURL)
add_executable(curl_example curl_example.cpp)
if(CURL_FOUND)
    target_include_directories(curl_example PRIVATE ${CURL_INCLUDE_DIR})
    target_link_libraries(curl_example ${CURL_LIBRARY})
else(CURL_FOUND)
    message(FATAL_ERROR ”CURL library not found”)
endif(CURL_FOUND)
