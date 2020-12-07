find_path(RDKAFKA_INCLUDE_DIR
    NAMES librdkafka/rdkafka.h librdkafka/rdkafkacpp.h
    PATHS /usr/local/include
    )

# default shared lib
find_library(RDKAFKA_LIBRARY
    NAMES rdkafka
    PATHS /usr/local/lib
    NO_DEFAULT_PATH
    )
#find_library(RDKAFKA_LIBRARY
#    NAMES librdkafka.a
#    PATHS /usr/local/lib
#    NO_DEFAULT_PATH
#    )

# default shared lib
find_library(RDKAFKA++_LIBRARY
    NAMES rdkafka++
    PATHS /usr/local/lib
    NO_DEFAULT_PATH
    )
#find_library(RDKAFKA++_LIBRARY
#    NAMES librdkafka++.a
#    PATHS /usr/local/lib
#    NO_DEFAULT_PATH
#    )


if (RDKAFKA_INCLUDE_DIR AND RDKAFKA_LIBRARY AND RDKAFKA++_LIBRARY)
    set(RDKAFKA_FOUND TRUE)
endif (RDKAFKA_INCLUDE_DIR AND RDKAFKA_LIBRARY AND RDKAFKA++_LIBRARY)

