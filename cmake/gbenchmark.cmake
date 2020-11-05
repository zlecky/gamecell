include(fetchcontent)

fetchcontent_declare(gbenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v${GBENCHMARK_VERSION}
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3party/gbenchmark
    )

fetchcontent_makeavailable(gbenchmark)
