include(fetchcontent)

fetchcontent_declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3party/googletest
    )

fetchcontent_makeavailable(googletest)
