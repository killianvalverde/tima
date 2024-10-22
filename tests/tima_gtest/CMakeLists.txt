project(tima_test)

include_directories(${PROJECT_SOURCE_DIR}/../../src)

set(GTEST_LIBRARIES gtest gtest_main)

set(TIMA_TEST_SOURCE_FILES
        program_test.cpp
)

add_executable(tima_test
        main.cpp
        ${TIMA_TEST_SOURCE_FILES}
)

target_link_libraries(tima_test tima ${GTEST_LIBRARIES})
