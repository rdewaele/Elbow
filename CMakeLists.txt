cmake_minimum_required(VERSION 3.10)
project(Elbow)

# Add the main source directory
add_subdirectory(src)

# Place the final executable in the top-level build dir
set_target_properties(elbow PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)
