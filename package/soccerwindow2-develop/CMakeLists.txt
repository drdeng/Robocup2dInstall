cmake_minimum_required(VERSION 3.5)

project(soccerwindow2 VERSION 2020.08)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# compliler optionsqc
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()
set(CMAKE_CXX_FLAGS "-W -Wall")

# install destination
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local" CACHE PATH "Install destination path" FORCE)
endif()

# check librcsc
set(LIBRCSC_INSTALL_DIR "$ENV{HOME}/.local;$ENV{HOME}/local" CACHE PATH "The path where librcsc is installed.")

find_library(LIBRCSC_LIB NAMES rcsc
  PATHS ${LIBRCSC_INSTALL_DIR}
  PATH_SUFFIXES lib
  )
if(NOT LIBRCSC_LIB)
  message(FATAL_ERROR "librcsc not found!")
endif()

find_path(LIBRCSC_INCLUDE_DIR
  NAME rcsc/types.h
  PATHS ${LIBRCSC_INSTALL_DIR}
  PATH_SUFFIXES include
  )
if(NOT LIBRCSC_INCLUDE_DIR)
  message(FATAL_ERROR "librcsc include dir not found!")
endif()

# remove variables from GUI
mark_as_advanced(
  LIBRCSC_LIB
  LIBRCSC_INCLUDE_DIR
  )


# boost
find_package(Boost 1.36.0 COMPONENTS system REQUIRED)
if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not found!")
endif()

# Qt5 or Qt4
set(CMAKE_AUTOMOC ON)
find_package(Qt5 5.1.0 COMPONENTS Core Gui Widgets Network REQUIRED)
if(NOT Qt5_FOUND)
  find_package(Qt4 4.1.0 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
  if(NOT Qt4_FOUND)
	message(FATAL_ERROR "Qt not found!")
  endif()
endif()

if(Qt5_FOUND)
  list(APPEND QT_LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
elseif(Qt4_FOUND)
  list(APPEND QT_LIBS Qt4::QtCore Qt4::QtGui Qt4::QtNetwork)
endif()

# zlib
find_package(ZLIB)
if(ZLIB_FOUND)
  set(HAVE_LIBZ TRUE)
endif()

# generate config.h
add_definitions(-DHAVE_CONFIG_H)
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake-config.h.in
  ${PROJECT_BINARY_DIR}/config.h
  )

# check the settings
message(STATUS "Found librcsc:")
message(STATUS "  LIBRCSC_LIB=${LIBRCSC_LIB}")
message(STATUS "  LIBRCSC_INCLUDE_DIR=${LIBRCSC_INCLUDE_DIR}")
message(STATUS "Build settings:")
message(STATUS "  BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "  INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

add_subdirectory(src)

install(FILES xpm/soccerwindow2.xpm
  DESTINATION share/soccerwindow2
  )
