cmake_minimum_required(VERSION 3.10)

project(MyProject)

# Add the source files
set(SOURCES
    src/main.cpp
)

# Add the header files
include_directories(include)

# Create the executable
add_executable(engine ${SOURCES})