cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(Elbow)

set(Elbow_VERSION_NUMBER_MAJOR 0)
set(Elbow_VERSION_NUMBER_MINOR 2)

add_executable(elbow bowshell.c main.c options.c sendfiles.c serial.c)

target_compile_options(elbow PRIVATE
	-g -O0 -pipe -pedantic -std=c99
	-W -Wall -Wextra -Wundef -Wendif-labels -Wshadow
	-Wpointer-arith -Wbad-function-cast -Wcast-align
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes
	-Wnested-externs -Winline -Wdisabled-optimization
	-Wno-missing-field-initializers
)

# configure a header file to pass some of the CMake settings to the source code
configure_file(
	"${PROJECT_SOURCE_DIR}/elbow_config.h.in"
	"${PROJECT_BINARY_DIR}/elbow_config.h"
)

find_package(Popt REQUIRED)
find_package(Readline REQUIRED)

target_include_directories(elbow PRIVATE
	${READLINE_INCLUDE_DIR}
	${POPT_INCLUDE_DIR}
	# contains elbow_config.h
	"${PROJECT_BINARY_DIR}"
)

target_link_libraries(elbow PRIVATE popt::popt readline::readline)
