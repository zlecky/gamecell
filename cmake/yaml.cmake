include(fetchcontent)

fetchcontent_declare(yaml
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG yaml-cpp-0.6.3
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3party/yaml
    )

fetchcontent_makeavailable(yaml)
