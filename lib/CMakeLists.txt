file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(lib ${SOURCES} ${HEADERS})
# add_subdirectory(${PROJECT_SOURCE_DIR}/external/glfw PARENT_SCOPE)
add_subdirectory(${PROJECT_SOURCE_DIR}/gui PARENT_SCOPE)
target_include_directories(lib PUBLIC gui_test)
# target_link_libraries(lib PUBLIC gui_test)
# target_link_directories(lib PUBLIC external)
# target_link_libraries(lib PUBLIC external)
# target_link_libraries(lib PUBLIC glfw)


