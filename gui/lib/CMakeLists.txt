file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(gui_lib ${SOURCES} ${HEADERS})

add_subdirectory(${PROJECT_SOURCE_DIR}/gui/external PARENT_SCOPE)
target_include_directories(gui_lib PUBLIC gui_external)
target_link_libraries(gui_lib PUBLIC gui_external)
# target_link_directories(lib PUBLIC external)
# target_link_libraries(lib PUBLIC external)
# target_link_libraries(lib PUBLIC glfw)

find_package(Vulkan)

# Check if Vulkan was found
if(NOT Vulkan_FOUND)
    message(WARNING "Vulkan not found. Please make sure Vulkan is installed.")
else()
    message(STATUS "Vulkan found: ${Vulkan_LIBRARY}")
    # Include Vulkan in the project
    include_directories(${Vulkan_INCLUDE_DIRS})
    link_libraries(${Vulkan_LIBRARIES})
endif()

# Link libraries with the executable
target_link_libraries(gui_lib PUBLIC gui_external ${Vulkan_LIBRARIES})
