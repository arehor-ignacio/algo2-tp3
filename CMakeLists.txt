cmake_minimum_required(VERSION 2.8)
project(tp3)

set(CMAKE_CXX_STANDARD 11)

# Algunos flags para pasar al compilador (gnu++11 en vez de c++11 para que sea cross-plat)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=gnu++11 -ggdb3 -g -Werror")

# Google Test
add_library(gtest STATIC tests/gtest-1.8.1/gtest-all.cc)
add_library(gtest_main STATIC tests/gtest-1.8.1/gtest_main.cc)

# Leemos todos los archivos fuente en ./src
file(GLOB SOURCE_FILES src/*.cpp src/*.h src/*.hpp src/modulos_basicos/*.h
     src/modulos_basicos/*.cpp src/modulos_basicos/*.hpp)

# Leemos todos los archivos de test en ./tests
file(GLOB TEST_SOURCES tests/*.cpp)

# Copiamos archivos de datos
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
else()
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/datos DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

#add_executable(main ${SOURCE_FILES})
add_executable(correrTests ${SOURCE_FILES} ${TEST_SOURCES})
add_executable(correrTests-Wall ${SOURCE_FILES} ${TEST_SOURCES})
add_executable(correrTestsStringMap ${SOURCE_FILES} tests/string_map_tests.cpp)
add_executable(correrTestsTabla ${SOURCE_FILES} tests/tabla_tests.cpp)
add_executable(correrTestsRegistro ${SOURCE_FILES} tests/registro_tests.cpp)
add_executable(correrTestsBDD ${SOURCE_FILES} tests/base_de_datos_tests.cpp)

# Necesitamos asociar los archivos del framework de testing
target_link_libraries(correrTests gtest gtest_main)
target_link_libraries(correrTests-Wall gtest gtest_main)
target_link_libraries(correrTestsStringMap gtest gtest_main)
target_link_libraries(correrTestsTabla gtest gtest_main)
target_link_libraries(correrTestsRegistro gtest gtest_main)
target_link_libraries(correrTestsBDD gtest gtest_main)

## Test suites a ejecutar
add_test(correrTests driver_test)
add_test(correrTests-Wall driver_test)
add_test(correrTestsStringMap string_map_tests)
add_test(correrTestsTabla tabla_test)
add_test(correrTestsRegistro registro_test)
add_test(correrTestsBDD BDD_test)

# Target para correr Valgrind
add_custom_target(correrValgrind
        valgrind --leak-check=full -v ./correrTests 2>&1
        DEPENDS correrTests)

target_compile_options(correrTests-Wall PUBLIC -Wall)

target_compile_definitions(correrTestsStringMap PUBLIC STRING_MAP_TEST)
