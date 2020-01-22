add_executable(main
        main.cpp
        parts.h
        parts.cpp
        )
configure_file(${CMAKE_CURRENT_SOURCE_DIR}definitions.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}queries.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)