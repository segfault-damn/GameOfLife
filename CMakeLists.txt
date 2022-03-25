cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(src_dir src)
set(lifeGame_src ${src_dir}/game.cpp)

set(wxWidgets_CONFIGURATION mswud)
set(wxWidgets_ROOT_DIR C:/wxWidgets-3.1.5)
set(wxWidgets_LIB_DIR C:/wxWidgets-3.1.5/lib/vc_x64_lib)
# Use the package PkgConfig to detect GTK+ headers/library files
find_package(wxWidgets COMPONENTS net core base REQUIRED) 
include(${wxWidgets_USE_FILE})

project(life_game)

add_library(life_game ${lifeGame_src})

target_include_directories(life_game PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<INSTALL_INTERFACE:include>  
)

add_executable(life_game_exe WIN32 main.cpp ${src_dir}/LifeApp.cpp ${src_dir}/LifeFrame.cpp)
target_link_libraries(life_game_exe life_game)
target_link_libraries(life_game_exe ${wxWidgets_LIBRARIES})