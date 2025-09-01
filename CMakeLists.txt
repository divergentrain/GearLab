cmake_minimum_required(VERSION 3.16)  # Qt6 usually needs >=3.16
project(GearLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable CTest
include(CTest)
enable_testing()

# Add include path for headers
include_directories(${CMAKE_SOURCE_DIR}/src)

# ---- Find Qt ----
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ---- Add your main executable (example) ----
add_executable(gearlab ${CMAKE_SOURCE_DIR}/src/main.cpp)

target_link_libraries(gearlab
    PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
)

# ---- Tests ----
# Collect all .cpp files in tests/
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

# Make a meta-target "tests" that depends on all test executables
set(TEST_EXECUTABLES "")

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)

    add_executable(${test_name} ${test_src})
    # target_link_libraries(${test_name}
    #     PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui
    # )

    add_test(NAME ${test_name} COMMAND ${test_name})

    list(APPEND TEST_EXECUTABLES ${test_name})
endforeach()

# Create a custom "tests" target so `make tests` builds them
add_custom_target(tests DEPENDS ${TEST_EXECUTABLES})

# ---- Manual tests for UI ----
add_executable(test_GearParamInput 
    ${CMAKE_SOURCE_DIR}/src/ui/tests/test_GearParamInput.cpp
    ${CMAKE_SOURCE_DIR}/src/ui/OutputDirSelect.cpp
    ${CMAKE_SOURCE_DIR}/src/ui/BevelGearForm.cpp)
target_link_libraries(test_GearParamInput PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)
