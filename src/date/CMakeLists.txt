

add_executable(getdate getdate.c)
add_library(date date.c)
include_directories(${CMAKE_SOURCE_DIR}/src/genrows)