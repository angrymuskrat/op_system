cmake_minimum_required ( VERSION 2.6 )
project ( graph_fork )

set ( APP_NAME  graph_fork )

set( CMAKE_BUILD_TYPE RELEASE )

add_executable ( ${APP_NAME} forker.c )
