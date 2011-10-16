project(GuiliGuili)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-0.10)
pkg_check_modules(QTGSTREAMER REQUIRED QtGStreamer-0.10)
IF (GSTREAMER_FOUND)
Message("Gstreamer found")
ENDIF (GSTREAMER_FOUND)

IF (QTGSTREAMER_FOUND)
Message("QtGstreamer found")
Message(${QTGSTREAMER_INCLUDE_DIRS})
Message(${QTGSTREAMER_LIBRARIES})
ENDIF (QTGSTREAMER_FOUND)

include_directories(${QTGSTREAMER_INCLUDE_DIRS})

#Configure QToyunda files
set(FAKEPLAYER 0)
set(DEBUGRENDERER 0)
set(QTOSD 1)



set(QTOYUNDA_DIR "${CMAKE_SOURCE_DIR}/../qtoyunda/")
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${QTOYUNDA_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

CONFIGURE_FILE("${QTOYUNDA_DIR}/allplayer.h.in" "${QTOYUNDA_DIR}/allplayer.h")
CONFIGURE_FILE("${QTOYUNDA_DIR}/allrenderer.h.in" "${QTOYUNDA_DIR}/allrenderer.h")



set(QTOYUNDA_SRC ${QTOYUNDA_DIR}/qtoyunda.cpp ${QTOYUNDA_DIR}/fileplayer.cpp ${QTOYUNDA_DIR}/toyundasubstream.cpp ${QTOYUNDA_DIR}/toyundasub.cpp
		${QTOYUNDA_DIR}/toyundarenderer.cpp ${QTOYUNDA_DIR}/scomponent.cpp ${QTOYUNDA_DIR}/sqarg.cpp
		${QTOYUNDA_DIR}/qtoyunda.cpp ${QTOYUNDA_DIR}/debugrenderer.cpp ${QTOYUNDA_DIR}/qgstaudioplayer.cpp 
		${QTOYUNDA_DIR}/rawsubstream.cpp ${QTOYUNDA_DIR}/toyundadrawer.cpp ${QTOYUNDA_DIR}/qosd.cpp)

set(QTOYUNDA_HEADERS ${QTOYUNDA_DIR}/qtoyunda.h ${QTOYUNDA_DIR}/fileplayer.h ${QTOYUNDA_DIR}/toyundasubstream.h ${QTOYUNDA_DIR}/toyundasub.h
		${QTOYUNDA_DIR}/toyundarenderer.h ${QTOYUNDA_DIR}/scomponent.h ${QTOYUNDA_DIR}/sqarg.h
		${QTOYUNDA_DIR}/qtoyunda.h ${QTOYUNDA_DIR}/debugrenderer.h ${QTOYUNDA_DIR}/qgstaudioplayer.h 
		${QTOYUNDA_DIR}/rawsubstream.h ${QTOYUNDA_DIR}/toyundadrawer.h ${QTOYUNDA_DIR}/qosd.h)


set(GuiliGuili_FORMS GuiliGuili.ui)
set(GuiliGuili_RESSOURCES icone.qrc)
set(GuiliGuili_SRCS playlistmodel.cpp playlist.cpp GuiliGuili.cpp main.cpp song.cpp)
set(GuiliGuili_HEADERS ${QTOYUNDA_HEADERS} playlistmodel.h playlist.h GuiliGuili.h song.h)
QT4_WRAP_CPP(GuiliGuili_MOC ${GuiliGuili_HEADERS})
QT4_WRAP_UI(GuiliGuili_FORMS_HEADERS ${GuiliGuili_FORMS})
QT4_ADD_RESOURCES(GuiliGuili_RESSOURCES_RCC ${GuiliGuili_RESSOURCES})


add_executable(GuiliGuili songlisttreeitemmodel.cpp ${GuiliGuili_FORMS_HEADERS} ${GuiliGuili_MOC} ${QTOYUNDA_SRC} ${GuiliGuili_SRCS} ${GuiliGuili_RESSOURCES_RCC})
target_link_libraries(GuiliGuili ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QTGSTREAMER_LIBRARIES})
