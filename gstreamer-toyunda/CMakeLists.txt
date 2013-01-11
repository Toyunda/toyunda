cmake_minimum_required(VERSION 2.8)
project(gstreamer-toyunda)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB_PKG glib-2.0)

IF (GLIB_PKG_FOUND)
    message(Found glib-2.0)
	link_libraries(${GLIB_PKG_LDFLAGS})
	include_directories(${GLIB_PKG_INCLUDE_DIRS})
ENDIF()

set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -W -Wextra -ggdb")


add_executable(gstreamer-toyunda toyundaparse.c)
target_link_libraries(gstreamer-toyunda ${GLIB_PKG_LIBRARIES})

install(TARGETS gstreamer-toyunda RUNTIME DESTINATION bin)
