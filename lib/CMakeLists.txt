file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(lib ${SOURCES} ${HEADERS})
# add_subdirectory(${PROJECT_SOURCE_DIR}/gui PARENT_SCOPE) 
# FATAL: the line above just can't be added!
target_include_directories(lib PUBLIC gui/lib/include)
target_link_directories(lib PUBLIC gui/lib)
target_link_libraries(lib PUBLIC gui_lib)


