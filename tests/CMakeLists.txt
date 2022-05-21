set(SOURCES
        main.cpp
        utils_tests.cpp
        )

add_executable(pianola_tests ${SOURCES})
target_include_directories(pianola_tests PUBLIC
        ${CATCH2_INCLUDES}
        ${PIANOLA_INCLUDES}
        )
target_link_options(pianola_tests INTERFACE --coverage)

if(UNIX)
    if(APPLE)
        target_link_libraries(pianola_tests PRIVATE
                ${NADARE_LIBRARIES}
                )
    else(APPLE)
        target_link_libraries(pianola_tests PRIVATE
                gcov
                ${PIANOLA_LIBRARIES}
                )

        add_custom_target(test_pianola
                COMMAND lcov --directory  . --zerocounters
                COMMAND pianola_tests DEPENDS pianola_tests
                COMMAND lcov --directory . --capture -output-file cov.info
                COMMAND lcov --remove cov.info '/usr/*' --output-file cov.info
                COMMAND lcov --remove cov.info '*/ext/*' --output-file cov.info
                COMMAND lcov --remove cov.info '*/tests/*' --output-file cov.info
                COMMAND genhtml cov.info --output-directory out
                )
    endif(APPLE)
else(UNIX)

    target_link_libraries(pianola_tests
            #            ${COMPACTNSEARCH_LIBRARIES}
            )

endif(UNIX)