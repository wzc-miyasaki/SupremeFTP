project(SupremeFTP)

# Find the threads package
find_package(Threads REQUIRED)


# Create Executables
add_executable(server server.cc)
add_executable(client client.cc)


# Link Library
target_link_libraries(server PRIVATE readfile)
target_link_libraries(server PRIVATE networkanalyzer)
target_link_libraries(client PRIVATE networkanalyzer)

