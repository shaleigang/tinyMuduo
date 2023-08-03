aux_source_directory(. BASE_SRCS)
add_library(tmuduo_base SHARED ${BASE_SRCS})

install(TARGETS tmuduo_base
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)