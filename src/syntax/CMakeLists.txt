

add_executable(syntax main.c output.c readcells.c ${CMAKE_SOURCE_DIR}/src/date/date.c)

target_link_libraries(syntax ycadgraywolf)
target_link_libraries(syntax ${X11_LIBRARIES})
target_link_libraries(syntax m)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/include . ${X11_INCLUDE_DIR})

install(TARGETS syntax DESTINATION lib/graywolf/bin)
