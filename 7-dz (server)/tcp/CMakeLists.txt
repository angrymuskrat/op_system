cmake_minimum_required ( VERSION 2.6 )
project ( server_tcp )

set ( APP_NAME  server_tcp )

set( CMAKE_BUILD_TYPE RELEASE )

add_executable ( client client_tcp.c )
add_executable ( server server_tcp.c )
target_link_libraries(server -pthread)
