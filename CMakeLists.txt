cmake_minimum_required(VERSION 3.24.1) 

# set the project name 
project(mcfunction_generator) 

# set(CMAKE_CXX_STANDADR 14)

add_compile_options(-std=c++11 -Wall)

include_directories(include)

# configure_file(mcfunction_generator.h.in lexer.hpp)

# add the executable 
add_executable(build_test Minecraft-function-generator.cpp include/lexer.hpp include/parser.hpp include/backend.hpp) 

# target_include_directories(mcfunction_generator PUBLIC Minecraft-function-generator.cpp lexer.hpp)
