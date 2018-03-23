cmake_minimum_required(VERSION 2.8)

project(ogon-qt-greeter CXX)
set(CMAKE_COLOR_MAKEFILE ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include_directories("${CMAKE_BINARY_DIR}")

include(GNUInstallDirsWrapper)

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
if (CMAKE_EXTRA_RPATH) 
	set(CMAKE_INSTALL_RPATH "${CMAKE_EXTRA_RPATH}:${CMAKE_INSTALL_RPATH}")
else()
	set(CMAKE_INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
endif()

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

find_package(ogon 1 REQUIRED)
set(OGON_SHARE_DIR "${OGON_PROTOCOL_DIR}/protobuf/")

find_path(OGON_PROTOBUF_DIR
	NAMES SBP.proto
	HINTS ${OGON_SHARE_DIR})

if (NOT OGON_PROTOBUF_DIR)
	message(FATAL_ERROR "ogon protocol SBP.proto not found")
endif()

find_package(qogon REQUIRED)
include(FindProtobufCPP)
find_package(ProtobufCPP REQUIRED)

include_directories(${PROTOBUFC_INCLUDE_DIRS} ${QOGON_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

PROTOBUF_GENERATE_CPP(SBP_SOURCES SBP_HEADERS "${OGON_PROTOBUF_DIR}/SBP.proto")

set(QOGON_TRANSLATION_PATH "${CMAKE_INSTALL_PREFIX}/share/qt5/translations")

add_definitions(-DTRANSLATIONS_DIR=\"${QOGON_TRANSLATION_PATH}\")

# the module itself
set(MODULE_NAME "ogon-qt-greeter")

include(FindQT5)
find_package(Qt5LinguistTools REQUIRED)

set(FORMS forms/nice_greeter.ui forms/small_greeter.ui)

QT5_WRAP_UI(FORMS_sources ${FORMS})

qt5_add_resources(QT_RESOURCES ogon_qt_greeter.qrc)

SET(TRANS
	greeter_de.ts
	greeter_en.ts
	greeter_fr.ts
)

qt5_add_translation(QM ${TRANS})



set(${MODULE_NAME}_SRCS
	${FORMS_sources}
	abstract_greeter.cpp
	abstract_greeter.h
	lowres_greeter.cpp
	lowres_greeter.h
	nice_greeter.cpp
	nice_greeter.h	
	main.cpp
	${SBP_HEADERS}
	${SBP_SOURCES}
	${QT_RESOURCES}
	${QM}
)


add_executable(${MODULE_NAME} ${${MODULE_NAME}_SRCS})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${QT_EXECUTEABLE_FLAGS}")
include_directories(${WinPR_INCLUDE_DIR} ${Qt5Gui_PRIVATE_INCLUDE_DIRS})

target_link_libraries(${MODULE_NAME} ${QT_LIBRARIES} ${PROTOBUFCPP_LIBRARIES})

install(TARGETS ${MODULE_NAME} DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES ${QM} DESTINATION "${QOGON_TRANSLATION_PATH}")
