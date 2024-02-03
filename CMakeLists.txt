cmake_minimum_required(VERSION 3.10)

project(Log-Structured-KV-Storage-Engine)

# Add the source files
set(SOURCES
    src/main.cpp
    src/KeyValueStore
)

# Add the header files
include_directories(headers)

# Create the executable
add_executable(engine ${SOURCES})