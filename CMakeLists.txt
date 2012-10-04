#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
# CMake file for vJoyQt project
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

PROJECT(VJOYQT)
MESSAGE(STATUS "CREATE V_JOY_QT")

# temp setting
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

# build libraries type
IF(NOT DEFINED BUILD_SHARED_LIBS)
    SET(BUILD_SHARED_LIBS ON)
ENDIF(NOT DEFINED BUILD_SHARED_LIBS)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

# add the include folder to the include path
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

SET(LIB_DIR ${PROJECT_SOURCE_DIR}/vjoyqt)

ADD_SUBDIRECTORY(${LIB_DIR})
ADD_SUBDIRECTORY(ui)

#==============================================================================
#------------------------------- install --------------------------------------
#==============================================================================

IF(UNIX)
    SET(CMAKE_INSTALL_PREFIX /usr/local)
ELSEIF(WIN32)
    SET(CMAKE_INSTALL_PREFIX c:/vjoyqt)
ENDIF(UNIX)

INSTALL(FILES vjoyqt/v_joystick_adapter.h DESTINATION include/vjoyqt)

ADD_CUSTOM_TARGET(uninstall @echo uninstall package)

ADD_CUSTOM_COMMAND(
    COMMENT "uninstall package"
    COMMAND xargs ARGS rm < install_manifest.txt
    COMMAND rm ARGS -r ${CMAKE_INSTALL_PREFIX}/include/vjoyqt
    TARGET  uninstall)
