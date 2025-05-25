# FindPopt.cmake - Portable, checks for libpopt (options parsing library)
#
# Defines:
#  - POPT_FOUND
#  - POPT_INCLUDE_DIR
#  - POPT_LIBRARY
#  - popt::popt (IMPORTED target, if found)

find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
  pkg_search_module(PC_POPT QUIET popt)
endif()

# Find headers and library
find_path(POPT_INCLUDE_DIR
  NAMES popt.h
  HINTS
    ${PC_POPT_INCLUDEDIR}
    ${PC_POPT_INCLUDE_DIRS}
  PATH_SUFFIXES include
  DOC "Path to popt.h header"
)

find_library(POPT_LIBRARY
  NAMES popt
  HINTS
    ${PC_POPT_LIBRARYDIR}
    ${PC_POPT_LIBRARY_DIRS}
  PATH_SUFFIXES lib
  DOC "popt library path"
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Popt
  REQUIRED_VARS POPT_INCLUDE_DIR POPT_LIBRARY
  VERSION_VAR PC_POPT_VERSION
)

mark_as_advanced(POPT_INCLUDE_DIR POPT_LIBRARY)

# Create an imported target if found
if (POPT_FOUND AND NOT TARGET popt::popt)
  add_library(popt::popt UNKNOWN IMPORTED)
  set_target_properties(popt::popt PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    IMPORTED_LOCATION "${POPT_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${POPT_INCLUDE_DIR}"
  )
endif()
