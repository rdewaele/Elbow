cmake_minimum_required (VERSION 3.10)
project (Elbow)

# add the main source directory
add_subdirectory (src)

set (CMAKE_BUILD_TYPE DEV)
set (CMAKE_C_FLAGS_DEV
	"-g -O0 -pipe -pedantic -std=c99
	-W -Wall -Wextra -Wundef -Wendif-labels -Wshadow	-Wpointer-arith
	-Wbad-function-cast -Wcast-align -Wwrite-strings -Wstrict-prototypes
	-Wmissing-prototypes -Wnested-externs -Winline -Wdisabled-optimization
	-Wno-missing-field-initializers"
	)

SET(CMAKE_BUILD_TYPE distribution)
SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")
SET(CMAKE_C_FLAGS_DISTRIBUTION "-O3")

# normally the elbow executable is saved in the main source directory, but we
# want it to appear in this directory instead
set_target_properties (
	elbow
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
	)
