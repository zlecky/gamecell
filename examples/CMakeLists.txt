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
add_executable(curl_example curl_example.cpp)
find_package(CURL)
if(CURL_FOUND)
    target_include_directories(curl_example PRIVATE ${CURL_INCLUDE_DIR})
    target_link_libraries(curl_example ${CURL_LIBRARY})
else(CURL_FOUND)
    message(FATAL_ERROR ”CURL library not found”)
endif(CURL_FOUND)

# rdkafka
#add_executable(rdkafka_example rdkafka_example.cpp)
#target_include_directories(rdkafka_example PUBLIC /usr/local/include)
#target_link_libraries(rdkafka_example PUBLIC /usr/local/lib/librdkafka++.a)

add_executable(rdkafka_example rdkafka_example.cpp)
find_package(RDKAFKA)
if(RDKAFKA_FOUND)
    add_library(rdkafka ${RDKAFKA_LIBRARY_TYPE} IMPORTED GLOBAL)
    add_library(rdkafka++ ${RDKAFKA_LIBRARY_TYPE} IMPORTED GLOBAL)

    target_include_directories(rdkafka_example PUBLIC ${RDKAFKA_INCLUDE_DIR})
    message(${RDKAFKA_INCLUDE_DIR})

    target_link_libraries(rdkafka_example ${RDKAFKAPP_LIBRARY})
    message(${RDKAFKA_LIBRARY_DIR})
    message(${RDKAFKAPP_LIBRARY})
else(RDKAFKA_FOUND)
    message(FATAL_ERROR ”RDKAFKA library not found”)
endif(RDKAFKA_FOUND)
