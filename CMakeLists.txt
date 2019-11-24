cmake_minimum_required(VERSION 3.15)

project(calculator_laguage)

find_package(BISON)
find_package(FLEX)

add_definitions(-D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)

BISON_TARGET(MyParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
FLEX_TARGET(MyScanner tokens.l ${CMAKE_CURRENT_BINARY_DIR}/tokens.c)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(calclang ${BISON_MyParser_OUTPUTS} ${FLEX_MyScanner_OUTPUTS} symbols.c list.c)
