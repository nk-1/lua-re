cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(re C)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")

# Find Tarantool and Lua dependecies
set(TARANTOOL_FIND_REQUIRED ON)
find_package(Tarantool)
include_directories(${TARANTOOL_INCLUDE_DIRS})

# Find other dependecies
set(PCRE2_FIND_REQUIRED ON)
find_package(PCRE2)
include_directories(${PCRE2_INCLUDE_DIRS})

# Set CFLAGS
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra")

# Build module
add_subdirectory(re)

# Add testing
enable_testing()
set(LUA_CPATH "LUA_CPATH=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/?.so\\;")
add_test(re ${CMAKE_SOURCE_DIR}/test/re.test.lua)
set_tests_properties(re PROPERTIES ENVIRONMENT "${LUA_CPATH}")

# Add custom target `make check`
add_custom_target(check
    WORKING_DIRECTORY ${PROJECT_BUILD_DIR}
    COMMAND ctest -V)
