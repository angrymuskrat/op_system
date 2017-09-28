cmake_minimum_required ( VERSION 2.6 )
project ( matrix_adder )

set ( APP_NAME  matrix_adder )

set( CMAKE_BUILD_TYPE RELEASE )

add_executable ( adder_mp adder_mp.c )

