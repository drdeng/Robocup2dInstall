# FindLibrcsc
#
# Acceptable environment variable: LIBRCSC_PREFIX
#
# librcsc_FOUND
# librcsc_LIBRARY
# librcsc_INCLUDE_DIR
# librcsc_LIB_DIR

if(NOT librcsc_FOUND)

  message(STATUS "start FindLibrcsc")
  set(LIBRCSC_INSTALL_DIR "$ENV{HOME}/.local" CACHE PATH "The path where librcsc is installed.")

  find_path(librcsc_INCLUDE_DIR
	NAMES rcsc/types.h
	PATHS ${LIBRCSC_INSTALL_DIR} $ENV{HOME}/.local $ENV{HOME}/local ENV LIBRCSC_PREFIX
	PATH_SUFFIXES include
	)

  find_library(librcsc_LIBRARY
	NAMES rcsc
	PATHS ${LIBRCSC_INSTALL_DIR} $ENV{HOME}/.local $ENV{HOME}/local ENV LIBRCSC_PREFIX
	PATH_SUFFIXES lib
	)
  get_filename_component(librcsc_LIB_DIR ${librcsc_LIBRARY} DIRECTORY)

  mark_as_advanced(
	librcsc_LIBRARY
	librcsc_INCLUDE_DIR
	librcsc_LIB_DIR
	)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(librcsc
	DEFAULT_MSG
	librcsc_LIBRARY
    librcsc_INCLUDE_DIR
	librcsc_LIB_DIR
	)

  if(librcsc_FOUND)
	add_library(rcsc SHARED IMPORTED)
	set_target_properties(rcsc PROPERTIES
	  IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
	  IMPORTED_LOCATION "${librcsc_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${librcsc_INCLUDE_DIR}"
	  )
  endif()

endif()
