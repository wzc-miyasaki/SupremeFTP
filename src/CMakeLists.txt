project(SupremeFTP)

add_library(networkanalyzer NetworkAnalyzer.cc)
add_library(readfile ReadFile.cc)
target_include_directories(networkanalyzer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(readfile PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

