include(fetchcontent)

fetchcontent_declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.8.1
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3party/spdlog
    )

fetchcontent_makeavailable(spdlog)
