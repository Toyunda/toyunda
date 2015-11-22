cmake_minimum_required(VERSION 2.8)
project(gstreamer-toyunda)

set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/../../build)

IF (WIN32)
	
ELSE(WIN32)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GLIB_PKG glib-2.0)
	pkg_check_modules(GSTREAMER_PKG gstreamer-1.0)
	pkg_check_modules(GSTREAMER_BASE_PKG gstreamer-base-1.0)
	pkg_check_modules(GSTREAMER_VIDEO_PKG gstreamer-video-1.0)
	pkg_check_modules(PANGO_PKG pango)
	pkg_check_modules(PANGOCAIRO_PKG pangocairo)
	pkg_check_modules(GTK2_PKG gtk+-2.0)
	pkg_check_modules(GIO2_PKG gio-2.0)
	

	IF (GLIB_PKG_FOUND)
	    message("Found glib-2.0")
		link_libraries(${GLIB_PKG_LDFLAGS})
		include_directories(${GLIB_PKG_INCLUDE_DIRS})
	else()
		message("glib-2.0 not found")
		set (FAIL_LIB_CHECK 1)
	ENDIF()

	IF (GSTREAMER_PKG_FOUND)
	    message("Found gstreamer-1.0")
		link_libraries(${GSTREAMER_PKG_LDFLAGS})
		include_directories(${GSTREAMER_PKG_INCLUDE_DIRS})
	else()
		message("gstreamer-1.0 not found")
		set (FAIL_LIB_CHECK 1)
	ENDIF()

	IF (GSTREAMER_BASE_PKG_FOUND)
	    message("Found gstreamer-base-1.0")
		link_libraries(${GSTREAMER_BASE_PKG_LDFLAGS})
		include_directories(${GSTREAMER_BASE_PKG_INCLUDE_DIRS})
	else()
		message("gstreamer-base-1.0 not found")
		set (FAIL_LIB_CHECK 1)	
	ENDIF()

	IF (GSTREAMER_BASE_PKG_FOUND)
	    message("Found gstreamer-video-1.0")
		link_libraries(${GSTREAMER_VIDEO_PKG_LDFLAGS})
		include_directories(${GSTREAMER_VIDEO_PKG_INCLUDE_DIRS})
	else()
		message("gstreamer-video-1.0 not found")
		set (FAIL_LIB_CHECK 1)
	ENDIF()

	
	IF (PANGO_PKG_FOUND)
	    message("Found pango")
		link_libraries(${PANGO_PKG_LDFLAGS})
		include_directories(${PANGO_PKG_INCLUDE_DIRS})
	else()
		message("pango not found")
		set (FAIL_LIB_CHECK 1)
	ENDIF()

	IF (PANGOCAIRO_PKG_FOUND)
	    message("Found pangocairo")
		link_libraries(${PANGOCAIRO_PKG_LDFLAGS})
		include_directories(${PANGOCAIRO_PKG_INCLUDE_DIRS})
	else()
		message("pangocairo not found")
		set (FAIL_LIB_CHECK 1)
	ENDIF()

	IF (GTK2_PKG_FOUND)
                message("Found GTK2+")
                link_libraries(${GTK2_PKG_LDFLAGS})
                include_directories(${GTK2_PKG_INCLUDE_DIRS})
        else()
                message("gtk+2.0 not found")
                set (FAIL_LIB_CHECK 1)
        ENDIF()
	
	IF (GIO2_PKG_FOUND)
		message("Found GIO2")
		link_libraries(${GIO2_PKG_LDFLAGS})
		include_directories(${GIO2_PKG_INCLUDE_DIRS})
	else()
		message("gio2.0 not found")
		set (FAIL_LIB_CHECK 1)
	ENDIF()

ENDIF()
if (FAIL_LIB_CHECK)
	return()
endif()
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -W -ggdb -Wno-unused-variable -Wno-sign-compare")

add_executable(toyunda-player toyunda-player2.c)
#add_executable(gstreamer-toyunda toyundaparse.c)
add_library(gsttoyunda SHARED gsttoyunda.c)


target_link_libraries(gsttoyunda ${GSTREAMER_PKG_LIBRARIES} ${GSTREAMER_BASE_PKG_LIBRARIES} 
				${GSTREAMER_VIDEO_PKG_LIBRARIES} ${PANGO_PKG_LIBRARIES}
				${PANGOCAIRO_PKG_LIBRARIES})


target_link_libraries(toyunda-player ${GSTREAMER_PKG_LIBRARIES} ${GTK2_PKG_LIBRARIES} 
					${GIO2_PKG_LIBRARIES})



GET_TARGET_PROPERTY(TOYUNDA_PLAYER_BIN_NAME toyunda-player LOCATION)
GET_TARGET_PROPERTY(GST_TOYUNDA_BIN_NAME gsttoyunda LOCATION)

# copy bin to binary folder
ADD_CUSTOM_COMMAND(TARGET toyunda-player
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${TOYUNDA_PLAYER_BIN_NAME} ${PROJECT_BINARY_DIR}/.
)

ADD_CUSTOM_COMMAND(TARGET gsttoyunda
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${GST_TOYUNDA_BIN_NAME} ${PROJECT_BINARY_DIR}/gsttoyunda/.
)
