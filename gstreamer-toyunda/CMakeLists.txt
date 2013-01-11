cmake_minimum_required(VERSION 2.8)
project(gstreamer-toyunda)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB_PKG glib-2.0)

IF (GLIB_PKG_FOUND)
    message(Found glib-2.0)
include_directories(${GLIB_PKG_INCLUDE_DIRS})
ENDIF()


add_executable(gstreamer-toyunda toyundaparse.c)
target_link_libraries(gstreamer-toyunda ${GLIB_PKG_LIBRARIES})

install(TARGETS gstreamer-toyunda RUNTIME DESTINATION bin)
