cmake_minimum_required(VERSION 2.8)
project(gstreamer-toyunda)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB_PKG glib-2.0)
pkg_check_modules(GSTREAMER_PKG gstreamer-0.10)
pkg_check_modules(GSTREAMER_BASE_PKG gstreamer-base-0.10)
pkg_check_modules(GSTREAMER_VIDEO_PKG gstreamer-video-0.10)
pkg_check_modules(PANGO_PKG pango)
pkg_check_modules(PANGOCAIRO_PKG pangocairo)

IF (GLIB_PKG_FOUND)
    message(Found glib-2.0)
	link_libraries(${GLIB_PKG_LDFLAGS})
	include_directories(${GLIB_PKG_INCLUDE_DIRS})
ENDIF()

IF (GSTREAMER_PKG_FOUND)
    message(Found gstreamer-0.10)
	link_libraries(${GSTREAMER_PKG_LDFLAGS})
	include_directories(${GSTREAMER_PKG_INCLUDE_DIRS})
ENDIF()

IF (GSTREAMER_BASE_PKG_FOUND)
    message(Found gstreamer-base-0.10)
	link_libraries(${GSTREAMER_BASE_PKG_LDFLAGS})
	include_directories(${GSTREAMER_BASE_PKG_INCLUDE_DIRS})
ENDIF()

IF (GSTREAMER_BASE_PKG_FOUND)
    message(Found gstreamer-video-0.10)
	link_libraries(${GSTREAMER_VIDEO_PKG_LDFLAGS})
	include_directories(${GSTREAMER_VIDEO_PKG_INCLUDE_DIRS})
ENDIF()

IF (PANGO_PKG_FOUND)
    message(Found pango)
	link_libraries(${PANGO_PKG_LDFLAGS})
	include_directories(${PANGO_PKG_INCLUDE_DIRS})
ENDIF()

IF (PANGOCAIRO_PKG_FOUND)
    message(Found pangocairo)
	link_libraries(${PANGOCAIRO_PKG_LDFLAGS})
	include_directories(${PANGOCAIRO_PKG_INCLUDE_DIRS})
ENDIF()


set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -W -Wextra -ggdb")

add_executable(toyunda-player toyunda-player.c)
add_executable(gstreamer-toyunda toyundaparse.c)
add_library(gsttoyunda SHARED gsttoyunda.c)


target_link_libraries(gsttoyunda ${GSTREAMER_PKG_LIBRARIES} ${GSTREAMER_BASE_PKG_LIBRARIES} 
				${GSTREAMER_VIDEO_PKG_LIBRARIES} ${PANGO_PKG_LIBRARIES}
				${PANGOCAIRO_PKG_LIBRARIES})


target_link_libraries(gstreamer-toyunda ${GLIB_PKG_LIBRARIES})
target_link_libraries(toyunda-player ${GSTREAMER_PKG_LIBRARIES})

install(TARGETS gstreamer-toyunda RUNTIME DESTINATION bin)
