project(SupremeFTP)

add_library(networkanalyzer NetworkAnalyzer.cc)
target_include_directories(networkanalyzer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)