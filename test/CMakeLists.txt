project(SupremeFTP)

# Find the threads package
find_package(Threads REQUIRED)


# Create Executables
add_executable(main main.cc)
add_executable(server server.cc)
add_executable(client client.cc)
add_executable(mixServer selectSocket.cc)
add_executable(mixTCPclient tcpclient.cc)
add_executable(mixUDPclient udpclient.cc)
add_executable(fatClient fatclient.cc)


# Link Library
target_link_libraries(main PRIVATE networkanalyzer)
target_link_libraries(main PRIVATE readfile)

target_link_libraries(server PRIVATE readfile)
target_link_libraries(server PRIVATE networkanalyzer)
target_link_libraries(client PRIVATE networkanalyzer)

target_link_libraries(mixServer PRIVATE Threads::Threads)
