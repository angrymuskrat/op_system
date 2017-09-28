cmake_minimum_required ( VERSION 2.6 )
project ( graph_thread )

set ( APP_NAME  graph_thread )

set( CMAKE_BUILD_TYPE RELEASE )

add_executable ( ${APP_NAME} pthreader.c )

target_link_libraries(${APP_NAME} -pthread)
