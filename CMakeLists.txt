cmake_minimum_required(VERSION 3.10)
set(project_name ChicOS)
project(${project_name} VERSION 0.0.1)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif()

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  add_compile_options(-g3 -O0 -Wall -Wextra -Wpedantic -fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

add_definitions(-DBUILD_TYPE="${CMAKE_BUILD_TYPE}")

file(GLOB OS_MODULES src/modules/*/*.c)
file(GLOB SRC_FILES src/*.c)
file(GLOB TEST_FILES test/*.c)

include_directories(src)

find_package(PkgConfig REQUIRED)

# --- ALTERAÇÃO IMPORTANTE AQUI ---
# Usamos IMPORTED_TARGET para criar alvos que contêm todas as informações necessárias.
pkg_check_modules(SODIUM REQUIRED IMPORTED_TARGET libsodium)
pkg_check_modules(NCURSES REQUIRED IMPORTED_TARGET ncurses)

add_executable(${project_name} ${SRC_FILES} ${TEST_FILES} ${OS_MODULES})

# --- ALTERAÇÃO IMPORTANTE AQUI ---
# Agora vinculamos aos alvos importados, que é a maneira mais robusta.
target_link_libraries(${project_name}
    PRIVATE
        PkgConfig::SODIUM
        PkgConfig::NCURSES
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")