
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

add_custom_target(qt-thing ALL qmake-qt4 ../toyunda.pro 
		COMMAND make
		WORKING_DIRECTORY tmp)

add_subdirectory(gstreamer-toyunda)

set(PROJ_DATA_PATH ${CMAKE_INSTALL_PREFIX}/share/qtoyunda/)
set(PROJ_TRANSLATION_PATH ${CMAKE_INSTALL_PREFIX}/share/qtoyunda/translations/)
set(PROJ_PLUGINS_PATH ${CMAKE_INSTALL_PREFIX}/share/qtoyunda/plugins/)
set(GSTTOYUNDA_PLUGIN_PATH ${CMAKE_INSTALL_PREFIX}/share/gsttoyunda/)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/proj_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/commons/proj_config.h)

install(CODE "configure_file(${CMAKE_CURRENT_SOURCE_DIR}/proj_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/commons/proj_config.h)")
install(CODE "EXEC_PROGRAM(make ARGS CXXFLAGS=-DIS_INSTALL)") 

#Install things
#Binaries
install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/build/QToyTime
	      ${CMAKE_CURRENT_SOURCE_DIR}/build/GuiliGuili
	      ${CMAKE_CURRENT_SOURCE_DIR}/build/qtoy2lyr
	      ${CMAKE_CURRENT_SOURCE_DIR}/build/qttoyunda-player
	      ${CMAKE_CURRENT_SOURCE_DIR}/build/toyunda-player
	DESTINATION bin)

#Data file
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/build/toyunda.tga
	DESTINATION share/qtoyunda/)

#Plugins and translation
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/plugins/
	DESTINATION share/qtoyunda/plugins)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/translations/
	DESTINATION share/qtoyunda/translations/)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/gsttoyunda/
	DESTINATION share/qtoyunda/gsttoyunda/)

