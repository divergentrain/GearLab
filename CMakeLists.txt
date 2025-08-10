cmake_minimum_required(VERSION 3.10)
project(GearLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable CTest
include(CTest)
enable_testing()

# Add include path for headers
include_directories(${CMAKE_SOURCE_DIR}/src)

# Find all test .cpp files
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

foreach(test_src ${TEST_SOURCES})
    # Get the filename without extension
    get_filename_component(test_name ${test_src} NAME_WE)

    # Create an executable for this test
    add_executable(${test_name} ${test_src})

    # Link to any project libraries here if needed
    # target_link_libraries(${test_name} PRIVATE some_library)

    # Register it with CTest
    add_test(NAME ${test_name} COMMAND ${test_name})

    # Pass color through CTest by forcing output to terminal
    set_tests_properties(${test_name} PROPERTIES
        ENVIRONMENT "CTEST_OUTPUT_ON_FAILURE=1"
    )
endforeach()
