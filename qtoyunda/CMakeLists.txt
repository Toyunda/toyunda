project(qtoyunda)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER QUIET gstreamer-0.10)
pkg_check_modules(QTGSTREAMER QUIET QtGStreamer-0.10)
IF (GSTREAMER_FOUND)
Message("Gstreamer found")
ENDIF (GSTREAMER_FOUND)

IF (QTGSTREAMER_FOUND)
Message("QtGstreamer found")
Message(${QTGSTREAMER_INCLUDE_DIRS})
Message(${QTGSTREAMER_LIBRARIES})
ENDIF (QTGSTREAMER_FOUND)

CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/allplayer.h.in" "${PROJECT_SOURCE_DIR}/allplayer.h")

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(qtoyunda_BASE_SRC fileplayer.cpp toyundasubstream.cpp toyundasub.cpp toyundarenderer.cpp scomponent.cpp sqarg.cpp)
set(qtoyunda_BASE_HEADERS fileplayer.h toyundasubstream.h toyundasub.h toyundarenderer.h toyundasub.hxx scomponent.h sqarg.h)
set(qtoyunda_BASE_HEADERS_QOBJECT fileplayer.h toyundasubstream.h)

# Debug
set(qtoyundadebug_SRCS ${qtoyunda_BASE_SRC} debugmain.cpp debugrenderer.cpp fakeplayer.cpp rawsubstream.cpp)
set(qtoyundadebug_HEADERS ${qtoyunda_BASE_HEADERS} debugrenderer.h fakeplayer.h rawsubstream.h)
set(qtoyundadebug_HEADERS_QOBJECT ${qtoyunda_BASE_HEADERS_QOBJECT} debugrenderer.h fakeplayer.h rawsubstream.h)

# Basic qtoyunda launcher
	set(qtoyunda_SRCS ${qtoyunda_BASE_SRC} main.cpp qtoyunda.cpp debugrenderer.cpp fakeplayer.cpp rawsubstream.cpp
						toyundadrawer.cpp qosd.cpp)
	set(qtoyunda_HEADERS ${qtoyunda_BASE_HEADERS} qtoyunda.h debugrenderer.h fakeplayer.h rawsubstream.h 
						toyundadrawer.h qosd.h)
	set(qtoyunda_HEADERS_QOBJECT ${qtoyunda_BASE_HEADERS_QOBJECT} qtoyunda.h debugrenderer.h fakeplayer.h rawsubstream.h qosd.h)
	# Add optionnal renderer and player
		IF (QTGSTREAMER_FOUND)
			include_directories(${QTGSTREAMER_INCLUDE_DIRS})
			set(qtoyunda_OPTIONNAL_LIBRARY ${QTGSTREAMER_LIBRARIES})
			set(qtoyunda_SRCS ${qtoyunda_SRCS} qgstaudioplayer.cpp)
			set(qtoyunda_HEADERS ${qtoyunda_HEADERS} qgstaudioplayer.h)
			set(qtoyunda_HEADERS_QOBJECT ${qtoyunda_HEADERS_QOBJECT} qgstaudioplayer.h)
		ENDIF(QTGSTREAMER_FOUND)
  

QT4_WRAP_CPP(qtoyundadebug_HEADERS_MOC ${qtoyundadebug_HEADERS_QOBJECT})
QT4_WRAP_CPP(qtoyunda_HEADERS_MOC ${qtoyunda_HEADERS_QOBJECT})
add_executable(qtoyunda-debug ${qtoyundadebug_SRCS} ${qtoyundadebug_HEADERS_MOC})
add_executable(qtoyunda ${qtoyunda_SRCS} ${qtoyunda_HEADERS_MOC})
target_link_libraries(qtoyunda-debug ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
target_link_libraries(qtoyunda ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${qtoyunda_OPTIONNAL_LIBRARY})
