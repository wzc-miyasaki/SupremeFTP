project(SupremeFTP)

add_executable(main main.cc)
add_executable(server server.cc)
add_executable(client client.cc)

target_link_libraries(main PRIVATE networkanalyzer)
target_link_libraries(main PRIVATE readfile)
target_link_libraries(server PRIVATE readfile)