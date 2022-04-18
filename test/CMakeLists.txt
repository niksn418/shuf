cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME shuf_test)
project(${PROJECT_NAME})

# Data files
file(GLOB INPUT_FILES ${PROJECT_SOURCE_DIR}/etc/*.txt)
list(JOIN INPUT_FILES " " TEST_DATA)

file(GLOB RANGE_FILES ${PROJECT_SOURCE_DIR}/etc/range-*.txt.eta)
list(JOIN RANGE_FILES " " RANGE_DATA)

file(GLOB NRANGE_FILES ${PROJECT_SOURCE_DIR}/etc/nrange-*.txt.eta)
list(JOIN NRANGE_FILES " " NRANGE_DATA)

# Tests
enable_testing()
add_test(
    NAME shuf
    COMMAND sh -c "${PROJECT_SOURCE_DIR}/test.sh $<TARGET_FILE:shuf> ${TEST_DATA}"
    )
add_test(
    NAME shuf-range
    COMMAND sh -c "${PROJECT_SOURCE_DIR}/test-range.sh $<TARGET_FILE:shuf> ${RANGE_DATA}"
    )
add_test(
    NAME shuf-nrange
    COMMAND sh -c "${PROJECT_SOURCE_DIR}/test-nrange.sh $<TARGET_FILE:shuf> ${NRANGE_DATA}"
    )
add_test(
    NAME shuf-repeated
    COMMAND sh -c "${PROJECT_SOURCE_DIR}/test-repeat.sh $<TARGET_FILE:shuf> ${TEST_DATA}"
    )
