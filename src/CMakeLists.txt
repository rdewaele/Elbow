cmake_minimum_required (VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

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

#set (CMAKE_C_FLAGS -03)

#if (UNIX)
#	if (CMAKE_COMPILER_IS_GNUCC)
#		set (TEMP ${CMAKE_C_FLAGS})
#		set (CMAKE_C_FLAGS "")
#		add_definitions (${CFLAGS} ${TEMP})
#	endif (CMAKE_COMPILER_IS_GNUCC)
#endif (UNIX)

set (CMAKE_C_FLAGS_DEVELOPMENT "-g -O0 -pipe -pedantic -std=c99 -W -Wall -Wextra -Wundef -Wendif-labels -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wnested-externs -Winline -Wdisabled-optimization -Wno-missing-field-initializers"
	)

# configure a header file to pass some of the CMake settings to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/elbow_config.h.in"
	"${PROJECT_BINARY_DIR}/elbow_config.h"
	)

# add the executable
add_executable (elbow bowshell.c main.c options.c sendfiles.c serial.c)

# configure dependencies
find_package(POPT)
find_package(Readline)

include_directories(${Readline_INCLUDE_DIR} ${POPT_INCLUDE_DIR})

# add the binary tree to the search path for include files so that we will find
# elbow_config.h
include_directories("${PROJECT_BINARY_DIR}")

target_link_libraries (elbow PRIVATE ${Readline_LIBRARY} ${POPT_LIBRARY})
