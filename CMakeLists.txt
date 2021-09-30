cmake_minimum_required(VERSION 3.21.3)
project(testd2xx VERSION 1.0)

SET(ftd2xx "/usr/lib/libftd2xx.so.1.4.24")
SET(ftd2xx_headers "/usr/include/ftd2xx.h")

find_package (Boost COMPONENTS system filesystem thread program_options unit_test_framework REQUIRED)

if (Boost_FOUND)
    message(STATUS "Boost is found!")
    include_directories(${Boost_INCLUDE_DIR})
    include_directories (${TEST_SOURCE_DIR}/src)
    add_definitions("-DHAS_BOOST")
    add_definitions ("-DBOOST_TEST_DYN_LINK")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(testd2xx main.cpp)

target_include_directories(testd2xx PRIVATE ftd2xx_headers)

target_link_libraries(testd2xx PRIVATE Threads::Threads ${Boost_LIBRARIES} ftd2xx)
