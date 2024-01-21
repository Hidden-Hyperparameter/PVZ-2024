add_library(gui_external ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.cpp)
add_subdirectory(Vulkan-Headers)
add_subdirectory(glfw)
add_subdirectory(glm)
message("
--------------------------------------------\n
If there are some errors with these headers, please make sure that you have cloned the Github repository RECURSIVELY !!!!\n
------------------------------------------\n"
)
target_include_directories(gui_external INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gui_external INTERFACE Vulkan::Headers glfw glm)
