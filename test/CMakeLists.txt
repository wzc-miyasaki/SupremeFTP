project(SupremeFTP)

add_executable(main main.cc)
target_link_libraries(main PRIVATE networkanalyzer)
target_link_libraries(main PRIVATE readfile)
