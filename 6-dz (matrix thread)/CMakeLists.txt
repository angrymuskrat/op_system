cmake_minimum_required ( VERSION 2.6 )
project ( matrix_adder )

set ( APP_NAME  matrix_adder )

set( CMAKE_BUILD_TYPE RELEASE )

add_executable ( adder_mt adder_mt.c )

target_link_libraries(adder_mt -pthread)
