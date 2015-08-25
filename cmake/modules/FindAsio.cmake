###############################################################################
# Find Asio
#
# This sets the following variables:
# ASIO_FOUND - True if Asio was found.
# ASIO_INCLUDE_DIRS - Directories containing the Asio include files.

if(WIN32)
    find_path(ASIO_INCLUDE_DIR asio.hpp PATHS "/usr/include" "/usr/local/include" "/usr/x86_64-w64-mingw32/include" "$ENV{PROGRAMFILES}" NO_DEFAULT_PATHS)

else(WIN32)
    find_path(ASIO_INCLUDE_DIR asio.hpp PATHS "/usr/include" "/usr/local/include" "$ENV{PROGRAMFILES}" NO_DEFAULT_PATHS)
endif(WIN32)

set(ASIO_INCLUDE_DIRS ${ASIO_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Asio DEFAULT_MSG ASIO_INCLUDE_DIR)

mark_as_advanced(ASIO_INCLUDE_DIR)
