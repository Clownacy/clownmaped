cmake_minimum_required(VERSION 3.5)

project(ClownMapEd VERSION 0.1 LANGUAGES CXX)

######
# Qt #
######

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
	"ClownMapEd.rc"
	"icon.qrc"
	"about.cpp"
	"about.h"
	"about.ui"
	"colour-button.cpp"
	"colour-button.h"
	"data-stream.h"
	"dynamic-pattern-load-cues.cpp"
	"dynamic-pattern-load-cues.h"
	"licence.cpp"
	"licence.h"
	"licence.ui"
	"main.cpp"
	"main-window.cpp"
	"main-window.h"
	"main-window.ui"
	"palette.cpp"
	"palette.h"
	"palette-editor.cpp"
	"palette-editor.h"
	"signal-wrapper.h"
	"sprite-viewer.cpp"
	"sprite-viewer.h"
	"sprite-frame.cpp"
	"sprite-frame.h"
	"sprite-mappings.cpp"
	"sprite-mappings.h"
	"sprite-piece.cpp"
	"sprite-piece.h"
	"sprite-piece-button.cpp"
	"sprite-piece-button.h"
	"sprite-piece-picker.cpp"
	"sprite-piece-picker.h"
	"tile-manager.cpp"
	"tile-manager.h"
	"tile-viewer.cpp"
	"tile-viewer.h"
	"utilities.cpp"
	"utilities.h"
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
	qt_add_executable(ClownMapEd
	MANUAL_FINALIZATION
	${PROJECT_SOURCES}
)
# Define target properties for Android with Qt 6 as:
#	set_property(TARGET clownmaped APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#				${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
	if(ANDROID)
		add_library(ClownMapEd SHARED
			${PROJECT_SOURCES}
		)
	# Define properties for Android with Qt 5 after find_package() calls as:
	#	set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
	else()
		add_executable(ClownMapEd
			${PROJECT_SOURCES}
		)
	endif()
endif()

target_link_libraries(ClownMapEd PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(ClownMapEd PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

install(TARGETS ClownMapEd
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
	qt_finalize_executable(ClownMapEd)
endif()

##########
# mdcomp #
##########

# We're using the Shell Extension's fork of FW-KENSC/mdcomp because
# upstream has a hard dependency on Boost which is revolting.
set(mdcomp_directory "FW-KENSC-ShellExt/FW-KENSC-ShellExt/FW-KENSC")

add_library(mdcomp STATIC
	# General stuff.
	"${mdcomp_directory}/basic_decoder.h"
	"${mdcomp_directory}/bigendian_io.h"
	"${mdcomp_directory}/bitstream.h"
	"${mdcomp_directory}/ignore_unused_variable_warning.h"
	"${mdcomp_directory}/lzss.h"
	"${mdcomp_directory}/moduled_adaptor.h"
	# Format-specific stuff.
	"${mdcomp_directory}/comper.cc"
	"${mdcomp_directory}/comper.h"
	"${mdcomp_directory}/kosinski.cc"
	"${mdcomp_directory}/kosinski.h"
	"${mdcomp_directory}/kosplus.cc"
	"${mdcomp_directory}/kosplus.h"
	"${mdcomp_directory}/nemesis.cc"
	"${mdcomp_directory}/nemesis.h"
)

target_include_directories(mdcomp PUBLIC ${mdcomp_directory})

target_link_libraries(ClownMapEd PRIVATE mdcomp)

#############
# CIEDE2000 #
#############

add_library(CIEDE2000 STATIC
	"CIEDE2000/CIEDE2000.cpp"
	"CIEDE2000/CIEDE2000.h"
)

target_include_directories(CIEDE2000 PRIVATE "CIEDE2000")

target_link_libraries(ClownMapEd PRIVATE CIEDE2000)

##################
# clownassembler #
##################

add_subdirectory("clownassembler" EXCLUDE_FROM_ALL)

target_link_libraries(ClownMapEd PRIVATE clownassembler_core)
