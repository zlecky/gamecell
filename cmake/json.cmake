include(fetchcontent)

fetchcontent_declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.9.1
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3party/json
    )

fetchcontent_makeavailable(json)
