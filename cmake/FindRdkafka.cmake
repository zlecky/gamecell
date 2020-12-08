if (RDKAFKA_STATIC_LIB)
    set(RDKAFKA_PREFIX ${CMAKE_STATIC_LIBRARY_PREFIX})
    set(RDKAFKA_SUFFIX ${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(RDKAFKA_LIBRARY_TYPE STATIC)
else()
    set(RDKAFKA_PREFIX ${CMAKE_SHARED_LIBRARY_PREFIX})
    set(RDKAFKA_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(RDKAFKA_LIBRARY_TYPE SHARED)
endif()

set(RDKAFKA_LIB_NAME ${RDKAFKA_PREFIX}rdkafka${RDKAFKA_SUFFIX})
set(RDKAFKAPP_LIB_NAME ${RDKAFKA_PREFIX}rdkafka++${RDKAFKA_SUFFIX})

find_path(RDKAFKA_INCLUDE_DIR
    NAMES librdkafka/rdkafka.h librdkafka/rdkafkacpp.h
    PATHS /usr/local/include
    )

find_library(RDKAFKA_LIBRARY_DIR
    NAMES ${RDKAFKA_LIB_NAME}
    PATHS /usr/local/lib
    NO_DEFAULT_PATH
    )

find_library(RDKAFKAPP_LIBRARY
    NAMES ${RDKAFKAPP_LIB_NAME}
    PATHS /usr/local/lib
    NO_DEFAULT_PATH
    )


if (RDKAFKA_INCLUDE_DIR AND RDKAFKA_LIBRARY_DIR AND RDKAFKAPP_LIBRARY)
    set(RDKAFKA_FOUND TRUE)
endif (RDKAFKA_INCLUDE_DIR AND RDKAFKA_LIBRARY_DIR AND RDKAFKAPP_LIBRARY)

