cmake_minimum_required(VERSION 3.10)
set(project_name ChicOS)
project(${project_name} VERSION 0.0.1)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif()

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  add_compile_options(-g3 -O0 -Wall -Wextra -Wpedantic)
  if(NOT MSVC)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
  endif()
endif()

add_definitions(-DBUILD_TYPE="${CMAKE_BUILD_TYPE}")

file(GLOB OS_MODULES src/modules/*/*.c)
file(GLOB SRC_FILES src/*.c)
file(GLOB TEST_FILES test/*.c)

include_directories(src)

if (WIN32)

  set(SODIUM_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vendor/libsodium")
  set(NCURSES_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vendor/pdcurses")

  find_path(SODIUM_INCLUDE_DIR NAMES sodium.h PATHS "${SODIUM_ROOT}/include")
  find_path(NCURSES_INCLUDE_DIR NAMES curses.h PATHS "${NCURSES_ROOT}/include")

  find_library(SODIUM_LIBRARY NAMES libsodium.lib sodium PATHS "${SODIUM_ROOT}/lib")
  find_library(NCURSES_LIBRARY NAMES pdcurses.lib pdcurses PATHS "${NCURSES_ROOT}/lib")

  if (NOT SODIUM_INCLUDE_DIR OR NOT SODIUM_LIBRARY)
    message(FATAL_ERROR "Libsodium not found in ${SODIUM_ROOT}")
  endif()
  if (NOT NCURSES_INCLUDE_DIR OR NOT NCURSES_LIBRARY)
    message(FATAL_ERROR "PDCurses not found in ${NCURSES_ROOT}")
  endif()

  add_library(PkgConfig::SODIUM UNKNOWN IMPORTED)
  set_target_properties(PkgConfig::SODIUM PROPERTIES
          IMPORTED_LOCATION "${SODIUM_LIBRARY}"
          INTERFACE_INCLUDE_DIRECTORIES "${SODIUM_INCLUDE_DIR}"
  )

  add_library(PkgConfig::NCURSES UNKNOWN IMPORTED)
  set_target_properties(PkgConfig::NCURSES PROPERTIES
          IMPORTED_LOCATION "${NCURSES_LIBRARY}"
          INTERFACE_INCLUDE_DIRECTORIES "${NCURSES_INCLUDE_DIR}"
  )

  add_definitions(-DPDC_DLL_BUILD)

else ()
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SODIUM REQUIRED IMPORTED_TARGET libsodium)
  pkg_check_modules(NCURSES REQUIRED IMPORTED_TARGET ncurses)
endif ()

add_executable(${project_name} ${SRC_FILES} ${TEST_FILES} ${OS_MODULES})

target_link_libraries(${project_name}
        PRIVATE
        PkgConfig::SODIUM
        PkgConfig::NCURSES
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")