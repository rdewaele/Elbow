cmake_minimum_required (VERSION 2.6)
project (Elbow)

# version number
set (Elbow_VERSION_NUMBER_MAJOR 0)
set (Elbow_VERSION_NUMBER_MINOR 2)

# compiler flags
set (CWARNINGS
	-W -Wall -Wextra -Wundef -Wendif-labels -Wshadow	-Wpointer-arith
	-Wbad-function-cast -Wcast-align -Wwrite-strings -Wstrict-prototypes
	-Wmissing-prototypes -Wnested-externs -Winline -Wdisabled-optimization
	-Wno-missing-field-initializers
	)
set (CFLAGS
	-g -O0 -pipe -pedantic -std=c99 ${CWARNINGS}
	)

if (UNIX)
	if (CMAKE_COMPILER_IS_GNUCC)
		add_definitions (${CFLAGS})
	endif (CMAKE_COMPILER_IS_GNUCC)
endif (UNIX)

# configure a header file to pass some of the CMake settings to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/elbow_config.h.in"
	"${PROJECT_BINARY_DIR}/elbow_config.h"
	)

# add the binary tree to the search path for include files so that we will find
# elbow_config.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable (elbow bowshell.c main.c options.c sendfiles.c serial.c)
target_link_libraries (elbow popt readline)
