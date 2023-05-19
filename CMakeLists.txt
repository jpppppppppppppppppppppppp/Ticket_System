cmake_minimum_required(VERSION 3.16)
project(Ticket_System_master)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        src/main.cpp
        src/BPT.hpp
        src/map.hpp
        src/utility.hpp
        src/exceptions.hpp)


set(test_prefix "${PROJECT_SOURCE_DIR}/testcases")
file(GLOB tests RELATIVE "${test_prefix}" testcases/*)

foreach (test-name IN ITEMS ${tests})
    if (NOT IS_DIRECTORY "${test_prefix}/${test-name}")
        continue()
    endif ()
    add_test(NAME "${test-name}"
            COMMAND bash ../run-test "${test_prefix}" "${test-name}" "$<TARGET_FILE:code>"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/run
            )
endforeach ()

enable_testing()
