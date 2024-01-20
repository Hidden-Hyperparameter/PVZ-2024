# cmake_minimum_required(VERSION 3.11)
add_executable(gui_test main.cpp)
add_subdirectory(lib)
#add_subdirectory(external/Vulkan-Headers)
target_include_directories(gui_test PUBLIC lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)
target_link_directories(gui_test PUBLIC lib external)
target_link_libraries(gui_test PUBLIC gui_lib gui_external)
# file(GLOB_RECURSE GLSL_SOURCE_FILES "${PROJECT_SOURCE_DIR}/shaders/*.spv"
# )
# Find all vertex and fragment sources within shaders directory
# taken from VBlancos vulkan tutorial
# https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt
# message("Compiling Shaders...") 
find_program(GLSL_VALIDATOR glslangValidator REQUIRED_FILES 
  ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
  /usr/bin 
  /usr/local/bin 
  ${VULKAN_SDK_PATH}/Bin
  ${VULKAN_SDK_PATH}/Bin32
  $ENV{VULKAN_SDK}/Bin/ 
  $ENV{VULKAN_SDK}/Bin32/
)

# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag"
  "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert"
)


foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)

  set(SPIRV "${CMAKE_CURRENT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND echo "Compiling shader file ${GLSL}"
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(gui_test Shaders)