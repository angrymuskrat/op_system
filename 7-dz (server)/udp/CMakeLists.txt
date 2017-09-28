cmake_minimum_required ( VERSION 2.6 )
project ( server_udp )

set ( APP_NAME  server_udp )

set( CMAKE_BUILD_TYPE RELEASE )

add_executable ( client client_udp.c )
add_executable ( server server_udp.c )
target_link_libraries(server -pthread)
