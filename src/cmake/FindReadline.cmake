# FindReadline.cmake - Portable, checks for GNU Readline by probing for rl_free
# macOS provides libedit in lieu of libreadline, but it's not sufficiently compatible for Elbow

# Defines:
#  - READLINE_FOUND
#  - READLINE_INCLUDE_DIRS
#  - READLINE_LIBRARIES

include(CheckCSourceCompiles)

find_package(PkgConfig QUIET)

# Optional pkg-config search
if (PKG_CONFIG_FOUND)
  pkg_search_module(PC_READLINE QUIET readline)
endif()

# On macOS, use Homebrew hints
if(APPLE)
    set(READLINE_HINTS
        /opt/homebrew/include
        /opt/homebrew/opt/readline
        /usr/local/opt/readline
    )
else()
    set(READLINE_HINTS "")
endif()

# Find headers
find_path(READLINE_INCLUDE_DIR
    readline/readline.h
    readline/history.h
    HINTS
      ${PC_READLINE_INCLUDEDIR}
      ${PC_READLINE_INCLUDE_DIRS}
      ${READLINE_HINTS}
    PATH_SUFFIXES include
)

# Find library
find_library(READLINE_LIBRARY
    NAMES readline
    HINTS
      ${PC_READLINE_LIBDIR}
      ${PC_READLINE_LIBRARY_DIRS}
      ${READLINE_HINTS}
    PATH_SUFFIXES lib
)

if (READLINE_INCLUDE_DIR AND READLINE_LIBRARY)
    set(CMAKE_REQUIRED_INCLUDES ${READLINE_INCLUDE_DIR})
    set(CMAKE_REQUIRED_LIBRARIES ${READLINE_LIBRARY})

    # Check for real GNU readline by probing for rl_free
    check_c_source_compiles("
        #include <stdio.h>
        #include <readline/readline.h>
        int main() { rl_free((char*)0); return 0; }
    " HAVE_RL_FREE)

    if (HAVE_RL_FREE)
        set(READLINE_FOUND TRUE)
        set(READLINE_INCLUDE_DIRS ${READLINE_INCLUDE_DIR})
        set(READLINE_LIBRARIES ${READLINE_LIBRARY})
        message(STATUS "Found GNU Readline with rl_free (not libedit).")
    else()
        set(READLINE_FOUND FALSE)
        message(FATAL_ERROR "Found readline, but it does not have rl_free (likely libedit).")
    endif()
else()
    set(READLINE_FOUND FALSE)
    message(FATAL_ERROR "GNU Readline not found! Searched in:
${READLINE_HINTS}
Also checked pkg-config hints: ${PC_READLINE_INCLUDEDIR} ${PC_READLINE_LIBDIR}
On macOS, install with: brew install readline")
endif()

mark_as_advanced(READLINE_INCLUDE_DIR READLINE_LIBRARY)

# Create an imported target if found
if(READLINE_FOUND AND NOT TARGET readline::readline)
  add_library(readline::readline UNKNOWN IMPORTED)
  set_target_properties(readline::readline PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    IMPORTED_LOCATION "${READLINE_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${READLINE_INCLUDE_DIRS}"
  )
endif()
