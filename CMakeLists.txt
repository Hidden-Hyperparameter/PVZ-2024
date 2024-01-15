cmake_minimum_required(VERSION 3.1.9)

project(PVZ2023)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

#set(EASYX_DIR "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/VS")

#include_directories(${EASYX_DIR}/include)
#link_directories(${EASYX_DIR}/lib)

add_library(example STATIC 
game.cpp 
game_parameters.cpp 
helper.cpp 
pea.cpp 
peashooter.cpp 
plant.cpp 
sun.cpp
sunflower.cpp
test.cpp
unit.cpp
zombie.cpp)

#add_library(graph STATIC C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/VS/lib/x64/EasyXa.lib)

add_executable(main main.cpp)

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/include")
#INCLUDE_DIRECTORIES("C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/VS/include")
#INCLUDE_DIRECTORIES("C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/VS/lib/x64")
#link_directories(C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/VS/lib/x64)
#target_link_libraries(main example)
target_link_libraries(main example)