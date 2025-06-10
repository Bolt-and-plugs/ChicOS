cmake_minimum_required(VERSION 3.10)
set(project_name ChicOS)
project(${project_name} VERSION 0.0.1)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif()

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -Wall -Wextra -Wpedantic -fsanitize=address")
endif()

add_definitions(-DBUILD_TYPE="${CMAKE_BUILD_TYPE}")

file(GLOB OS_MODULES src/modules/*/*.c)
file(GLOB SRC_FILES src/*.c)
file(GLOB TEST_FILES test/*.c)

include_directories(src)

find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIRS})

add_executable(${project_name} ${SRC_FILES} ${TEST_FILES} ${OS_MODULES})

target_link_libraries(${project_name} ${CURSES_LIBRARIES})

#enable_testing()
#add_test(NAME ${project_name}_test COMMAND ${project_name}_test)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C flags debug: ${CMAKE_C_FLAGS_DEBUG}")
#message(STATUS "Test files: ${TEST_FILES}")
